#include "jigsaw/parameter/jigsaw_parameter_scene.h"

#include "util/player_preferences_helper.h"

#include <godot_cpp/classes/gltf_document.hpp>
#include <godot_cpp/classes/json.hpp>
#include <godot_cpp/classes/light3d.hpp>
#include <godot_cpp/classes/marshalls.hpp>

void JigsawParameterScene::_bind_methods() {
	BIND_PROPERTY_RESOURCE_NOT_SAVED(GLTFState, scene);
	BIND_PROPERTY_RESOURCE_NOT_SAVED(AnimationRootNode, animation);
	BIND_PROPERTY(Variant::DICTIONARY, json_data);

	ClassDB::bind_method(D_METHOD("instantiate_scene", "static_lights", "allow_shadows"), &JigsawParameterScene::instantiate_scene);
	ClassDB::bind_static_method("JigsawParameterScene", D_METHOD("parse_animation_data", "raw"), &JigsawParameterScene::parse_animation_data);
	ClassDB::bind_static_method("JigsawParameterScene", D_METHOD("serialize_animation_data", "node"), &JigsawParameterScene::serialize_animation_data);
}

IMPLEMENT_PROPERTY(JigsawParameterScene, Ref<GLTFState>, scene);
IMPLEMENT_PROPERTY(JigsawParameterScene, Ref<AnimationRootNode>, animation);
IMPLEMENT_PROPERTY(JigsawParameterScene, Dictionary, json_data);

static LazyGlobalFile<GDScript> FILE_REQUEST{"res://api/file_request.gd"};
static LazyGlobal<GLTFDocument> GLTF_DOCUMENT{ []() -> Ref<GLTFDocument> {
	ERR_FAIL_NULL_V(*FILE_REQUEST, Ref<GLTFDocument>());
	return FILE_REQUEST->get("_gltf_document");
} };

Node *JigsawParameterScene::instantiate_scene(bool static_lights, bool allow_shadows) const {
	ERR_FAIL_NULL_V(*GLTF_DOCUMENT, nullptr);
	ERR_FAIL_COND_V(get_scene().is_null(), nullptr);

	Node *scene = GLTF_DOCUMENT->generate_scene(get_scene());
	ERR_FAIL_NULL_V(scene, nullptr);

	TypedArray<Node> children = scene->find_children("*");
	for (int64_t i = 0; i < children.size(); i++) {
		Light3D *light = Object::cast_to<Light3D>(children[i]);
		if (likely(!light)) {
			continue;
		}

		// make scene lights static for GI
		light->set_bake_mode(static_lights ? Light3D::BAKE_STATIC : Light3D::BAKE_DISABLED);

		// enable shadows on lights where shadows are requested
		if (allow_shadows && FILE_REQUEST->call("_check_suffix", light->get_name(), "shadow")) {
			light->set_shadow(PlayerPreferences::shadows());
		}
	}

	return scene;
}

static bool safe_animation_node_dict(const Variant &json_value, bool root = false);
static bool safe_animation_node_value(const Variant &json_value) {
	switch (json_value.get_type()) {
	case Variant::STRING:
	case Variant::FLOAT:
	case Variant::BOOL:
	case Variant::NIL:
		return true;
	case Variant::ARRAY:
		return json_value.operator Array().all(callable_mp_static(&safe_animation_node_value));
	case Variant::DICTIONARY:
	{
		const Dictionary dict = json_value;
		if (dict.keys().size() == 2 && dict.has_all(Array::make("type", "args")) && dict["type"] == "Vector2") {
			ERR_FAIL_COND_V_MSG(dict["args"].get_type() != Variant::ARRAY, false, "invalid Vector2");
			const Array args = dict["args"];
			ERR_FAIL_COND_V_MSG(args.size() != 2 || args[0].get_type() != Variant::FLOAT || args[1].get_type() != Variant::FLOAT, false, "invalid Vector2");
			return true;
		}
		return safe_animation_node_dict(dict);
	}
	default:
		ERR_FAIL_V_MSG(false, vformat("unexpected type for property: %s", Variant::get_type_name(json_value.get_type())));
	}
}
static bool safe_animation_node_dict(const Variant &json_value, bool root) {
	ERR_FAIL_COND_V_MSG(json_value.get_type() != Variant::DICTIONARY, false, vformat("invalid animation object type %s", Variant::get_type_name(json_value.get_type())));
	const Dictionary dict = json_value;
	ERR_FAIL_COND_V_MSG(dict.keys().size() != 2 || !dict.has_all(Array::make("type", "props")), false, vformat("invalid dict keys %s", dict.keys()));

	ERR_FAIL_COND_V_MSG(dict["type"].get_type() != Variant::STRING, false, "invalid type type");
	const String type = dict["type"];
	ERR_FAIL_COND_V_MSG(ClassDB::is_parent_class(type, "AnimationNodeExtension"), false, vformat("invalid class %s", type));
	ERR_FAIL_COND_V_MSG(!ClassDB::is_parent_class(type, root ? "AnimationRootNode" : "AnimationNode") && (root || (type != "AnimationNodeStateMachineTransition" && type != "Curve")), false, vformat("invalid class %s", type));

	ERR_FAIL_COND_V_MSG(dict["props"].get_type() != Variant::ARRAY, false, "invalid props type");
	const Array props = dict["props"];
	ERR_FAIL_COND_V_MSG(props.size() % 2 != 0, false, vformat("invalid props array length %d", props.size()));
	for (int64_t i = 0; i < props.size(); i += 2) {
		ERR_FAIL_COND_V_MSG(props[i].get_type() != Variant::STRING, false, "invalid prop key type");
		const String key = props[i];
		const Variant value = props[i + 1];
		if (key == "script") {
			ERR_FAIL_COND_V_MSG(value.get_type() != Variant::NIL, false, "cannot have script");
		} else {
			ERR_FAIL_COND_V_MSG(!safe_animation_node_value(value), false, vformat("error in property %s", key));
		}
	}

	return true;
}

Ref<AnimationRootNode> JigsawParameterScene::parse_animation_data(const String &raw) {
	// encode JSON as base64 to:
	// - discourage users from editing it directly
	// - make it pretty print on a single line rather than many
	// - avoid a mess of backslashes we would get if it was just encoded as a string
	String raw_json = Marshalls::get_singleton()->base64_to_utf8(raw);
	if (raw_json.is_empty()) {
		raw_json = "null";
	}
	Variant serialized_objects = JSON::parse_string(raw_json);
	if (serialized_objects.get_type() == Variant::NIL) {
		// special case: if the JSON fails to decode or it simply decodes to null, don't print an extra error message.
		return Ref<AnimationRootNode>();
	}

	if (safe_animation_node_dict(serialized_objects, true)) {
		return JSON::to_native(serialized_objects, true);
	}

	return Ref<AnimationRootNode>();
}
String JigsawParameterScene::serialize_animation_data(const Ref<AnimationRootNode> &node) {
	if (node.is_null()) {
		return String();
	}

	Variant serialized_objects = JSON::from_native(node, true);
	ERR_FAIL_COND_V_MSG(!safe_animation_node_dict(serialized_objects, true), String(), "serialized animation tree failed validation");

	String raw_json = JSON::stringify(serialized_objects);
	return Marshalls::get_singleton()->utf8_to_base64(raw_json);
}
