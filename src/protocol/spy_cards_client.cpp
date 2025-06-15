#include "protocol/spy_cards_client.h"

#include "util/base32.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>

const static LazyStringName REQUEST_COMPLETED{"request_completed"};

void SpyCardsClientRequest::_bind_methods() {
	BIND_PROPERTY_OBJECTID_NODE_NOT_SAVED(HTTPRequest, request);

	BIND_PROPERTY_ENUM(HTTPRequest::Result, result);
	BIND_PROPERTY_ENUM(HTTPClient::ResponseCode, response_code);
	BIND_PROPERTY(Variant::PACKED_STRING_ARRAY, headers);
	BIND_PROPERTY(Variant::PACKED_BYTE_ARRAY, body);

	ADD_SIGNAL(MethodInfo(REQUEST_COMPLETED));
}

IMPLEMENT_PROPERTY_OBJECTID_SIMPLE(SpyCardsClientRequest, HTTPRequest, request);

IMPLEMENT_PROPERTY_SIMPLE(SpyCardsClientRequest, HTTPRequest::Result, result);
IMPLEMENT_PROPERTY_SIMPLE(SpyCardsClientRequest, HTTPClient::ResponseCode, response_code);
IMPLEMENT_PROPERTY_SIMPLE(SpyCardsClientRequest, PackedStringArray, headers);
IMPLEMENT_PROPERTY_SIMPLE(SpyCardsClientRequest, PackedByteArray, body);

void SpyCardsClientRequest::_on_request_completed(HTTPRequest::Result p_result, HTTPClient::ResponseCode p_response_code, const PackedStringArray &p_headers, const PackedByteArray &p_body) {
	HTTPRequest *request = get_request();
	if (likely(request)) {
		request->queue_free();
		set_request(nullptr);
	}

	set_result(p_result);
	set_response_code(p_response_code);
	set_headers(p_headers);
	set_body(p_body);

	emit_signal(REQUEST_COMPLETED);
}

void SpyCardsClientRequest::connect_request_completed(const Callable &p_callable, uint32_t p_flags) {
	connect(REQUEST_COMPLETED, p_callable, p_flags);
}

void SpyCardsClient::_bind_methods() {
	Dictionary base_url_info;
	base_url_info["name"] = "spy_cards_online/base_url";
	base_url_info["type"] = Variant::STRING;
	ProjectSettings::get_singleton()->add_property_info(base_url_info);

	BIND_PROPERTY(Variant::STRING, base_url);
}

SpyCardsClient *SpyCardsClient::get_singleton() {
	static SpyCardsClient * const singleton = Object::cast_to<SpyCardsClient>(Engine::get_singleton()->get_singleton(get_class_static()));
	return singleton;
}

SpyCardsClient::SpyCardsClient() {
	_base_url = ProjectSettings::get_singleton()->get_setting_with_override("spy_cards_online/base_url");
}

IMPLEMENT_PROPERTY_SIMPLE(SpyCardsClient, String, base_url);

Ref<SpyCardsClientRequest> SpyCardsClient::create_request(double p_timeout) const {
	MainLoop *main_loop = Engine::get_singleton()->get_main_loop();
	SceneTree *tree = Object::cast_to<SceneTree>(main_loop);

	HTTPRequest *request = memnew(HTTPRequest);
	request->set_timeout(p_timeout);
	tree->get_root()->add_child(request);

	Ref<SpyCardsClientRequest> wrapper;
	wrapper.instantiate();

	wrapper->set_request(request);
	request->set_meta(REQUEST_COMPLETED, wrapper); // keep a reference alive so the node can be freed by the callback
	request->connect(REQUEST_COMPLETED, callable_mp(*wrapper, &SpyCardsClientRequest::_on_request_completed));

	return wrapper;
}

Ref<SpyCardsClientRequest> SpyCardsClient::matchmaking_create_session(const String &p_command, int32_t p_max_players) const {
	const Ref<SpyCardsClientRequest> request = create_request();

	const String url = _base_url + String("/spy-cards/matchmaking/create-session");
	const String body = vformat("cmd=%s&maxplayers=%d", p_command.uri_encode(), p_max_players);
	request->get_request()->request(url, {{"Content-Type: application/x-www-form-urlencoded"}}, HTTPClient::METHOD_POST, body);

	return request;
}

Ref<SpyCardsClientRequest> SpyCardsClient::matchmaking_join(const String &p_code) const {
	const Ref<SpyCardsClientRequest> request = create_request();

	const String url = _base_url + String("/spy-cards/matchmaking/join");
	const String body = vformat("code=%s", p_code.uri_encode());
	request->get_request()->request(url, {{"Content-Type: application/x-www-form-urlencoded"}}, HTTPClient::METHOD_POST, body);

	return request;
}

Ref<SpyCardsClientRequest> SpyCardsClient::matchmaking_poll(const String &p_code, int32_t p_from, int32_t p_to, const String &p_verify) const {
	const Ref<SpyCardsClientRequest> request = create_request();

	const String url = _base_url + String("/spy-cards/matchmaking/poll");
	const String body = vformat("code=%s&from=%d&to=%d&verify=%s", p_code.uri_encode(), p_from, p_to, p_verify.uri_encode());
	request->get_request()->request(url, {{"Content-Type: application/x-www-form-urlencoded"}}, HTTPClient::METHOD_POST, body);

	return request;
}

Ref<SpyCardsClientRequest> SpyCardsClient::matchmaking_send(const String &p_code, int32_t p_from, int32_t p_to, const String &p_verify, const String &p_command) const {
	const Ref<SpyCardsClientRequest> request = create_request();

	const String url = _base_url + String("/spy-cards/matchmaking/send");
	const String body = vformat("code=%s&from=%d&to=%d&verify=%s&cmd=%s", p_code.uri_encode(), p_from, p_to, p_verify.uri_encode(), p_command.uri_encode());
	request->get_request()->request(url, {{"Content-Type: application/x-www-form-urlencoded"}}, HTTPClient::METHOD_POST, body);

	return request;
}

Ref<SpyCardsClientRequest> SpyCardsClient::matchmaking_get_configuration() const {
	const Ref<SpyCardsClientRequest> request = create_request();

	const String url = _base_url + String("/spy-cards/matchmaking/get-configuration");
	request->get_request()->request(url);

	return request;
}

Ref<SpyCardsClientRequest> SpyCardsClient::fetch_file_ipfs(const PackedByteArray &p_cid, ResourceLoader::CacheMode p_cache_mode) const {
	// TODO: p_cache_mode

	const Ref<SpyCardsClientRequest> request = create_request();

	const String cid_string = Base32::encode_cid(p_cid);
	const String url = vformat("%s/ipfs/%s", _base_url, cid_string);

	if (p_cid.size() != 36 || p_cid[0] != 0x01 || p_cid[1] != 0x70 || p_cid[2] != 0x12 || p_cid[3] != 0x20) {
		WARN_PRINT(vformat("Requesting invalid legacy (IPFS) file ID %s", cid_string));
	}

	request->get_request()->request(url, {{"Accept: audio/ogg"}});

	return request;
}

Ref<SpyCardsClientRequest> SpyCardsClient::fetch_file(const PackedByteArray &p_file_id, ResourceLoader::CacheMode p_cache_mode) const {
	// TODO: p_cache_mode

	const Ref<SpyCardsClientRequest> request = create_request();

	const String file_id_string = Base32::encode_crockford(p_file_id);
	const String url = vformat("%s/spy-cards/godot-fileapi/%s", _base_url, file_id_string);
	PackedStringArray headers;

	const Pair<uint64_t, int64_t> opaque_id = uvarint(p_file_id, 8);
	if (opaque_id.second > 0) {
		if (8 + opaque_id.second == p_file_id.size()) {
			headers.append("Accept: image/png");
		} else {
			const Pair<uint64_t, int64_t> file_type = uvarint(p_file_id, 8 + opaque_id.second);
			if (file_type.second > 0 && 8 + opaque_id.second + file_type.second == p_file_id.size()) {
				switch (file_type.first) {
				case 1:
					headers.append("Accept: audio/ogg; codecs=opus");
					break;
				case 2:
					headers.append("Accept: model/gltf-binary");
					break;
				case 3:
					headers.append("Accept: application/vnd.spycards.datacontainer");
					break;
				case 4:
				case 5:
				case 6:
				case 7:
					WARN_PRINT(vformat("Requesting local file ID %s", file_id_string));
					break;
				default:
					WARN_PRINT(vformat("Requesting invalid file ID %s", file_id_string));
					break;
				}
			} else {
				WARN_PRINT(vformat("Requesting invalid file ID %s", file_id_string));
			}
		}
	} else {
		WARN_PRINT(vformat("Requesting invalid file ID %s", file_id_string));
	}

	request->get_request()->request(url, headers);

	return request;
}

Ref<SpyCardsClientRequest> SpyCardsClient::fetch_legacy_recording(const PackedByteArray &p_recording_id, ResourceLoader::CacheMode p_cache_mode) const {
	// TODO: p_cache_mode

	const Ref<SpyCardsClientRequest> request = create_request();

	const String recording_id_string = Base32::encode_crockford(p_recording_id);
	const String url = vformat("%s/spy-cards/godot-api/legacy-recording/%s", _base_url, recording_id_string);

	const Pair<uint64_t, int64_t> opaque_id = uvarint(p_recording_id, 8);
	if (opaque_id.second <= 0 || 8 + opaque_id.second != p_recording_id.size()) {
		WARN_PRINT(vformat("Requesting invalid legacy recording file ID %s", recording_id_string));
	}

	request->get_request()->request(url, {{"Accept: application/vnd.spycards.matchdata, application/vnd.spycards.arcadedata"}});

	return request;
}

/*
TODO:

GET /spy-cards/oh_yeah_name_every_spy_card.zip
->
200 OK (application/zip) (warning! this is like a third of a gigabyte)

GET /spy-cards/godot-api/status (same origin only)
->
200 OK (application/json)
{"logged-in":true,"user-name":"ben"}
*/
