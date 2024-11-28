#include "protocol/matchmaking_connection.h"

#include "protocol/matchmaking_handler.h"
#include "util/why_isnt_this_in_godot.h"

#include <godot_cpp/classes/json.hpp>
#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/offline_multiplayer_peer.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/variant/utility_functions.hpp>


constexpr char MATCHMAKING_CREATE_SESSION[] = "https://spy-cards.lubar.me/spy-cards/matchmaking/create-session";
constexpr char MATCHMAKING_JOIN[] = "https://spy-cards.lubar.me/spy-cards/matchmaking/join";
constexpr char MATCHMAKING_POLL[] = "https://spy-cards.lubar.me/spy-cards/matchmaking/poll";
constexpr char MATCHMAKING_SEND[] = "https://spy-cards.lubar.me/spy-cards/matchmaking/send";

void MatchmakingConnection::_bind_methods() {
	BIND_CONSTANT(PING_EVERY);
	BIND_CONSTANT(PING_COUNT);

	BIND_PROPERTY_RESOURCE(MatchmakingHandler, handler);
	BIND_PROPERTY(Variant::INT, remote_id);
	BIND_PROPERTY_RESOURCE(WebRTCPeerConnection, conn);
	BIND_PROPERTY(Variant::PACKED_INT64_ARRAY, ping_times);
	BIND_PROPERTY(Variant::INT, sent_commands);
	BIND_PROPERTY(Variant::INT, recv_commands);
	BIND_PROPERTY(Variant::PACKED_INT64_ARRAY, ping_start_times);
	BIND_PROPERTY(Variant::INT, ping_id);
	BIND_PROPERTY(Variant::INT, highest_remote_ping_id);
	BIND_PROPERTY_RESOURCE(HTTPRequest, sender);
	BIND_PROPERTY_RESOURCE(HTTPRequest, poller);
	BIND_PROPERTY(Variant::BOOL, sender_active);
	BIND_PROPERTY(Variant::STRING, send_queue);
	BIND_PROPERTY(Variant::INT, since_last_ping);
	BIND_PROPERTY(Variant::STRING, fatal_error);

	BIND_PROPERTY(Variant::BOOL, was_fully_connected);
	BIND_PROPERTY(Variant::BOOL, loaded_mode);

	BIND_PROPERTY(Variant::INT, frame_ack);
	BIND_PROPERTY(Variant::INT, local_frame_advantage);
	BIND_PROPERTY(Variant::INT, prev_local_frame_advantage);
	BIND_PROPERTY(Variant::INT, remote_frame_advantage);
	BIND_PROPERTY(Variant::INT, prev_remote_frame_advantage);
	BIND_PROPERTY(Variant::INT, suggested_drop_inputs);
	BIND_PROPERTY(Variant::PACKED_BYTE_ARRAY, realtime_inputs);

	ClassDB::add_signal("MatchmakingConnection", MethodInfo("encountered_fatal_error", PropertyInfo(Variant::STRING, "message")));
	ClassDB::add_signal("MatchmakingConnection", MethodInfo("fully_connected"));

	ClassDB::bind_method(D_METHOD("restart"), &MatchmakingConnection::restart);
	ClassDB::bind_method(D_METHOD("get_average_ping"), &MatchmakingConnection::get_average_ping);
} 

IMPLEMENT_PROPERTY_SIMPLE(MatchmakingConnection, MatchmakingHandler *, handler);
IMPLEMENT_PROPERTY_SIMPLE(MatchmakingConnection, int32_t, remote_id);
IMPLEMENT_PROPERTY_SIMPLE(MatchmakingConnection, Ref<WebRTCPeerConnection>, conn);
IMPLEMENT_PROPERTY_SIMPLE(MatchmakingConnection, PackedInt64Array, ping_times);
IMPLEMENT_PROPERTY_SIMPLE(MatchmakingConnection, int64_t, sent_commands);
IMPLEMENT_PROPERTY_SIMPLE(MatchmakingConnection, int64_t, recv_commands);
IMPLEMENT_PROPERTY_SIMPLE(MatchmakingConnection, PackedInt64Array, ping_start_times);
IMPLEMENT_PROPERTY_SIMPLE(MatchmakingConnection, int64_t, ping_id);
IMPLEMENT_PROPERTY_SIMPLE(MatchmakingConnection, int64_t, highest_remote_ping_id);
IMPLEMENT_PROPERTY_SIMPLE(MatchmakingConnection, HTTPRequest *, sender);
IMPLEMENT_PROPERTY_SIMPLE(MatchmakingConnection, HTTPRequest *, poller);
IMPLEMENT_PROPERTY_SIMPLE(MatchmakingConnection, bool, sender_active);
IMPLEMENT_PROPERTY_SIMPLE(MatchmakingConnection, String, send_queue);
IMPLEMENT_PROPERTY_SIMPLE(MatchmakingConnection, int64_t, since_last_ping);
IMPLEMENT_PROPERTY_SIMPLE(MatchmakingConnection, String, fatal_error);

IMPLEMENT_PROPERTY_SIMPLE(MatchmakingConnection, bool, was_fully_connected);
IMPLEMENT_PROPERTY_SIMPLE(MatchmakingConnection, bool, loaded_mode);

IMPLEMENT_PROPERTY_SIMPLE(MatchmakingConnection, int64_t, frame_ack);
IMPLEMENT_PROPERTY_SIMPLE(MatchmakingConnection, int32_t, local_frame_advantage);
IMPLEMENT_PROPERTY_SIMPLE(MatchmakingConnection, int32_t, prev_local_frame_advantage);
IMPLEMENT_PROPERTY_SIMPLE(MatchmakingConnection, int32_t, remote_frame_advantage);
IMPLEMENT_PROPERTY_SIMPLE(MatchmakingConnection, int32_t, prev_remote_frame_advantage);
IMPLEMENT_PROPERTY_SIMPLE(MatchmakingConnection, int32_t, suggested_drop_inputs);
IMPLEMENT_PROPERTY_SIMPLE(MatchmakingConnection, PackedByteArray, realtime_inputs);

MatchmakingConnection::MatchmakingConnection(MatchmakingHandler *handler, int32_t remote_id) {
	_handler = handler;
	_remote_id = remote_id;
	_loaded_mode = remote_id == 1;

	_conn.instantiate();
	_conn->initialize(handler->get_ice_config());
	_conn->connect("session_description_created", callable_mp(this, &MatchmakingConnection::_on_session_description_created));
	_conn->connect("ice_candidate_created", callable_mp(this, &MatchmakingConnection::_on_ice_candidate_created));

	_sender = memnew(HTTPRequest);
	_sender->set_name("Sender");
	add_child(_sender, false, INTERNAL_MODE_BACK);
	_sender->connect("request_completed", callable_mp(this, &MatchmakingConnection::_on_sender_request_completed));

	_poller = memnew(HTTPRequest);
	_poller->set_name("Poller");
	add_child(_poller, false, INTERNAL_MODE_BACK);
	_poller->connect("request_completed", callable_mp(this, &MatchmakingConnection::_on_poller_request_completed));
}

void MatchmakingConnection::_ready() {
	ERR_FAIL_NULL(_handler);

	Ref<OfflineMultiplayerPeer> offline = get_multiplayer()->get_multiplayer_peer();
	if (offline.is_valid() && _remote_id == 1) {
		_sender->request(
			MATCHMAKING_JOIN,
			Array::make("Content-Type: application/x-www-form-urlencoded"),
			HTTPClient::METHOD_POST,
			vformat("code=%s", _handler->get_lobby_id().uri_encode())
		);
		_sender_active = true;
		return;
	}

	_handler->get_peer()->add_peer(_conn, _remote_id);

	if (get_multiplayer()->get_unique_id() < _remote_id) {
		_conn->create_offer();
	}

	if (!_handler->get_lobby_id().is_empty()) {
		_start_poll_request();
	}
}
void MatchmakingConnection::_physics_process(double delta) {
	if (!_fatal_error.is_empty()) {
		return;
	}

	Ref<OfflineMultiplayerPeer> offline = get_multiplayer()->get_multiplayer_peer();
	if (offline.is_valid()) {
		_conn->poll();
	}

	switch (_conn->get_connection_state()) {
	case WebRTCPeerConnection::STATE_CONNECTED:
		_since_last_ping++;
		if (_since_last_ping >= PING_EVERY) {
			_since_last_ping %= PING_EVERY;
			_start_ping();
		}
		break;
	case WebRTCPeerConnection::STATE_FAILED:
	case WebRTCPeerConnection::STATE_CLOSED:
		if (_conn->get_signaling_state() == WebRTCPeerConnection::SIGNALING_STATE_STABLE) {
			if (_ping_times.is_empty()) {
				restart();
			} else {
				_on_fatal_error("Opponent disconnected");
			}
		}
		break;
	case WebRTCPeerConnection::STATE_NEW:
	case WebRTCPeerConnection::STATE_CONNECTING:
	case WebRTCPeerConnection::STATE_DISCONNECTED:
		break;
	}
}
void MatchmakingConnection::restart() {
	if (!_ping_times.is_empty()) {
		// can't restart ICE if we're not connected to the signaling server anymore
		return;
	}

	UtilityFunctions::print(vformat("Restarting connection between player %d and player %d", get_multiplayer()->get_unique_id(), _remote_id));
	_conn->initialize(_handler->get_ice_config());
	_conn->create_offer();
}
void MatchmakingConnection::_start_ping() {
	_ping_times.append(-1);
	_ping_start_times.append(Time::get_singleton()->get_ticks_usec());
	if (_ping_times.size() > PING_COUNT) {
		_ping_times.remove_at(0);
		_ping_start_times.remove_at(0);
	}

	int32_t behind = 0;

	if (_handler->get_state() == MatchmakingHandler::REALTIME) {
		// estimate how many frames the other side of the connection will have seen before this message gets there
		int64_t add_frames = Math::fast_ftoi(get_average_ping() * (60.0 / 1000.0));

		int64_t remote_frame = _realtime_inputs.size() + add_frames;
		int64_t local_frame = _handler->get_realtime_inputs().size();

		behind = remote_frame - local_frame;

		_prev_local_frame_advantage = _local_frame_advantage;
		_local_frame_advantage = behind;

		if (_ping_id > 10 && _ping_id % 2 == 0) {
			int32_t local_advantage = _local_frame_advantage + _prev_local_frame_advantage;
			int32_t remote_advantage = _remote_frame_advantage + _prev_remote_frame_advantage;
			_suggested_drop_inputs = local_advantage >= remote_advantage ? 0 : (remote_advantage - local_advantage) / 4;
			if (_suggested_drop_inputs < 3) {
				_suggested_drop_inputs = 0;
			}
			if (_suggested_drop_inputs > 9) {
				_suggested_drop_inputs = 9;
			}
		}
	}

	_ping_id++;
	_handler->rpc_id(_remote_id, "ping", _ping_id, behind);
}
void MatchmakingConnection::_ping(int64_t i, int32_t frames_behind) {
	_prev_remote_frame_advantage = _remote_frame_advantage;
	_remote_frame_advantage = frames_behind;
}
void MatchmakingConnection::_pong(int64_t i) {
	ERR_FAIL_COND_MSG(i > _ping_id, vformat("Player %d received ping response from player %d for ping number %d, but %d is the highest that has been sent!", get_multiplayer()->get_unique_id(), _remote_id, i, _ping_id));

	i -= _ping_id - (_ping_times.size() - 1);
	if (unlikely(i < 0)) {
		// very late ping response
		return;
	}

	// should have been caught above, but double check.
	ERR_FAIL_INDEX(i, _ping_times.size());

	_ping_times[i] = Time::get_singleton()->get_ticks_usec() - _ping_start_times[i];

	if (unlikely(!_was_fully_connected)) {
		// since we successfully sent and recieved an RPC call, we're done with matchmaking for this peer.
		_poller->cancel_request();

		_was_fully_connected = true;
		emit_signal("fully_connected");
	}
}
double MatchmakingConnection::get_average_ping() const {
	int64_t total = 0;
	int64_t count = 0;

	for (int64_t i = 0; i < _ping_times.size(); i++) {
		int64_t time = _ping_times[i];
		if (time == -1) {
			continue;
		}

		total += time;
		count++;
	}

	return double(total) / double(count) * 0.001; // convert usec to msec
}

void MatchmakingConnection::_on_session_description_created(const String &type, const String &sdp) {
	_conn->set_local_description(type, sdp);
	_send_command("d", PackedStringArray{type, sdp});
}
void MatchmakingConnection::_on_ice_candidate_created(const String &media, int64_t index, const String &candidate) {
	_send_command("c", PackedStringArray{media, String::num_int64(index), candidate});
}
void MatchmakingConnection::_on_command_received(const String &command, const PackedStringArray &args) {
	_recv_commands++;

	if (command == "d") {
		ERR_FAIL_COND(args.size() != 2);

		if (args[0] == "offer" && _conn->get_signaling_state() == WebRTCPeerConnection::SIGNALING_STATE_HAVE_LOCAL_OFFER) {
			// we're swapping the dialer and the answerer
			_conn->initialize(_handler->get_ice_config());
		}

		_conn->set_remote_description(args[0], args[1]);

		return;
	}

	if (command == "c") {
		ERR_FAIL_COND(args.size() != 3);
		ERR_FAIL_COND(!args[1].is_valid_int());

		_conn->add_ice_candidate(args[0], args[1].to_int(), args[2]);

		return;
	}

	_on_fatal_error(vformat("unknown matchmaking command %s", command));
}
void MatchmakingConnection::_on_sender_request_completed(HTTPRequest::Result result, HTTPClient::ResponseCode response_code, const PackedStringArray &headers, const PackedByteArray &body) {
	if (unlikely(result != HTTPRequest::RESULT_SUCCESS)) {
		_on_fatal_error(vformat("sending matchmaking request failed with %s", WhyIsntThisInGodot::find_builtin_enum_key_name("HTTPRequest", "Result", result)));
		return;
	}

	String body_str = body.get_string_from_utf8();
	if (response_code == HTTPClient::RESPONSE_CREATED) {
		ERR_FAIL_COND_MSG(get_multiplayer()->get_unique_id() != 1, "only player 1 should have created a lobby");
		ERR_FAIL_COND_MSG(!_handler->get_lobby_id().is_empty(), "lobby was already created");

		PackedStringArray lines = body_str.split("\n");
		ERR_FAIL_COND(lines.size() != 2);
		_handler->_on_lobby_created(lines[0], lines[1]);

		_start_poll_request();
	} else if (response_code == HTTPClient::RESPONSE_ACCEPTED) {
		ERR_FAIL_COND_MSG(_handler->get_lobby_id().is_empty(), "missing lobby");
		ERR_FAIL_COND_MSG(!body_str.is_empty(), "expected empty body");
	} else if (response_code == HTTPClient::RESPONSE_OK) {
		ERR_FAIL_COND_MSG(_handler->get_lobby_id().is_empty(), "missing lobby");
		ERR_FAIL_COND_MSG(!Object::cast_to<OfflineMultiplayerPeer>(get_multiplayer()->get_multiplayer_peer().ptr()), "we are joining from an already-online state");
		ERR_FAIL_COND_MSG(_remote_id != 1, "connecting to a player other than player 1 before we know our player number");

		PackedStringArray lines = body_str.split("\n");
		ERR_FAIL_COND(lines.size() != 2);
		ERR_FAIL_COND(!lines[0].is_valid_int());
		_handler->_on_player_id(lines[0].to_int(), lines[1]);
	} else if (response_code == HTTPClient::RESPONSE_NOT_FOUND) {
		_on_fatal_error(vformat("Matchmaking server responded with:\n%s", body_str));
	} else {
		_on_fatal_error(vformat("Sending matchmaking request failed: %s\n%s", WhyIsntThisInGodot::find_builtin_enum_key_name("HTTPClient", "ResponseCode", response_code), body_str));
	}

	_sender_active = false;
	if (!_send_queue.is_empty()) {
		_start_send_request();
	}
}
void MatchmakingConnection::_on_poller_request_completed(HTTPRequest::Result result, HTTPClient::ResponseCode response_code, const PackedStringArray &headers, const PackedByteArray &body) {
	if (result != HTTPRequest::RESULT_SUCCESS) {
		_on_fatal_error(vformat("Receiving matchmaking request failed: %s", WhyIsntThisInGodot::find_builtin_enum_key_name("HTTPRequest", "Result", result)));
		return;
	}

	String body_str = body.get_string_from_utf8();
	if (response_code != HTTPClient::RESPONSE_OK) {
		_on_fatal_error(vformat("Receiving matchmaking request failed: %s\n%s", WhyIsntThisInGodot::find_builtin_enum_key_name("HTTPClient", "ResponseCode", response_code), body_str));
		return;
	}

	PackedStringArray encoded_commands = body_str.split("\n", false);
	for (String encoded_command : encoded_commands) {
		String command = encoded_command.substr(0, 1);
		PackedStringArray args = JSON::parse_string(encoded_command.substr(1));
		_on_command_received(command, args);
	}

	_start_poll_request();
}
void MatchmakingConnection::_send_command(const String &command, const PackedStringArray &args) {
	ERR_FAIL_COND_MSG(command.length() != 1, "command should be 1 character");

	String cmd = command + JSON::stringify(args);
	_sent_commands++;

	if (_handler->get_lobby_id().is_empty() && !_sender_active) {
		ERR_FAIL_COND_MSG(get_multiplayer()->get_unique_id() != 1, "player 1 should create the lobby");
		ERR_FAIL_COND_MSG(_remote_id != 2, "player 1 should invite player 2 first");
		ERR_FAIL_COND_MSG(command != "d", "lobby creation command should be [d]escription");
		ERR_FAIL_COND_MSG(args[0] != "offer", "lobby creation description should be an offer");

		_sender->request(
			MATCHMAKING_CREATE_SESSION,
			Array::make("Content-Type: application/x-www-form-urlencoded"),
			HTTPClient::METHOD_POST,
			vformat("cmd=%s&maxplayers=%d", cmd.uri_encode(), _handler->get_max_players())
		);
		_sender_active = true;
		return;
	}

	if (!_send_queue.is_empty()) {
		_send_queue += "\n";
	}
	_send_queue += cmd;

	if (!_sender_active) {
		_start_send_request();
	}
}
String MatchmakingConnection::_post_params() const {
	return vformat(
		"code=%s&verify=%s&from=%d&to=%d",
		_handler->get_lobby_id().uri_encode(),
		_handler->get_verification_code().uri_encode(),
		get_multiplayer()->get_unique_id(),
		_remote_id
	);
}
void MatchmakingConnection::_start_send_request() {
	ERR_FAIL_COND_MSG(_sender_active, "overlapping sends");
	ERR_FAIL_COND_MSG(_send_queue.is_empty(), "empty send queue");

	_sender->request(
		MATCHMAKING_SEND,
		Array::make("Content-Type: application/x-www-form-urlencoded"),
		HTTPClient::METHOD_POST,
		_post_params() + "&cmd=" + _send_queue.uri_encode()
	);
	_sender_active = true;
	_send_queue = "";
}
void MatchmakingConnection::_start_poll_request() {
	_poller->request(
		MATCHMAKING_POLL,
		Array::make("Content-Type: application/x-www-form-urlencoded"),
		HTTPClient::METHOD_POST,
		_post_params()
	);
}
void MatchmakingConnection::_on_fatal_error(const String &text) {
	_fatal_error = text;
	ERR_PRINT(text);

	emit_signal("encountered_fatal_error", text);

	_sender->cancel_request();
	_poller->cancel_request();
	_conn->close();
}
