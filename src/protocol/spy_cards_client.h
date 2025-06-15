#pragma once

#include "dry.h"

#include <godot_cpp/classes/http_request.hpp>

class SpyCardsClientRequest : public RefCounted {
	GDCLASS(SpyCardsClientRequest, RefCounted);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY_OBJECTID(HTTPRequest, request);

	DECLARE_PROPERTY(HTTPRequest::Result, result, = HTTPRequest::RESULT_NO_RESPONSE);
	DECLARE_PROPERTY(HTTPClient::ResponseCode, response_code, = static_cast<HTTPClient::ResponseCode>(0));
	DECLARE_PROPERTY(PackedStringArray, headers);
	DECLARE_PROPERTY(PackedByteArray, body);

private:
	void _on_request_completed(HTTPRequest::Result p_result, HTTPClient::ResponseCode p_response_code, const PackedStringArray &p_headers, const PackedByteArray &p_body);
	friend class SpyCardsClient;

public:
	void connect_request_completed(const Callable &p_callable, uint32_t p_flags = 0);
};

class SpyCardsClient : public Object {
	GDCLASS(SpyCardsClient, Object);

protected:
	static void _bind_methods();

public:
	static SpyCardsClient *get_singleton();

	DECLARE_PROPERTY(String, base_url);

	SpyCardsClient();

private:
	Ref<SpyCardsClientRequest> create_request(double p_timeout = 30.0) const;

	Ref<SpyCardsClientRequest> matchmaking_create_session(const String &p_command, int32_t p_max_players) const;
	Ref<SpyCardsClientRequest> matchmaking_join(const String &p_code) const;
	Ref<SpyCardsClientRequest> matchmaking_poll(const String &p_code, int32_t p_from, int32_t p_to, const String &p_verify) const;
	Ref<SpyCardsClientRequest> matchmaking_send(const String &p_code, int32_t p_from, int32_t p_to, const String &p_verify, const String &p_command) const;
	friend class SpyCardsLobbyConnection;

public:
	Ref<SpyCardsClientRequest> fetch_file_ipfs(const PackedByteArray &p_cid, ResourceLoader::CacheMode p_cache_mode = ResourceLoader::CACHE_MODE_REUSE) const;
	Ref<SpyCardsClientRequest> fetch_file(const PackedByteArray &p_file_id, ResourceLoader::CacheMode p_cache_mode = ResourceLoader::CACHE_MODE_REUSE) const;
	Ref<SpyCardsClientRequest> fetch_legacy_recording(const PackedByteArray &p_recording_id, ResourceLoader::CacheMode p_cache_mode = ResourceLoader::CACHE_MODE_REUSE) const;
};
