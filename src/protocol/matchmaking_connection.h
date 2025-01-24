#ifndef MATCHMAKING_CONNECTION_H
#define MATCHMAKING_CONNECTION_H

#include "dry.h"

class MatchmakingConnection;

#include <godot_cpp/classes/http_client.hpp>
#include <godot_cpp/classes/http_request.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/web_rtc_peer_connection.hpp>

class MatchmakingHandler;

class MatchmakingConnection : public Node {
	GDCLASS(MatchmakingConnection, Node);

protected:
	static void _bind_methods();

public:
	static constexpr int64_t PING_EVERY = 60;
	static constexpr int64_t PING_COUNT = 15;

	DECLARE_PROPERTY(MatchmakingHandler *, handler, = nullptr);
	DECLARE_PROPERTY(int32_t, remote_id, = 0);
	DECLARE_PROPERTY(Ref<WebRTCPeerConnection>, conn);
	DECLARE_PROPERTY(PackedInt64Array, ping_times);
	DECLARE_PROPERTY(int64_t, sent_commands, = 0);
	DECLARE_PROPERTY(int64_t, recv_commands, = 0);
	DECLARE_PROPERTY(PackedInt64Array, ping_start_times);
	DECLARE_PROPERTY(int64_t, ping_id, = 0);
	DECLARE_PROPERTY(int64_t, highest_remote_ping_id, = 0);
	DECLARE_PROPERTY(HTTPRequest *, sender, = nullptr);
	DECLARE_PROPERTY(HTTPRequest *, poller, = nullptr);
	DECLARE_PROPERTY(bool, sender_active, = false);
	DECLARE_PROPERTY(String, send_queue);
	DECLARE_PROPERTY(int64_t, since_last_ping, = 0);
	DECLARE_PROPERTY(String, fatal_error);

	DECLARE_PROPERTY(bool, was_fully_connected, = false);
	DECLARE_PROPERTY(bool, loaded_mode, = false);
	DECLARE_PROPERTY_IS(bool, deck_ready, = false);

	DECLARE_PROPERTY(int64_t, frame_ack, = 0);
	DECLARE_PROPERTY(int32_t, local_frame_advantage, = 0);
	DECLARE_PROPERTY(int32_t, prev_local_frame_advantage, = 0);
	DECLARE_PROPERTY(int32_t, remote_frame_advantage, = 0);
	DECLARE_PROPERTY(int32_t, prev_remote_frame_advantage, = 0);
	DECLARE_PROPERTY(int32_t, suggested_drop_inputs, = 0);
	DECLARE_PROPERTY(PackedInt32Array, realtime_inputs);

	MatchmakingConnection() = default; // trivial constructor for documentation generator
	MatchmakingConnection(MatchmakingHandler *handler, int32_t remote_id);

	void _ready() override;
	void _physics_process(double delta) override;
	void restart();
	void _start_ping();
	void _ping(int64_t i, int32_t frames_behind);
	void _pong(int64_t i);
	double get_average_ping() const;

	void _on_session_description_created(const String &type, const String &sdp);
	void _on_ice_candidate_created(const String &media, int64_t index, const String &candidate);
	void _on_command_received(const String &command, const PackedStringArray &args);
	void _on_sender_request_completed(HTTPRequest::Result result, HTTPClient::ResponseCode response_code, const PackedStringArray &headers, const PackedByteArray &body);
	void _on_poller_request_completed(HTTPRequest::Result result, HTTPClient::ResponseCode response_code, const PackedStringArray &headers, const PackedByteArray &body);
	void _send_command(const String &command, const PackedStringArray &args);
	String _post_params() const;
	void _start_send_request();
	void _start_poll_request();
	void _on_fatal_error(const String &text);
};

#endif // MATCHMAKING_CONNECTION_H
