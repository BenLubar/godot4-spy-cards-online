#ifndef MATCHMAKING_HANDLER_H
#define MATCHMAKING_HANDLER_H

#include "dry.h"

class MatchmakingHandler;

#include "protocol/data_container.h"
#include "protocol/matchmaking_connection.h"

#include <godot_cpp/classes/crypto.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/web_rtc_multiplayer_peer.hpp>

class MatchmakingHandler : public Node {
	GDCLASS(MatchmakingHandler, Node);

protected:
	static void _bind_methods();

public:
	static constexpr int32_t SHARED_SEED_LENGTH = 32;

	enum MatchState {
		INIT_WAIT = 0,
		ABORTED = 1,
		COSMETIC = 2,
		DECK = 3,
		START_WAIT = 4,
		RESOLVE = 5,
		CHOICE = 6,
		REALTIME = 7,
		REALTIME_WAIT = 8,
		FINALIZE = 9,
		COMPLETED = 10,
		REMATCH_WAIT = 11,
		REMATCH_ABORTED = 12,
	};

	MatchmakingHandler();

	DECLARE_PROPERTY(MatchState, state, = INIT_WAIT);
	DECLARE_PROPERTY(Dictionary, ice_config);
	DECLARE_PROPERTY(String, lobby_id);
	DECLARE_PROPERTY(String, verification_code);
	DECLARE_PROPERTY(int32_t, max_players, = 0);

	DECLARE_PROPERTY(Ref<Crypto>, crypto);
	DECLARE_PROPERTY(Ref<WebRTCMultiplayerPeer>, peer);
	DECLARE_PROPERTY(TypedArray<MatchmakingConnection>, connections);
	DECLARE_PROPERTY(Ref<DataContainer>, recording);

	DECLARE_PROPERTY(PackedByteArray, realtime_inputs);

private:
	// authority only
	PackedByteArray _game_mode_container_serialized;

	// all players
	Vector<bool> _deck_ready;

public:
	void clear();
	void create_lobby(const Ref<DataContainer> &game_mode_container, int64_t selected_variant, const String &mode_public_name = "", int64_t mode_public_revision = 0);
	void join_lobby(const String &lobby_id);

	MatchmakingConnection *find_remote_connection() const;

	void ping(int64_t i, int32_t frames_behind);
	void pong(int64_t i);
	void fatal_error_encountered(const String &message);

	void init_game_data(const PackedByteArray &game_mode_container_bytes, int64_t selected_variant, const String &mode_public_name, int64_t mode_public_revision, uint64_t match_start_timestamp, const PackedByteArray &shared_seed);
	void _do_notify_loaded_mode(MatchmakingConnection *conn);
	void notify_loaded_mode();
	void set_player_cosmetic_data(const String &display_name, enums::CharacterDef::Character character);
	void set_player_initial_deck(const PackedInt64Array &packed_deck);

	void set_local_player_cosmetic_data(const String &display_name, enums::CharacterDef::Character character);
	void set_local_player_initial_deck(const TypedArray<enums::CardDef::Card> &deck);

	void state_advance(const PackedByteArray &state_checksum, const PackedByteArray &next_random_seed);
	void choices_preview(const PackedInt64Array &picked_cards);
	void choices_confirmed(const PackedInt64Array &picked_cards);
	void request_repick();
	void acknowledge_repick();
	void reject_repick();
	void realtime_update(int64_t acknowledge_frame, int64_t starting_frame, const PackedByteArray &packed_inputs);

	void _on_lobby_created(const String &lobby_id, const String &verification_code);
	void _on_player_id(int32_t player_id, const String &verification_code);
	void _create_remaining_connections();
	void _start_match();
};
DECLARE_ENUM(MatchmakingHandler::MatchState);

#endif // MATCHMAKING_HANDLER_H
