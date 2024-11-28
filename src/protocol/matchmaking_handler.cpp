#include "protocol/matchmaking_handler.h"

#include "util/base32.h"
#include "util/why_isnt_this_in_godot.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/offline_multiplayer_peer.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/time.hpp>
#include <godot_cpp/classes/window.hpp>

// TODO: convert ERR_FAIL macros to send errors to the match

void MatchmakingHandler::_bind_methods() {
	BIND_CONSTANT(SHARED_SEED_LENGTH);

	BIND_ENUM_CONSTANT(INIT_WAIT);
	BIND_ENUM_CONSTANT(ABORTED);
	BIND_ENUM_CONSTANT(COSMETIC);
	BIND_ENUM_CONSTANT(DECK);
	BIND_ENUM_CONSTANT(START_WAIT);
	BIND_ENUM_CONSTANT(RESOLVE);
	BIND_ENUM_CONSTANT(CHOICE);
	BIND_ENUM_CONSTANT(REALTIME);
	BIND_ENUM_CONSTANT(REALTIME_WAIT);
	BIND_ENUM_CONSTANT(FINALIZE);
	BIND_ENUM_CONSTANT(COMPLETED);
	BIND_ENUM_CONSTANT(REMATCH_WAIT);
	BIND_ENUM_CONSTANT(REMATCH_ABORTED);

	BIND_PROPERTY_ENUM(MatchmakingHandler::MatchState, state);
	BIND_PROPERTY(Variant::DICTIONARY, ice_config);
	BIND_PROPERTY(Variant::STRING, lobby_id);
	BIND_PROPERTY(Variant::STRING, verification_code);
	BIND_PROPERTY(Variant::INT, max_players);

	BIND_PROPERTY_RESOURCE_NOT_SAVED(Crypto, crypto);
	BIND_PROPERTY_RESOURCE_NOT_SAVED(WebRTCMultiplayerPeer, peer);
	BIND_PROPERTY_RESOURCE_ARRAY(MatchmakingConnection, connections);
	BIND_PROPERTY_RESOURCE(DataContainer, recording);

	BIND_PROPERTY(Variant::PACKED_INT32_ARRAY, realtime_inputs);

	ClassDB::bind_method(D_METHOD("clear"), &MatchmakingHandler::clear);
	ClassDB::bind_method(D_METHOD("create_lobby", "game_mode_container", "selected_variant", "mode_public_name", "mode_public_revision"), &MatchmakingHandler::create_lobby, DEFVAL(""), DEFVAL(0));
	ClassDB::bind_method(D_METHOD("join_lobby", "lobby_id"), &MatchmakingHandler::join_lobby);

	ClassDB::bind_method(D_METHOD("find_remote_connection"), &MatchmakingHandler::find_remote_connection);

	ClassDB::bind_method(D_METHOD("ping", "i", "frames_behind"), &MatchmakingHandler::ping, DEFVAL(0));
	ClassDB::bind_method(D_METHOD("pong", "i"), &MatchmakingHandler::pong);
	ClassDB::bind_method(D_METHOD("fatal_error_encountered", "message"), &MatchmakingHandler::fatal_error_encountered);

	ClassDB::bind_method(D_METHOD("init_game_data", "game_mode_container_bytes", "selected_variant", "mode_public_name", "mode_public_revision", "match_start_timestamp", "shared_seed"), &MatchmakingHandler::init_game_data);
	ClassDB::bind_method(D_METHOD("notify_loaded_mode"), &MatchmakingHandler::notify_loaded_mode);
	ClassDB::bind_method(D_METHOD("set_player_cosmetic_data", "display_name", "character"), &MatchmakingHandler::set_player_cosmetic_data);
	ClassDB::bind_method(D_METHOD("set_player_initial_deck", "packed_deck"), &MatchmakingHandler::set_player_initial_deck);

	ClassDB::bind_method(D_METHOD("set_local_player_cosmetic_data", "display_name", "character"), &MatchmakingHandler::set_local_player_cosmetic_data);
	ClassDB::bind_method(D_METHOD("set_local_player_initial_deck", "deck"), &MatchmakingHandler::set_local_player_initial_deck);

	ClassDB::bind_method(D_METHOD("state_advance", "state_checksum", "next_random_seed"), &MatchmakingHandler::state_advance);
	ClassDB::bind_method(D_METHOD("choices_preview", "picked_cards"), &MatchmakingHandler::choices_preview);
	ClassDB::bind_method(D_METHOD("choices_confirmed", "picked_cards"), &MatchmakingHandler::choices_confirmed);
	ClassDB::bind_method(D_METHOD("request_repick"), &MatchmakingHandler::request_repick);
	ClassDB::bind_method(D_METHOD("acknowledge_repick"), &MatchmakingHandler::acknowledge_repick);
	ClassDB::bind_method(D_METHOD("reject_repick"), &MatchmakingHandler::reject_repick);
	ClassDB::bind_method(D_METHOD("realtime_update", "acknowledge_frame", "starting_frame", "packed_inputs"), &MatchmakingHandler::realtime_update);

	ClassDB::add_signal("MatchmakingHandler", MethodInfo("lobby_created", PropertyInfo(Variant::STRING, "lobby_id")));
}

static Dictionary make_rpc_config(MultiplayerAPI::RPCMode rpc_mode, MultiplayerPeer::TransferMode transfer_mode, bool call_local = false, int32_t channel = 0) {
	Dictionary config;

	config["rpc_mode"] = rpc_mode;
	config["transfer_mode"] = transfer_mode;
	config["call_local"] = call_local;
	config["channel"] = channel;

	return config;
}

MatchmakingHandler::MatchmakingHandler() {
	Dictionary ice_server;
	ice_server["urls"] = Array::make("stun:stun.cloudflare.com:3478");

	_ice_config["iceServers"] = Array::make(ice_server);

	_crypto.instantiate();
	_peer.instantiate();

	rpc_config("ping", make_rpc_config(MultiplayerAPI::RPC_MODE_ANY_PEER, MultiplayerPeer::TRANSFER_MODE_UNRELIABLE));
	rpc_config("pong", make_rpc_config(MultiplayerAPI::RPC_MODE_ANY_PEER, MultiplayerPeer::TRANSFER_MODE_UNRELIABLE));
	rpc_config("fatal_error_encountered", make_rpc_config(MultiplayerAPI::RPC_MODE_ANY_PEER, MultiplayerPeer::TRANSFER_MODE_RELIABLE, true));

	rpc_config("init_game_data", make_rpc_config(MultiplayerAPI::RPC_MODE_AUTHORITY, MultiplayerPeer::TRANSFER_MODE_RELIABLE));
	rpc_config("notify_loaded_mode", make_rpc_config(MultiplayerAPI::RPC_MODE_ANY_PEER, MultiplayerPeer::TRANSFER_MODE_RELIABLE));
	rpc_config("set_player_cosmetic_data", make_rpc_config(MultiplayerAPI::RPC_MODE_ANY_PEER, MultiplayerPeer::TRANSFER_MODE_RELIABLE));
	rpc_config("set_player_initial_deck", make_rpc_config(MultiplayerAPI::RPC_MODE_ANY_PEER, MultiplayerPeer::TRANSFER_MODE_RELIABLE));

	rpc_config("state_advance", make_rpc_config(MultiplayerAPI::RPC_MODE_ANY_PEER, MultiplayerPeer::TRANSFER_MODE_RELIABLE, true));
	rpc_config("choices_preview", make_rpc_config(MultiplayerAPI::RPC_MODE_ANY_PEER, MultiplayerPeer::TRANSFER_MODE_UNRELIABLE_ORDERED, true));
	rpc_config("choices_confirmed", make_rpc_config(MultiplayerAPI::RPC_MODE_ANY_PEER, MultiplayerPeer::TRANSFER_MODE_RELIABLE, true));
	rpc_config("request_repick", make_rpc_config(MultiplayerAPI::RPC_MODE_ANY_PEER, MultiplayerPeer::TRANSFER_MODE_RELIABLE));
	rpc_config("acknowledge_repick", make_rpc_config(MultiplayerAPI::RPC_MODE_ANY_PEER, MultiplayerPeer::TRANSFER_MODE_RELIABLE));
	rpc_config("reject_repick", make_rpc_config(MultiplayerAPI::RPC_MODE_ANY_PEER, MultiplayerPeer::TRANSFER_MODE_RELIABLE));
	rpc_config("realtime_update", make_rpc_config(MultiplayerAPI::RPC_MODE_ANY_PEER, MultiplayerPeer::TRANSFER_MODE_UNRELIABLE));

	SceneTree *tree = Object::cast_to<SceneTree>(Engine::get_singleton()->get_main_loop());
	if (likely(tree)) {
		tree->get_root()->add_child(this, true);
	}
}

IMPLEMENT_PROPERTY_SIMPLE(MatchmakingHandler, MatchmakingHandler::MatchState, state);
IMPLEMENT_PROPERTY_SIMPLE(MatchmakingHandler, Dictionary, ice_config);
IMPLEMENT_PROPERTY_SIMPLE(MatchmakingHandler, String, lobby_id);
IMPLEMENT_PROPERTY_SIMPLE(MatchmakingHandler, String, verification_code);
IMPLEMENT_PROPERTY_SIMPLE(MatchmakingHandler, int32_t, max_players);

IMPLEMENT_PROPERTY_SIMPLE(MatchmakingHandler, Ref<Crypto>, crypto);
IMPLEMENT_PROPERTY_SIMPLE(MatchmakingHandler, Ref<WebRTCMultiplayerPeer>, peer);
IMPLEMENT_PROPERTY_SIMPLE(MatchmakingHandler, TypedArray<MatchmakingConnection>, connections);
IMPLEMENT_PROPERTY_SIMPLE(MatchmakingHandler, Ref<DataContainer>, recording);

IMPLEMENT_PROPERTY_SIMPLE(MatchmakingHandler, PackedInt32Array, realtime_inputs);

void MatchmakingHandler::clear() {
	_state = INIT_WAIT;
	_lobby_id = "";
	_verification_code = "";

	for (int64_t i = 0; i < _connections.size(); i++) {
		MatchmakingConnection *conn = Object::cast_to<MatchmakingConnection>(_connections[i]);
		_peer->remove_peer(conn->get_remote_id());
		conn->queue_free();
	}
	_connections.clear();
	_peer->close();
	_recording = Ref<DataContainer>();
	_game_mode_container_serialized = PackedByteArray();
	_deck_ready.clear();
}
void MatchmakingHandler::create_lobby(const Ref<DataContainer> &game_mode_container, int64_t selected_variant, const String &mode_public_name, int64_t mode_public_revision) {
	ERR_FAIL_COND(game_mode_container.is_null());
	ERR_FAIL_COND(game_mode_container->get_container_type() != DataContainer::GAME_MODE);
	TypedArray<VariantDef> variants = game_mode_container->get_mode()->get_variants();
	ERR_FAIL_INDEX(selected_variant, variants.size());
	Ref<VariantDef> variant = variants[selected_variant];
	int32_t max_players = variant->get_player_count();
	ERR_FAIL_COND(max_players < 1);

	clear();

	_max_players = max_players;
	_deck_ready.resize(max_players);
	_game_mode_container_serialized = game_mode_container->to_byte_array("game mode for networking");

	_recording.instantiate();
	_recording->set_container_type(DataContainer::RECORDING);
	_recording->set_game_version(WhyIsntThisInGodot::get_project_version_vector());

	_recording->set_timestamp(uint64_t(Math::floor(Time::get_singleton()->get_unix_time_from_system() * 1000.0)));
	_recording->set_data_files(game_mode_container->get_data_files());

	_recording->set_mode(game_mode_container->get_mode());
	_recording->set_mode_public_name(mode_public_name);
	_recording->set_mode_public_revision(mode_public_revision);

	_recording->set_selected_variant(selected_variant);
	TypedArray<RecordingPlayerData> player_data; // preallocate array of nulls
	player_data.resize(max_players);
	_recording->set_player_data(player_data);
	_recording->set_shared_seed(_crypto->generate_random_bytes(SHARED_SEED_LENGTH));

	// TODO: run cosmetic init
	// TODO: run npc inits

	// TODO: ui
	_state = COSMETIC;

	if (max_players == 1) {
		// special case: singleplayer modes get a fake lobby id and a fake connection

		String lobby_id = "local_" + Base32::encode_crockford(_crypto->generate_random_bytes(16));
		_lobby_id = lobby_id;
		set_name("MatchmakingHandler_" + lobby_id);

		get_tree()->set_multiplayer(MultiplayerAPI::create_default_interface(), get_path());
		get_multiplayer()->set_multiplayer_peer(memnew(OfflineMultiplayerPeer));

		emit_signal("lobby_created", lobby_id);

		return;
	}

	_peer->create_mesh(1);

	MatchmakingConnection *conn = memnew(MatchmakingConnection(this, 2));
	_connections.append(conn);
	add_child(conn);
}
void MatchmakingHandler::join_lobby(const String &lobby_id) {
	clear();
	_lobby_id = lobby_id;
	set_name("MatchmakingHandler_" + lobby_id);

	// each match is an isolated multiplayer environment
	get_tree()->set_multiplayer(MultiplayerAPI::create_default_interface(), get_path());

	// using OfflineMultiplayerPeer as a signal that we are not yet initialized
	get_multiplayer()->set_multiplayer_peer(memnew(OfflineMultiplayerPeer));

	MatchmakingConnection *conn = memnew(MatchmakingConnection(this, 1));
	_connections.append(conn);
	add_child(conn);
}

MatchmakingConnection *MatchmakingHandler::find_remote_connection() const {
	int32_t remote_id = get_multiplayer()->get_remote_sender_id();
	for (int64_t i = 0; i < _connections.size(); i++) {
		MatchmakingConnection *conn = Object::cast_to<MatchmakingConnection>(_connections[i]);
		if (conn->get_remote_id() == remote_id) {
			return conn;
		}
	}

	return nullptr;
}

void MatchmakingHandler::ping(int64_t i, int32_t frames_behind) {
	MatchmakingConnection *conn = find_remote_connection();
	ERR_FAIL_NULL(conn);

	conn->_ping(i, frames_behind);
	rpc_id(conn->get_remote_id(), "pong", i);
}
void MatchmakingHandler::pong(int64_t i) {
	MatchmakingConnection *conn = find_remote_connection();
	ERR_FAIL_NULL(conn);

	conn->_pong(i);
}
void MatchmakingHandler::fatal_error_encountered(const String &message) {
	MatchmakingConnection *conn = find_remote_connection();
	ERR_FAIL_NULL(conn);

	WARN_PRINT(vformat("TODO: received notification of fatal error from player %d: \"%s\"", conn->get_remote_id(), message)); // TODO
}

void MatchmakingHandler::init_game_data(const PackedByteArray &game_mode_container_bytes, int64_t selected_variant, const String &mode_public_name, int64_t mode_public_revision, uint64_t match_start_timestamp, const PackedByteArray &shared_seed) {
	ERR_FAIL_COND_MSG(_recording.is_valid(), "received init_game_data packet while recording is already active");

	Ref<DataContainer> game_mode_container = DataContainer::from_byte_array(game_mode_container_bytes, "networked game mode");
	ERR_FAIL_COND(game_mode_container.is_null());
	ERR_FAIL_COND(game_mode_container->get_container_type() != DataContainer::GAME_MODE);
	TypedArray<VariantDef> variants = game_mode_container->get_mode()->get_variants();
	ERR_FAIL_INDEX(selected_variant, variants.size());
	Ref<VariantDef> variant = variants[selected_variant];
	int32_t max_players = variant->get_player_count();
	ERR_FAIL_COND(max_players != _max_players);
	ERR_FAIL_COND(max_players != _connections.size() + 1);
	_deck_ready.resize(max_players);

	_recording.instantiate();
	_recording->set_container_type(DataContainer::RECORDING);
	_recording->set_game_version(WhyIsntThisInGodot::get_project_version_vector());

	_recording->set_timestamp(match_start_timestamp);
	_recording->set_data_files(game_mode_container->get_data_files());

	_recording->set_mode(game_mode_container->get_mode());
	_recording->set_mode_public_name(mode_public_name);
	_recording->set_mode_public_revision(mode_public_revision);

	_recording->set_selected_variant(selected_variant);
	TypedArray<RecordingPlayerData> player_data; // preallocate array of nulls
	player_data.resize(max_players);
	_recording->set_player_data(player_data);
	_recording->set_shared_seed(shared_seed);

	// TODO: run cosmetic init
	// TODO: run npc inits

	for (int64_t i = 0; i < _connections.size(); i++) {
		MatchmakingConnection *conn = Object::cast_to<MatchmakingConnection>(_connections[i]);
		if (conn->get_was_fully_connected()) {
			rpc_id(conn->get_remote_id(), "notify_loaded_mode");
		} else {
			conn->connect("fully_connected", callable_mp(this, &MatchmakingHandler::_do_notify_loaded_mode).bind(conn), CONNECT_ONE_SHOT);
		}
	}

	// TODO: ui
	_state = COSMETIC;
}
void MatchmakingHandler::_do_notify_loaded_mode(MatchmakingConnection *conn) {
	rpc_id(conn->get_remote_id(), "notify_loaded_mode");
}
void MatchmakingHandler::notify_loaded_mode() {
	MatchmakingConnection *conn = find_remote_connection();
	ERR_FAIL_NULL(conn);

	ERR_FAIL_COND_MSG(conn->get_loaded_mode(), vformat("duplicate notify_loaded_mode from player %d", conn->get_remote_id()));
	conn->set_loaded_mode(true);

	if (_recording.is_null()) {
		// we haven't loaded the mode; just record that they have for now.

		return;
	}

	Ref<RecordingPlayerData> player_data = _recording->get_player_data()[get_multiplayer()->get_unique_id() - 1];
	if (player_data.is_null()) {
		// we're not ready to send yet; just record that they're ready to receive for now.

		return;
	}

	// send local player data
	rpc_id(conn->get_remote_id(), "set_player_cosmetic_data", player_data->get_display_name(), player_data->get_character());

	// send deck if we have it
	if (_deck_ready[get_multiplayer()->get_unique_id() - 1]) {
		PackedInt64Array packed_deck = player_data->get_initial_deck();
		rpc_id(conn->get_remote_id(), "set_player_initial_deck", packed_deck);
	}
}
void MatchmakingHandler::set_player_cosmetic_data(const String &display_name, enums::CharacterDef::Character character) {
	MatchmakingConnection *conn = find_remote_connection();
	ERR_FAIL_NULL(conn);
	ERR_FAIL_COND_MSG(_recording.is_null(), vformat("received set_player_cosmetic_data from player %d before we were ready", conn->get_remote_id()));

	TypedArray<RecordingPlayerData> player_data = _recording->get_player_data();
	Ref<RecordingPlayerData> player = player_data[conn->get_remote_id() - 1];
	ERR_FAIL_COND_MSG(player.is_valid(), vformat("received set_player_cosmetic_data from player %d multiple times", conn->get_remote_id()));

	Ref<CharacterDef> character_def = _recording->get_mode()->get_character(character);
	ERR_FAIL_COND_MSG(character_def.is_null(), vformat("received invalid character %d in set_player_cosmetic_data from player %d", character, conn->get_remote_id()));
	ERR_FAIL_COND_MSG(character_def->is_hidden(), vformat("received hidden character %d (%s) in set_player_cosmetic_data from player %d", character, character_def->get_name(), conn->get_remote_id()));

	// TODO: validate arcade display name rules
	// TODO: validate non-arcade display name rules

	player.instantiate();
	player->set_display_name(display_name);
	player->set_character(character);

	player_data[conn->get_remote_id() - 1] = player;
	_recording->set_player_data(player_data);

	// TODO: jigsaw trigger
}
void MatchmakingHandler::set_player_initial_deck(const PackedInt64Array &packed_deck) {
	MatchmakingConnection *conn = find_remote_connection();
	ERR_FAIL_NULL(conn);
	ERR_FAIL_COND_MSG(_recording.is_null(), vformat("received set_player_initial_deck from player %d before we were ready", conn->get_remote_id()));

	TypedArray<RecordingPlayerData> player_data = _recording->get_player_data();
	Ref<RecordingPlayerData> player = player_data[conn->get_remote_id() - 1];
	ERR_FAIL_COND_MSG(player.is_null(), vformat("received set_player_initial_deck from player %d before set_player_cosmetic_data", conn->get_remote_id()));

	ERR_FAIL_COND_MSG(_deck_ready[conn->get_remote_id() - 1], vformat("received set_player_initial_deck from player %d multiple times", conn->get_remote_id()));
	_deck_ready.write[conn->get_remote_id() - 1] = true;

	// TODO: validate deck

	TypedArray<enums::CardDef::Card> deck;
	deck.append_array(packed_deck);
	player->set_initial_deck(deck);

	// TODO: jigsaw trigger

	if (!_deck_ready.has(false)) {
		_start_match();
	}
}

void MatchmakingHandler::set_local_player_cosmetic_data(const String &display_name, enums::CharacterDef::Character character) {
	ERR_FAIL_COND_MSG(_recording.is_null(), "set_local_player_cosmetic_data called before game mode was initialized");

	TypedArray<RecordingPlayerData> player_data = _recording->get_player_data();
	Ref<RecordingPlayerData> player = player_data[get_multiplayer()->get_unique_id() - 1];
	ERR_FAIL_COND_MSG(player.is_valid(), "set_local_player_cosmetic_data called multiple times");

	// assume name and character were valid because we just got them from our own UI (peers will validate)

	player.instantiate();
	player->set_display_name(display_name);
	player->set_character(character);

	player_data[get_multiplayer()->get_unique_id() - 1] = player;
	_recording->set_player_data(player_data);

	for (int64_t i = 0; i < _connections.size(); i++) {
		MatchmakingConnection *conn = Object::cast_to<MatchmakingConnection>(_connections[i]);
		if (conn->get_loaded_mode()) {
			rpc_id(conn->get_remote_id(), "set_player_cosmetic_data", display_name, character);
		}
	}

	// TODO: ui
	_state = DECK;
}
void MatchmakingHandler::set_local_player_initial_deck(const TypedArray<enums::CardDef::Card> &deck) {
	ERR_FAIL_COND_MSG(_recording.is_null(), "set_local_player_initial_deck called before game mode was initialized");

	TypedArray<RecordingPlayerData> player_data = _recording->get_player_data();
	Ref<RecordingPlayerData> player = player_data[get_multiplayer()->get_unique_id() - 1];
	ERR_FAIL_COND_MSG(player.is_null(), "set_local_player_initial_deck called before set_local_player_cosmetic_data");

	ERR_FAIL_COND_MSG(_deck_ready[get_multiplayer()->get_unique_id() - 1], "set_local_player_initial_deck called multiple times");
	_deck_ready.write[get_multiplayer()->get_unique_id() - 1] = true;

	// assume deck was valid because we just got it from our own UI (peers will validate)

	PackedInt64Array packed_deck = deck;
	player->set_initial_deck(deck.duplicate());

	for (int64_t i = 0; i < _connections.size(); i++) {
		MatchmakingConnection *conn = Object::cast_to<MatchmakingConnection>(_connections[i]);
		if (conn->get_loaded_mode()) {
			rpc_id(conn->get_remote_id(), "set_player_initial_deck", packed_deck);
		}
	}

	// TODO: ui
	_state = START_WAIT;

	if (!_deck_ready.has(false)) {
		_start_match();
	}
}

void MatchmakingHandler::state_advance(const PackedByteArray &state_checksum, const PackedByteArray &next_random_seed) {
	// TODO
}
void MatchmakingHandler::choices_preview(const PackedInt64Array &picked_cards) {
	// TODO
}
void MatchmakingHandler::choices_confirmed(const PackedInt64Array &picked_cards) {
	// TODO
}
void MatchmakingHandler::request_repick() {
	// TODO
}
void MatchmakingHandler::acknowledge_repick() {
	// TODO
}
void MatchmakingHandler::reject_repick() {
	// TODO
}
void MatchmakingHandler::realtime_update(int64_t acknowedge_frame, int64_t starting_frame, const PackedByteArray &packed_inputs) {
	// TODO
}

void MatchmakingHandler::_on_lobby_created(const String &lobby_id, const String &verification_code) {
	_lobby_id = lobby_id;
	set_name("MatchmakingHandler_" + lobby_id);

	get_tree()->set_multiplayer(MultiplayerAPI::create_default_interface(), get_path());
	get_multiplayer()->set_multiplayer_peer(_peer);

	_verification_code = verification_code;
	emit_signal("lobby_created", lobby_id);

	_create_remaining_connections();
}
void MatchmakingHandler::_on_player_id(int32_t player_id, const String &verification_code) {
	_peer->create_mesh(player_id);
	_verification_code = verification_code;

	get_multiplayer()->set_multiplayer_peer(_peer);

	for (int64_t i = 0; i < _connections.size(); i++) {
		MatchmakingConnection *conn = Object::cast_to<MatchmakingConnection>(_connections[i]);
		remove_child(conn);
		conn->request_ready();
		add_child(conn);
	}

	_create_remaining_connections();
}
void MatchmakingHandler::_create_remaining_connections() {
	ERR_FAIL_COND(_connections.size() != 1);

	int32_t local_player_id = get_multiplayer()->get_unique_id();
	for (int32_t i = 2; i <= _max_players; i++) {
		if (local_player_id == 1 && i == 2) {
			continue;
		}
		if (local_player_id == i) {
			continue;
		}

		MatchmakingConnection *conn = memnew(MatchmakingConnection(this, i));
		_connections.append(conn);
		add_child(conn);
	}
}
void MatchmakingHandler::_start_match() {
	// TODO
}
