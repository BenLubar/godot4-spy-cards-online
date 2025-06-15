#include "protocol/spy_cards_lobby_connection.h"

#include "util/why_isnt_this_in_godot.h"

#include <godot_cpp/classes/json.hpp>
#include <godot_cpp/classes/offline_multiplayer_peer.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/classes/web_rtc_multiplayer_peer.hpp>

const static LazyStringName FATAL_ERROR{"fatal_error"};
const static LazyStringName JOINED_LOBBY{"joined_lobby"};

const static LazyStringName ICE_CANDIDATE_CREATED{"ice_candidate_created"};
const static LazyStringName SESSION_DESCRIPTION_CREATED{"session_description_created"};

const static LazyStringName NET_PING{"net_ping"};
const static LazyStringName NET_PONG{"net_pong"};

constexpr static double PING_INTERVAL = 1.0;

void SpyCardsLobbyConnection::_bind_methods() {
	ClassDB::bind_method(D_METHOD("create_lobby", "max_players"), &SpyCardsLobbyConnection::create_lobby);
	ClassDB::bind_method(D_METHOD("join_lobby", "lobby_id"), &SpyCardsLobbyConnection::join_lobby);

	ClassDB::bind_method(D_METHOD("get_lobby_id"), &SpyCardsLobbyConnection::get_lobby_id);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "lobby_id"), "", "get_lobby_id");

	ADD_SIGNAL(MethodInfo(FATAL_ERROR, PropertyInfo(Variant::STRING, "message")));
	ADD_SIGNAL(MethodInfo(JOINED_LOBBY));

	ClassDB::bind_method(D_METHOD(NET_PING, "sequence_number"), &SpyCardsLobbyConnection::net_ping);
	ClassDB::bind_method(D_METHOD(NET_PONG, "sequence_number"), &SpyCardsLobbyConnection::net_pong);
}

SpyCardsLobbyConnection::SpyCardsLobbyConnection() {
	rpc_config(NET_PING, make_rpc_config(MultiplayerAPI::RPC_MODE_ANY_PEER, MultiplayerPeer::TRANSFER_MODE_UNRELIABLE, false, 1));
	rpc_config(NET_PONG, make_rpc_config(MultiplayerAPI::RPC_MODE_ANY_PEER, MultiplayerPeer::TRANSFER_MODE_UNRELIABLE, false, 1));
}

Dictionary SpyCardsLobbyConnection::make_rpc_config(MultiplayerAPI::RPCMode p_rpc_mode, MultiplayerPeer::TransferMode p_transfer_mode, bool p_call_local, int32_t p_channel) {
	Dictionary config;
	config["rpc_mode"] = p_rpc_mode;
	config["transfer_mode"] = p_transfer_mode;
	config["call_local"] = p_call_local;
	config["channel"] = p_channel;
	return config;
}

static Array get_channels_config() {
	return {{
		MultiplayerPeer::TRANSFER_MODE_UNRELIABLE, // channel 1: PING
	}};
}

void SpyCardsLobbyConnection::_physics_process(double p_delta) {
	bool should_ping = false;
	if (!is_empty() && !_waiting_to_create && !_waiting_to_join) {
		_ping_timer += p_delta;
		if (_ping_timer >= PING_INTERVAL) {
			_ping_timer -= PING_INTERVAL;
			_ping_sequence_number++;
			should_ping = true;
		}
	}

	for (int64_t i = 0; i < _peers.size(); i++) {
		_peers[i].conn->poll();
		if (_peers[i].conn->get_connection_state() != WebRTCPeerConnection::ConnectionState::STATE_NEW && _peers[i].conn->get_signaling_state() == WebRTCPeerConnection::SIGNALING_STATE_STABLE && !_peers[i].pending_matchmaking_commands.is_empty() && (_matchmaking_send[i].is_null() || !_matchmaking_send[i]->get_request())) {
			_start_send(i, String("\n").join(_peers[i].pending_matchmaking_commands));
			_peers.write[i].pending_matchmaking_commands.clear();
		}

		int32_t index = i + 1;
		if (index >= _player_index) {
			index++;
		}

		if (should_ping) {
			_peers.write[i].ping_received[_ping_sequence_number % PING_HISTORY_LENGTH] = 0;
			if (get_multiplayer()->get_peers().has(index)) {
				_peers.write[i].ping_sent[_ping_sequence_number % PING_HISTORY_LENGTH] = Time::get_singleton()->get_ticks_usec();
				rpc_id(index, NET_PING, _ping_sequence_number);
			} else {
				_peers.write[i].ping_sent[_ping_sequence_number % PING_HISTORY_LENGTH] = 0;
			}
		}

#if 0
		print_line(vformat("p%d->%d %s %s %s", _player_index, index,
			WhyIsntThisInGodot::find_builtin_enum_key_name("WebRTCPeerConnection", "ConnectionState", _peers[i].conn->get_connection_state()),
			WhyIsntThisInGodot::find_builtin_enum_key_name("WebRTCPeerConnection", "GatheringState", _peers[i].conn->get_gathering_state()),
			WhyIsntThisInGodot::find_builtin_enum_key_name("WebRTCPeerConnection", "SignalingState", _peers[i].conn->get_signaling_state())));
#endif
	}
}

bool SpyCardsLobbyConnection::is_empty() const {
	if (!_lobby_id.is_empty() || !_matchmaking_error.is_empty()) {
		return false;
	}

	if (_waiting_to_create || _waiting_to_join) {
		return false;
	}

	DEV_ASSERT(_multiplyer_peer.is_null());
	DEV_ASSERT(_player_index == 0);
	DEV_ASSERT(_max_players == 0);
	DEV_ASSERT(_matchmaking_poll.is_empty());
	DEV_ASSERT(_matchmaking_send.is_empty());
	DEV_ASSERT(_matchmaking_init.is_null());
	DEV_ASSERT(_peers.is_empty());

	return true;
}

void SpyCardsLobbyConnection::create_lobby(int32_t p_max_players) {
	ERR_FAIL_COND(!is_inside_tree());
	ERR_FAIL_COND(!is_empty());

	_player_index = 1;
	_max_players = p_max_players;

	if (unlikely(p_max_players < 0)) {
		_on_fatal_error("Max players cannot be negative");
		return;
	}

	if (p_max_players <= 1) {
		_lobby_id = vformat("local_%d", get_instance_id());
		_verification = "local";
		_multiplyer_peer = Ref<OfflineMultiplayerPeer>(memnew(OfflineMultiplayerPeer));
		_on_join_lobby();
		return;
	}

	Ref<WebRTCMultiplayerPeer> multiplayer_peer;
	multiplayer_peer.instantiate();
	_multiplyer_peer = multiplayer_peer;

	Error err = multiplayer_peer->create_mesh(1, get_channels_config());
	if (unlikely(err != OK)) {
		_on_fatal_error(vformat("Failed to create lobby: creating mesh network: %s", UtilityFunctions::error_string(err)));
		return;
	}

	_waiting_for_configuration = true;
	_waiting_to_create = true;

	_matchmaking_configuration = SpyCardsClient::get_singleton()->matchmaking_get_configuration();
	_matchmaking_configuration->connect_request_completed(callable_mp(this, &SpyCardsLobbyConnection::_on_init_config));
}

void SpyCardsLobbyConnection::join_lobby(const String &p_lobby_id) {
	ERR_FAIL_COND(!is_inside_tree());
	ERR_FAIL_COND(!is_empty());

	if (unlikely(p_lobby_id.is_empty())) {
		_on_fatal_error("Lobby ID cannot be empty");
		return;
	}

	_lobby_id = p_lobby_id;
	_waiting_for_configuration = true;
	_waiting_to_join = true;

	_matchmaking_configuration = SpyCardsClient::get_singleton()->matchmaking_get_configuration();
	_matchmaking_configuration->connect_request_completed(callable_mp(this, &SpyCardsLobbyConnection::_on_init_config));

	_matchmaking_init = SpyCardsClient::get_singleton()->matchmaking_join(p_lobby_id);
	_matchmaking_init->connect_request_completed(callable_mp(this, &SpyCardsLobbyConnection::_on_init_join));
}

String SpyCardsLobbyConnection::get_lobby_id() const {
	return _lobby_id;
}

void SpyCardsLobbyConnection::_on_ice_candidate_created(const String &p_media, int32_t p_index, const String &p_name, int32_t p_peer) {
	_peers.write[p_peer].pending_matchmaking_commands.append("i" + JSON::stringify(Array::make(p_media, p_index, p_name)));
}

void SpyCardsLobbyConnection::_on_lobby_creator_session_description_created(const String &p_type, const String &p_sdp) {
	if (unlikely(p_type != "offer")) {
		_on_fatal_error(vformat("Expected session description type to be 'offer', but it was '%s'", p_type));
		return;
	}

	Error err = _peers[0].conn->set_local_description(p_type, p_sdp);
	if (unlikely(err != OK)) {
		_on_fatal_error(vformat("Failed to create lobby: setting local SDP description: %s", UtilityFunctions::error_string(err)));
		return;
	}

	_peers.write[0].pending_matchmaking_commands.insert(0, "o" + JSON::stringify(Array::make(p_sdp));
	_matchmaking_init = SpyCardsClient::get_singleton()->matchmaking_create_session(String("\n").join(_peers[0].pending_matchmaking_commands), _max_players);
	_matchmaking_init->connect_request_completed(callable_mp(this, &SpyCardsLobbyConnection::_on_init_create_session));
	_peers.write[0].pending_matchmaking_commands.clear();
}

void SpyCardsLobbyConnection::_on_offer_session_description_created(const String &p_type, const String &p_sdp, int32_t p_peer) {
	if (unlikely(p_type != "offer")) {
		_on_fatal_error(vformat("Expected session description type to be 'offer', but it was '%s'", p_type));
		return;
	}

	Error err = _peers[p_peer].conn->set_local_description(p_type, p_sdp);
	if (unlikely(err != OK)) {
		_on_fatal_error(vformat("Setting local SDP description for player %d: %s", p_peer + 2, UtilityFunctions::error_string(err)));
		return;
	}

	_peers.write[p_peer].pending_matchmaking_commands.insert(0, "o" + JSON::stringify(Array::make(p_sdp)));
	_start_send(p_peer, String("\n").join(_peers[p_peer].pending_matchmaking_commands));
	_peers.write[p_peer].pending_matchmaking_commands.clear();
}

void SpyCardsLobbyConnection::_on_answer_session_description_created(const String &p_type, const String &p_sdp, int32_t p_peer) {
	if (unlikely(p_type != "answer")) {
		_on_fatal_error(vformat("Expected session description type to be 'answer', but it was '%s'", p_type));
		return;
	}

	Error err = _peers[p_peer].conn->set_local_description(p_type, p_sdp);
	if (unlikely(err != OK)) {
		_on_fatal_error(vformat("Setting local SDP description for player %d: %s", p_peer + 1, UtilityFunctions::error_string(err)));
		return;
	}

	_peers.write[p_peer].pending_matchmaking_commands.insert(0, "a" + JSON::stringify(Array::make(p_sdp)));
	_start_send(p_peer, String("\n").join(_peers[p_peer].pending_matchmaking_commands));
	_peers.write[p_peer].pending_matchmaking_commands.clear();
}

void SpyCardsLobbyConnection::_on_fatal_error(const String &p_message, bool p_forwarded) {
	ERR_FAIL_COND_MSG(!_matchmaking_error.is_empty(), vformat("%s: Additional matchmaking error: %s", this, p_message));
	ERR_PRINT(vformat("%s: Matchmaking error: %s", this, p_message));
	_matchmaking_error = p_message;
	emit_signal(FATAL_ERROR, p_message);

	if (!p_forwarded) {
		const String error_notification = "f" + JSON::stringify(p_message);
		for (PeerConnection &peer : _peers) {
			peer.pending_matchmaking_commands.append(error_notification);
		}
	}
}

void SpyCardsLobbyConnection::_on_join_lobby() {
	DEV_ASSERT(!_lobby_id.is_empty());

	set_name("Lobby_" + _lobby_id);

	Ref<MultiplayerAPI> multiplayer = MultiplayerAPI::create_default_interface();
	multiplayer->set_multiplayer_peer(_multiplyer_peer);
	get_tree()->set_multiplayer(multiplayer, get_path());

	_waiting_to_create = false;
	_waiting_to_join = false;

	if (_max_players > 1) {
		_matchmaking_send.resize(_max_players - 1);

		for (int32_t i = 0; i < _player_index - 1; i++) {
			_peers[i].conn->connect(SESSION_DESCRIPTION_CREATED, callable_mp(this, &SpyCardsLobbyConnection::_on_answer_session_description_created).bind(i));
		}

		for (int32_t i = _player_index - 1; i < _max_players - 1; i++) {
			_peers[i].conn->connect(SESSION_DESCRIPTION_CREATED, callable_mp(this, &SpyCardsLobbyConnection::_on_offer_session_description_created).bind(i));
			if (i == 0 && _player_index == 1) {
				// we already created the initial offer for player 2 during lobby creation
				continue;
			}

			Error err = _peers[i].conn->create_offer();
			if (unlikely(err != OK)) {
				_on_fatal_error(vformat("Error during join: creating WebRTC offer for player %d: %s", i + 2, UtilityFunctions::error_string(err)));
				return;
			}
		}

		_matchmaking_poll.resize(_max_players - 1);
		for (int32_t i = 0; i < _matchmaking_poll.size(); i++) {
			_start_poll(i);
		}
	}

	emit_signal(JOINED_LOBBY);
}

void SpyCardsLobbyConnection::_start_send(int32_t p_peer, const String &p_command) {
	DEV_ASSERT_ASSUME(p_peer >= 0 && p_peer < _matchmaking_send.size());
	DEV_ASSERT(_matchmaking_send[p_peer].is_null() || !_matchmaking_send[p_peer]->get_request());

	int32_t index = p_peer + 1;
	if (index >= _player_index) {
		index++;
	}

	_matchmaking_send.write[p_peer] = SpyCardsClient::get_singleton()->matchmaking_send(_lobby_id, _player_index, index, _verification, p_command);
	_matchmaking_send[p_peer]->connect_request_completed(callable_mp(this, &SpyCardsLobbyConnection::_on_send_completed).bind(p_peer));
}

void SpyCardsLobbyConnection::_start_poll(int32_t p_peer) {
	DEV_ASSERT_ASSUME(p_peer >= 0 && p_peer < _matchmaking_poll.size());
	DEV_ASSERT(_matchmaking_poll[p_peer].is_null() || !_matchmaking_poll[p_peer]->get_request());

	int32_t index = p_peer + 1;
	if (index >= _player_index) {
		index++;
	}

	_matchmaking_poll.write[p_peer] = SpyCardsClient::get_singleton()->matchmaking_poll(_lobby_id, _player_index, index, _verification);
	_matchmaking_poll[p_peer]->connect_request_completed(callable_mp(this, &SpyCardsLobbyConnection::_on_poll_completed).bind(p_peer));
}

bool SpyCardsLobbyConnection::_check_http_response(const String &p_request_name, const Ref<SpyCardsClientRequest> &p_request, HTTPClient::ResponseCode p_expected_response) {
	if (p_request->get_result() != HTTPRequest::RESULT_SUCCESS) {
		_on_fatal_error(vformat("HTTP request '%s' failed: %s", p_request_name, WhyIsntThisInGodot::find_builtin_enum_key_name("HTTPRequest", "Result", p_request->get_result())));
		return false;
	}

	if (p_request->get_response_code() != p_expected_response) {
		_on_fatal_error(vformat("HTTP request '%s' failed: expecting %s but got %s: \"%s\"", p_request_name, WhyIsntThisInGodot::find_builtin_enum_key_name("HTTPClient", "ResponseCode", p_expected_response), WhyIsntThisInGodot::find_builtin_enum_key_name("HTTPClient", "ResponseCode", p_request->get_response_code()), p_request->get_body().get_string_from_utf8()));
		return false;
	}

	return true;
}

static bool is_ice_server_credentialed(const Dictionary &p_server) {
	return p_server.has("credential");
}

void SpyCardsLobbyConnection::_on_init_config() {
	if (_matchmaking_configuration->get_result() == HTTPRequest::RESULT_SUCCESS && _matchmaking_configuration->get_response_code() == HTTPClient::RESPONSE_OK) {
		_rtc_config = JSON::parse_string(_matchmaking_configuration->get_body().get_string_from_utf8());
	}

	if (_rtc_config.is_empty()) {
		_rtc_config = ProjectSettings::get_singleton()->get_setting_with_override("spy_cards_online/webrtc_config");
	}

	_is_turn_available = _rtc_config["iceServers"].operator Array().any(callable_mp_static(&is_ice_server_credentialed));
	_is_turn_forced = _rtc_config["iceTransportPolicy"] == "relay";

	_waiting_for_configuration = false;

	if (_waiting_to_create) {
		const Ref<WebRTCMultiplayerPeer> multiplayer_peer = _multiplyer_peer;
		_peers.resize(_max_players - 1);
		for (int64_t i = 0; i < _peers.size(); i++) {
			_peers.write[i].conn.instantiate();
			_peers[i].conn->initialize(_rtc_config);
			_peers[i].conn->connect(ICE_CANDIDATE_CREATED, callable_mp(this, &SpyCardsLobbyConnection::_on_ice_candidate_created).bind(i));
			const Error err = multiplayer_peer->add_peer(_peers[i].conn, i + 2);
			if (unlikely(err != OK)) {
				_on_fatal_error(vformat("Failed to create lobby: pre-adding peer for player %d: %s", i + 2, UtilityFunctions::error_string(err)));
				return;
			}
		}

		_peers[0].conn->connect(SESSION_DESCRIPTION_CREATED, callable_mp(this, &SpyCardsLobbyConnection::_on_lobby_creator_session_description_created), CONNECT_ONE_SHOT);
		const Error err = _peers[0].conn->create_offer();
		if (unlikely(err != OK)) {
			_on_fatal_error(vformat("Failed to create lobby: creating WebRTC offer: %s", UtilityFunctions::error_string(err)));
			return;
		}
	} else if (_waiting_to_join && _matchmaking_init.is_valid() && !_matchmaking_init->get_request()) {
		_on_init_join();
	}
}

void SpyCardsLobbyConnection::_on_init_create_session() {
	if (!_check_http_response("create-lobby", _matchmaking_init, HTTPClient::RESPONSE_CREATED)) {
		return;
	}

	const PackedStringArray data = _matchmaking_init->get_body().get_string_from_utf8().split("\n");
	DEV_ASSERT(data.size() == 2);
	DEV_ASSERT(!data[0].is_empty());

	_lobby_id = data[0];
	_verification = data[1];

	_on_join_lobby();
}

void SpyCardsLobbyConnection::_on_init_join() {
	if (_waiting_for_configuration) {
		return;
	}

	if (!_check_http_response("join-lobby", _matchmaking_init, HTTPClient::RESPONSE_OK)) {
		return;
	}

	const PackedStringArray data = _matchmaking_init->get_body().get_string_from_utf8().split("\n");
	DEV_ASSERT(data.size() == 3);
	DEV_ASSERT(data[0].is_valid_int());
	DEV_ASSERT(data[1].is_valid_int());

	_max_players = data[0].to_int();
	_player_index = data[1].to_int();
	DEV_ASSERT(_player_index > 1);
	DEV_ASSERT(_player_index <= _max_players);
	_verification = data[2];

	Ref<WebRTCMultiplayerPeer> multiplayer_peer;
	multiplayer_peer.instantiate();
	_multiplyer_peer = multiplayer_peer;

	Error err = multiplayer_peer->create_mesh(_player_index, get_channels_config());
	if (unlikely(err != OK)) {
		_on_fatal_error(vformat("Failed to join lobby: creating mesh network: %s", UtilityFunctions::error_string(err)));
		return;
	}

	_peers.resize(_max_players - 1);
	for (int64_t i = 0; i < _peers.size(); i++) {
		_peers.write[i].conn.instantiate();
		_peers[i].conn->initialize(_rtc_config);
		_peers[i].conn->connect(ICE_CANDIDATE_CREATED, callable_mp(this, &SpyCardsLobbyConnection::_on_ice_candidate_created).bind(i));

		int64_t index = i + 1;
		if (index >= _player_index) {
			index++;
		}

		err = multiplayer_peer->add_peer(_peers[i].conn, index);
		if (unlikely(err != OK)) {
			_on_fatal_error(vformat("Failed to join lobby: pre-adding peer for player %d: %s", index, UtilityFunctions::error_string(err)));
			return;
		}
	}

	_on_join_lobby();
}

void SpyCardsLobbyConnection::_on_send_completed(int32_t p_peer) {
	if (!_check_http_response("lobby-send", _matchmaking_send[p_peer], HTTPClient::RESPONSE_ACCEPTED)) {
		return;
	}

	DEV_ASSERT(_matchmaking_send[p_peer]->get_body().is_empty());
}

void SpyCardsLobbyConnection::_on_poll_completed(int32_t p_peer) {
	if (!_check_http_response("lobby-poll", _matchmaking_poll[p_peer], HTTPClient::RESPONSE_OK)) {
		return;
	}

	int32_t index = p_peer + 1;
	if (index >= _player_index) {
		index++;
	}

	const PackedStringArray commands = _matchmaking_poll[p_peer]->get_body().get_string_from_utf8().split("\n");
	for (const String &command : commands) {
		if (command.is_empty()) {
			continue;
		}
		const int64_t command_number = command.unicode_at(0);
		const Variant command_payload = JSON::parse_string(command.substr(1));
		Array a;
		Error err;

#define CHECK_PAYLOAD_TYPE(m_expected_type) \
		if (command_payload.get_type() != Variant::m_expected_type) { \
			_on_fatal_error(vformat("From player %d: bad data type %s for command number %d", index, Variant::get_type_name(command_payload.get_type()), command_number)); \
			return; \
		}

		switch (command_number) {
		case 'o': // SDP offer
			if (index > _player_index) {
				_on_fatal_error(vformat("From player %d: unexpected SDP offer", index));
				return;
			}

			CHECK_PAYLOAD_TYPE(ARRAY);
			a = command_payload;
			if (a.size() != 1 || a[0].get_type() != Variant::STRING) {
				_on_fatal_error(vformat("From player %d: malformed SDP offer", index));
				return;
			}

			err = _peers[p_peer].conn->set_remote_description("offer", a[0]);
			if (unlikely(err != OK)) {
				_on_fatal_error(vformat("From player %d: error setting SDP offer: %s", index, UtilityFunctions::error_string(err)));
				return;
			}

			break;
		case 'a': // SDP answer
			if (index < _player_index) {
				_on_fatal_error(vformat("From player %d: unexpected SDP answer", index));
				return;
			}

			CHECK_PAYLOAD_TYPE(ARRAY);
			a = command_payload;
			if (a.size() != 1 || a[0].get_type() != Variant::STRING) {
				_on_fatal_error(vformat("From player %d: malformed SDP answer", index));
				return;
			}

			err = _peers[p_peer].conn->set_remote_description("answer", a[0]);
			if (unlikely(err != OK)) {
				_on_fatal_error(vformat("From player %d: error setting SDP answer: %s", index, UtilityFunctions::error_string(err)));
				return;
			}

			break;
		case 'i': // ICE candidate
			CHECK_PAYLOAD_TYPE(ARRAY);
			a = command_payload;
			if (a.size() != 3 || a[0].get_type() != Variant::STRING || a[1].get_type() != Variant::FLOAT || a[2].get_type() != Variant::STRING) {
				_on_fatal_error(vformat("From player %d: malformed ICE candidate", index));
				return;
			}

			err = _peers[p_peer].conn->add_ice_candidate(a[0], a[1], a[2]);
			if (unlikely(err != OK)) {
				_on_fatal_error(vformat("From player %d: error adding ICE candidate: %s", index, UtilityFunctions::error_string(err)));
				return;
			}

			break;
		case 'f': // fatal error
			CHECK_PAYLOAD_TYPE(STRING);
			_on_fatal_error(vformat("Player %d encountered a fatal error: \"%s\"", index, command_payload), true);
			return;
		default:
			_on_fatal_error(vformat("From player %d: unknown command number %d", index, command_number));
			return;
		}

#undef CHECK_PAYLOAD_TYPE
	}

	if (_matchmaking_error.is_empty()) {
		_start_poll(p_peer);
	}
}

void SpyCardsLobbyConnection::net_ping(uint64_t p_sequence_number) {
	const int32_t peer = get_multiplayer()->get_remote_sender_id();
	ERR_FAIL_COND_MSG(peer == 0, "net_ping should only be called via RPC!");

	rpc_id(peer, NET_PONG, p_sequence_number);
}

void SpyCardsLobbyConnection::net_pong(uint64_t p_sequence_number) {
	const int32_t peer = get_multiplayer()->get_remote_sender_id();
	ERR_FAIL_COND_MSG(peer == 0, "net_pong should only be called via RPC!");

	ERR_FAIL_COND_MSG(_ping_sequence_number < p_sequence_number, vformat("received a reply to a ping we haven't sent yet (%d > %d) from player %d", p_sequence_number, _ping_sequence_number, peer));

	if (_ping_sequence_number < PING_HISTORY_LENGTH || p_sequence_number > _ping_sequence_number - PING_HISTORY_LENGTH) {
		const int32_t i = peer - (peer > _player_index ? 2 : 1);
		ERR_FAIL_COND_MSG(_peers[i].ping_sent[p_sequence_number % PING_HISTORY_LENGTH] == 0, vformat("received a reply to a ping we didn't send (%d/%d) from player %d", p_sequence_number, _ping_sequence_number, peer));
		ERR_FAIL_COND_MSG(_peers[i].ping_received[p_sequence_number % PING_HISTORY_LENGTH] != 0, vformat("received duplicate reply to ping %d/%d from player %d", p_sequence_number, _ping_sequence_number, peer));
		_peers.write[i].ping_received[p_sequence_number % PING_HISTORY_LENGTH] = Time::get_singleton()->get_ticks_usec();
	}
}
