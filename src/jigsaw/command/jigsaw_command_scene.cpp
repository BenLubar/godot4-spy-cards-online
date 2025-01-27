#include "jigsaw_command_scene.h"

#include <godot_cpp/classes/gd_script.hpp>
#include <godot_cpp/classes/gltf_document.hpp>
#include <godot_cpp/classes/light3d.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>

#include "jigsaw/jigsaw_global.h"
#include "jigsaw/jigsaw_visual.h"
#include "jigsaw/parameter/jigsaw_parameter_file_id_gltf.h"
#include "jigsaw/parameter/jigsaw_parameter_float.h"
#include "jigsaw/parameter/jigsaw_parameter_scene_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_string.h"
#include "util/player_preferences_helper.h"

static LazyGlobalFile<GDScript> FILE_REQUEST{"res://api/file_request.gd"};
static LazyGlobal<GLTFDocument> GLTF_DOCUMENT{ []() -> Ref<GLTFDocument> {
	ERR_FAIL_NULL_V(*FILE_REQUEST, Ref<GLTFDocument>());
	return FILE_REQUEST->get("_gltf_document");
} };

void JigsawCommandScene::_bind_methods() {
	BIND_ENUM_CONSTANT(GET_PROPERTY_STRING);
	BIND_ENUM_CONSTANT(GET_PROPERTY_FLOAT);
	BIND_ENUM_CONSTANT(INSTANTIATE);

	BIND_PROPERTY_ENUM(JigsawCommandScene::Operation, operation);
	BIND_PROPERTY_RESOURCE(JigsawParameter, scene);
	BIND_PROPERTY_RESOURCE(JigsawParameter, property_name);
	BIND_PROPERTY_RESOURCE(JigsawParameter, default_value);
	BIND_PROPERTY_RESOURCE(JigsawParameterLocalVariable, output);
}

IMPLEMENT_PROPERTY(JigsawCommandScene, JigsawCommandScene::Operation, operation);
IMPLEMENT_PROPERTY(JigsawCommandScene, Ref<JigsawParameter>, scene);
IMPLEMENT_PROPERTY(JigsawCommandScene, Ref<JigsawParameter>, property_name);
IMPLEMENT_PROPERTY(JigsawCommandScene, Ref<JigsawParameter>, default_value);
IMPLEMENT_PROPERTY(JigsawCommandScene, Ref<JigsawParameterLocalVariable>, output);

bool JigsawCommandScene::allowed_in_mode(enums::JigsawProcedure::Mode mode, bool any_config) const {
	using namespace enums::JigsawProcedure;

	switch (mode) {
	case FUNCTIONAL:
	case INIT:
	case MAIN:
	case SELECT:
	case VISUAL:
	case CHOICE_PREVIEW:
	case REALTIME_LOGIC:
	case REALTIME_VISUAL:
		break; // TODO
	}

	ERR_FAIL_V(false);
}
JigsawExecutionState JigsawCommandScene::evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const {
	switch (_operation) {
	case GET_PROPERTY_STRING:
	{
		Ref<JigsawParameterScene> scene_id;
		err = context->resolve_variable(_scene, scene_id, "scene_id");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		Ref<JigsawParameterString> property_name;
		err = context->resolve_variable(_property_name, property_name, "property_name");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		Ref<JigsawParameterString> default_value;
		err = context->resolve_variable(_default_value, default_value, "default_value");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		Dictionary data = scene_id->get_json_data();
		Variant value = data.get(property_name->get_string(), default_value->get_string());
		if (value.get_type() != Variant::STRING) {
			value = default_value->get_string();
		}

		return set_command_result(context, err, 0, JigsawParameterString::make(value));
	}
	case GET_PROPERTY_FLOAT:
	{
		Ref<JigsawParameterScene> scene_id;
		err = context->resolve_variable(_scene, scene_id, "scene_id");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		Ref<JigsawParameterString> property_name;
		err = context->resolve_variable(_property_name, property_name, "property_name");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		Ref<JigsawParameterFloat> default_value;
		err = context->resolve_variable(_default_value, default_value, "default_value");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		Dictionary data = scene_id->get_json_data();
		Variant value = data.get(property_name->get_string(), default_value->get_value());
		if (value.get_type() != Variant::FLOAT) {
			value = default_value->get_value();
		}

		return set_command_result(context, err, 0, JigsawParameterFloat::make(value));
	}
	case INSTANTIATE:
	{
		Ref<JigsawParameterScene> scene_id;
		err = context->resolve_variable(_scene, scene_id, "scene_id");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		Node *scene = GLTF_DOCUMENT->generate_scene(scene_id->get_scene());

		// TODO!
		TypedArray<Node> children = scene->find_children("*");
		for (int64_t i = 0; i < children.size(); i++) {
			Light3D *light = Object::cast_to<Light3D>(children[i]);
			if (light) {
				// make all lights static for GI
				light->set_bake_mode(Light3D::BAKE_STATIC);

				// enable shadows on lights where shadows are requested
				if (likely(*FILE_REQUEST) && FILE_REQUEST->call("_check_suffix", light->get_name(), "shadow")) {
					light->set_shadow(PlayerPreferences::shadows());
				}
			}
		}

		// TODO!!!
		context->get_global()->get_visual()->get_stage_viewport()->add_child(scene);

		// TODO!!!!!
		if (scene_id->get_json_data().has("clear_color") && scene_id->get_json_data().get("clear_color", Variant()).get_type() == Variant::STRING) {
			context->get_global()->get_visual()->get_stage_viewport()->get_world_3d()->get_environment()->set_background(Environment::BG_COLOR);
			context->get_global()->get_visual()->get_stage_viewport()->get_world_3d()->get_environment()->set_bg_color(scene_id->get_json_data().get("clear_color", Variant()).stringify());
		} else {
			context->get_global()->get_visual()->get_stage_viewport()->get_world_3d()->get_environment()->set_background(Environment::BG_CLEAR_COLOR);
		}
		context->get_global()->get_visual()->get_stage_viewport()->get_world_3d()->get_environment()->set_ambient_light_sky_contribution(scene_id->get_json_data().get("ambient_light_intensity", 1.0f));

		// TODO
		return set_command_result(context, err, 0, JigsawParameterSceneInstance::make(-1));
	}
	}

	err = context->create_error(vformat("internal error: unhandled scene operation %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawCommandScene", "Operation", _operation)));
	return JigsawExecutionState::ERROR;
}

int64_t JigsawCommandScene::get_num_configs() const {
	return 1;
}
String JigsawCommandScene::get_config_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "Operation";
}
String JigsawCommandScene::get_config_desc(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "";
}
int64_t JigsawCommandScene::get_config_value(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, 0);

	return _operation;
}
void JigsawCommandScene::set_config_value(int64_t i, int64_t value) {
	ERR_FAIL_INDEX(i, 1);

	_operation = static_cast<Operation>(value);
	emit_changed();
}
PackedStringArray JigsawCommandScene::get_config_options(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, PackedStringArray());

	// If these asserts fail, you have broken compatibility with existing game modes.
	static_assert(GET_PROPERTY_STRING == 0);
	static_assert(GET_PROPERTY_FLOAT == 1);
	static_assert(INSTANTIATE == 2);

	return PackedStringArray{
		"Get property (string)",
		"Get property (float)",
		"Instantiate",
	};
}

int64_t JigsawCommandScene::get_num_arguments() const {
	switch (_operation) {
	case GET_PROPERTY_STRING:
	case GET_PROPERTY_FLOAT:
		return 3;
	case INSTANTIATE:
		return 1;
	}

	return 0;
}
Ref<JigsawParameter> JigsawCommandScene::get_argument(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), Ref<JigsawParameter>());

	switch (_operation) {
	case GET_PROPERTY_STRING:
	case GET_PROPERTY_FLOAT:
		if (i == 0) {
			return _scene;
		} else if (i == 1) {
			return _property_name;
		} else if (i == 2) {
			return _default_value;
		}
		break;
	case INSTANTIATE:
		if (i == 0) {
			return _scene;
		}
		break;
	}

	ERR_FAIL_V(Ref<JigsawParameter>());
}
TypedArray<JigsawParameter> JigsawCommandScene::get_argument_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), TypedArray<JigsawParameter>());

	switch (_operation) {
	case GET_PROPERTY_STRING:
		if (i == 0) {
			return Array::make(JigsawParameterFileIDGLTF::make(PackedByteArray(), Dictionary()));
		} else if (i == 1) {
			return Array::make(JigsawParameterString::make(""));
		} else if (i == 2) {
			return Array::make(JigsawParameterString::make(""));
		}
		break;
	case GET_PROPERTY_FLOAT:
		if (i == 0) {
			return Array::make(JigsawParameterFileIDGLTF::make(PackedByteArray(), Dictionary()));
		} else if (i == 1) {
			return Array::make(JigsawParameterString::make(""));
		} else if (i == 2) {
			return Array::make(JigsawParameterFloat::make(0.0));
		}
		break;
	case INSTANTIATE:
		if (i == 0) {
			return Array::make(JigsawParameterFileIDGLTF::make(PackedByteArray(), Dictionary()));
		}
		break;
	}

	ERR_FAIL_V(TypedArray<JigsawParameter>());
}
void JigsawCommandScene::set_argument(int64_t i, const Ref<JigsawParameter> &arg) {
	ERR_FAIL_INDEX(i, get_num_arguments());

	switch (_operation) {
	case GET_PROPERTY_STRING:
	case GET_PROPERTY_FLOAT:
		if (i == 0) {
			_scene = arg;
			emit_changed();
			return;
		} else if (i == 1) {
			_property_name = arg;
			emit_changed();
			return;
		} else if (i == 2) {
			_default_value = arg;
			emit_changed();
			return;
		}
		break;
	case INSTANTIATE:
		if (i == 0) {
			_scene = arg;
			emit_changed();
			return;
		}
		break;
	}

	ERR_FAIL();
}
String JigsawCommandScene::get_argument_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), "");

	switch (_operation) {
	case GET_PROPERTY_STRING:
	case GET_PROPERTY_FLOAT:
		if (i == 0) {
			return "scene_id";
		} else if (i == 1) {
			return "property_name";
		} else if (i == 2) {
			return "default_value";
		}
		break;
	case INSTANTIATE:
		if (i == 0) {
			return "scene_id";
		}
		break;
	}

	ERR_FAIL_V("");
}

int64_t JigsawCommandScene::get_num_results() const {
	switch (_operation) {
	case GET_PROPERTY_STRING:
	case GET_PROPERTY_FLOAT:
	case INSTANTIATE:
		return 1;
	}

	ERR_FAIL_V(0);
}
Ref<JigsawParameterLocalVariable> JigsawCommandScene::get_result(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_results(), Ref<JigsawParameterLocalVariable>());

	return _output;
}
TypedArray<JigsawParameter> JigsawCommandScene::get_result_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, get_num_results(), TypedArray<JigsawParameter>());

	switch (_operation) {
	case GET_PROPERTY_STRING:
		return Array::make(JigsawParameterString::make(""));
	case GET_PROPERTY_FLOAT:
		return Array::make(JigsawParameterFloat::make(0.0));
	case INSTANTIATE:
		return Array::make(JigsawParameterSceneInstance::make(-1));
	}

	return TypedArray<JigsawParameter>();
}
void JigsawCommandScene::set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) {
	ERR_FAIL_INDEX(i, get_num_results());

	_output = result;
	emit_changed();
}
String JigsawCommandScene::get_result_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_results(), "");

	switch (_operation) {
	case GET_PROPERTY_STRING:
	case GET_PROPERTY_FLOAT:
		if (i == 0) {
			return "value";
		}
		break;
	case INSTANTIATE:
		if (i == 0) {
			return "scene";
		}
		break;
	}

	ERR_FAIL_V("");
}
