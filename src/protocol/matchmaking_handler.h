#ifndef MATCHMAKING_HANDLER_H
#define MATCHMAKING_HANDLER_H

#include "dry.h"

class MatchmakingHandler;

#include "protocol/data_container.h"
#include "protocol/matchmaking_connection.h"

#include "jigsaw/jigsaw_input_source.h"
#include "jigsaw/jigsaw_state.h"

#include <godot_cpp/classes/crypto.hpp>
#include <godot_cpp/classes/web_rtc_multiplayer_peer.hpp>

class MatchmakingHandler : public JigsawInputSource {
	GDCLASS(MatchmakingHandler, JigsawInputSource);

protected:
	static void _bind_methods();

public:
	static constexpr int32_t SHARED_SEED_LENGTH = 32;

	enum MatchState {
		INIT_WAIT,
		ABORTED,
		ASSETS,
		CONSENT,
		COSMETIC,
		DECK,
		START_WAIT,
		RESOLVE,
		CHOICE,
		REALTIME,
		REALTIME_WAIT,
		FINALIZE,
		COMPLETED,
		REMATCH_WAIT,
		REMATCH_ABORTED,
	};

	MatchmakingHandler();

	DECLARE_PROPERTY(MatchState, state, = INIT_WAIT);
	DECLARE_PROPERTY(Dictionary, ice_config);
	DECLARE_PROPERTY(String, lobby_id);
	DECLARE_PROPERTY(String, verification_code);
	DECLARE_PROPERTY(String, display_name);
	DECLARE_PROPERTY(String, save_file_path);
	DECLARE_PROPERTY(int32_t, max_players, = 0);

	DECLARE_PROPERTY(Ref<Crypto>, crypto);
	DECLARE_PROPERTY(Ref<WebRTCMultiplayerPeer>, peer);
	DECLARE_PROPERTY(TypedArray<MatchmakingConnection>, connections);
	DECLARE_PROPERTY(Ref<DataContainer>, recording);
	DECLARE_PROPERTY(JigsawGlobal *, global, = nullptr);

	DECLARE_PROPERTY(PackedInt32Array, realtime_inputs);
	DECLARE_PROPERTY(bool, need_rollback, = false);
	DECLARE_PROPERTY(Ref<JigsawState>, base_state);
	DECLARE_PROPERTY(int64_t, current_frame, = -1);
	DECLARE_PROPERTY(int64_t, base_frame, = -1);

	DECLARE_PROPERTY(bool, handled_fatal_error, = false);

private:
	// authority only
	PackedByteArray _game_mode_container_serialized;

public:
	static MatchmakingHandler *create_lobby(const Ref<DataContainer> &game_mode_container, int64_t selected_variant, const String &mode_public_name = "", int64_t mode_public_revision = 0);
	static MatchmakingHandler *join_lobby(const String &lobby_id);

	MatchmakingConnection *find_remote_connection() const;
	MatchmakingConnection *find_remote_connection(int32_t remote_id) const;

	void ping(int64_t i, int32_t frames_behind);
	void pong(int64_t i);
	void fatal_error_encountered(const String &message);

	void init_game_data(const PackedByteArray &game_mode_container_bytes, int64_t selected_variant, const String &mode_public_name, int64_t mode_public_revision, uint64_t match_start_timestamp, const PackedByteArray &shared_seed);
	void on_consent(const String &display_name, const String &save_file_path);
	void _on_game_mode_assets_loaded();
	void _do_notify_loaded_mode(MatchmakingConnection *conn);
	void notify_loaded_mode();
	void set_player_cosmetic_data(const String &display_name, enums::CharacterDef::Character character);
	void set_player_initial_deck(const PackedArray<enums::CardDef::Card> &packed_deck);

	void set_local_player_cosmetic_data(enums::CharacterDef::Character character);
	void set_local_player_initial_deck(const PackedArray<enums::CardDef::Card> &deck);

	void state_advance(const PackedByteArray &state_checksum, const PackedByteArray &next_random_seed);
	void choices_preview(const PackedInt32Array &picked_cards);
	void choices_confirmed(const PackedInt32Array &picked_cards);
	void request_repick();
	void acknowledge_repick();
	void reject_repick();
	void realtime_update(int64_t acknowledge_frame, int64_t starting_frame, const PackedByteArray &packed_inputs);

	void _on_lobby_created(const String &lobby_id, const String &verification_code);
	void _on_player_id(int32_t player_id, const String &verification_code);
	void _create_remaining_connections();
	void _bind_init_game_data(MatchmakingConnection *conn);
	void _send_init_game_data(int64_t remote_id);
	void _check_start_match();
	void _start_match();
	void _on_connection_encountered_fatal_error(const String &message, MatchmakingConnection *conn);

	BitField<ButtonInputHistory::InputButton> get_player_realtime_inputs(int32_t side, int64_t frame) const override;
	void update_player_realtime_inputs(int64_t frame) override;
	void on_jigsaw_error(const Ref<JigsawError> &err) override;
};
DECLARE_ENUM(MatchmakingHandler::MatchState);

#endif // MATCHMAKING_HANDLER_H
