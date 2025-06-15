#pragma once

#include "dry.h"

#include "protocol/spy_cards_client.h"

#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/multiplayer_peer.hpp>
#include <godot_cpp/classes/web_rtc_peer_connection.hpp>

class SpyCardsLobbyConnection : public Node {
	GDCLASS(SpyCardsLobbyConnection, Node);

protected:
	static void _bind_methods();

private:
	String _lobby_id;
	String _verification;
	int32_t _player_index = 0;
	int32_t _max_players = 0;

	bool _waiting_for_configuration = false;
	bool _waiting_to_create = false;
	bool _waiting_to_join = false;

	bool _is_turn_available = false;
	bool _is_turn_forced = false;

	String _matchmaking_error;
	Ref<MultiplayerPeer> _multiplyer_peer;
	Vector<Ref<SpyCardsClientRequest>> _matchmaking_poll;
	Vector<Ref<SpyCardsClientRequest>> _matchmaking_send;
	Ref<SpyCardsClientRequest> _matchmaking_configuration;
	Ref<SpyCardsClientRequest> _matchmaking_init;
	Dictionary _rtc_config;
	constexpr static uint64_t PING_HISTORY_LENGTH = 64;
	struct PeerConnection {
		Ref<WebRTCPeerConnection> conn;
		PackedStringArray pending_matchmaking_commands;
		uint64_t ping_sent[PING_HISTORY_LENGTH];
		uint64_t ping_received[PING_HISTORY_LENGTH];
	};
	Vector<PeerConnection> _peers;

	double _ping_timer = 0.0;
	uint64_t _ping_sequence_number = 0;

public:
	SpyCardsLobbyConnection();

	static Dictionary make_rpc_config(MultiplayerAPI::RPCMode p_rpc_mode, MultiplayerPeer::TransferMode p_transfer_mode = MultiplayerPeer::TRANSFER_MODE_RELIABLE, bool p_call_local = false, int32_t p_channel = 0);

	void _physics_process(double p_delta) override;

	bool is_empty() const;
	void create_lobby(int32_t p_max_players);
	void join_lobby(const String &p_lobby_id);

	String get_lobby_id() const;

private:
	void _on_ice_candidate_created(const String &p_media, int32_t p_index, const String &p_name, int32_t p_peer);
	void _on_lobby_creator_session_description_created(const String &p_type, const String &p_sdp);
	void _on_offer_session_description_created(const String &p_type, const String &p_sdp, int32_t p_peer);
	void _on_answer_session_description_created(const String &p_type, const String &p_sdp, int32_t p_peer);
	void _on_fatal_error(const String &p_message, bool p_forwarded = false);
	void _on_join_lobby();
	void _start_poll(int32_t p_peer);
	void _start_send(int32_t p_peer, const String &p_command);
	bool _check_http_response(const String &p_request_name, const Ref<SpyCardsClientRequest> &p_request, HTTPClient::ResponseCode p_expected_response);
	void _on_init_config();
	void _on_init_create_session();
	void _on_init_join();
	void _on_send_completed(int32_t p_peer);
	void _on_poll_completed(int32_t p_peer);

public:
	void net_ping(uint64_t p_sequence_number);
	void net_pong(uint64_t p_sequence_number);
};
