#include "jigsaw/command/jigsaw_command_scene.h"

#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>

#include "jigsaw/jigsaw_global.h"
#include "jigsaw/jigsaw_visual.h"
#include "jigsaw/parameter/jigsaw_parameter_boolean.h"
#include "jigsaw/parameter/jigsaw_parameter_file_id_gltf.h"
#include "jigsaw/parameter/jigsaw_parameter_float.h"
#include "jigsaw/parameter/jigsaw_parameter_scene_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_string.h"
#include "util/player_preferences_helper.h"

void JigsawCommandScene::_bind_methods() {
	BIND_ENUM_CONSTANT(GET_PROPERTY_STRING);
	BIND_ENUM_CONSTANT(GET_PROPERTY_FLOAT);
	BIND_ENUM_CONSTANT(CREATE_IN_STAGE);
	BIND_ENUM_CONSTANT(CREATE_IN_HUD);
	BIND_ENUM_CONSTANT(DESTROY);
	BIND_ENUM_CONSTANT(SET_VISIBLE);

	BIND_PROPERTY_ENUM(JigsawCommandScene::Operation, operation);
	BIND_PROPERTY_RESOURCE(JigsawParameter, scene);
	BIND_PROPERTY_RESOURCE(JigsawParameter, property_name);
	BIND_PROPERTY_RESOURCE(JigsawParameter, default_value);
	BIND_PROPERTY_RESOURCE(JigsawParameter, x);
	BIND_PROPERTY_RESOURCE(JigsawParameter, y);
	BIND_PROPERTY_RESOURCE(JigsawParameter, z);
	BIND_PROPERTY_RESOURCE(JigsawParameter, pitch);
	BIND_PROPERTY_RESOURCE(JigsawParameter, yaw);
	BIND_PROPERTY_RESOURCE(JigsawParameter, roll);
	BIND_PROPERTY_RESOURCE(JigsawParameterLocalVariable, output);
}

IMPLEMENT_PROPERTY(JigsawCommandScene, JigsawCommandScene::Operation, operation);
IMPLEMENT_PROPERTY(JigsawCommandScene, Ref<JigsawParameter>, scene);
IMPLEMENT_PROPERTY(JigsawCommandScene, Ref<JigsawParameter>, property_name);
IMPLEMENT_PROPERTY(JigsawCommandScene, Ref<JigsawParameter>, default_value);
IMPLEMENT_PROPERTY(JigsawCommandScene, Ref<JigsawParameter>, x);
IMPLEMENT_PROPERTY(JigsawCommandScene, Ref<JigsawParameter>, y);
IMPLEMENT_PROPERTY(JigsawCommandScene, Ref<JigsawParameter>, z);
IMPLEMENT_PROPERTY(JigsawCommandScene, Ref<JigsawParameter>, pitch);
IMPLEMENT_PROPERTY(JigsawCommandScene, Ref<JigsawParameter>, yaw);
IMPLEMENT_PROPERTY(JigsawCommandScene, Ref<JigsawParameter>, roll);
IMPLEMENT_PROPERTY(JigsawCommandScene, Ref<JigsawParameterLocalVariable>, output);

bool JigsawCommandScene::allowed_in_mode(enums::JigsawProcedure::Mode mode, bool any_config) const {
	using namespace enums::JigsawProcedure;

	switch (mode) {
	case FUNCTIONAL:
		if (any_config) {
			return true;
		}
		switch (_operation) {
		case GET_PROPERTY_STRING:
		case GET_PROPERTY_FLOAT:
			return true;
		case CREATE_IN_STAGE:
		case CREATE_IN_HUD:
		case DESTROY:
		case SET_VISIBLE:
			return false;
		}
		ERR_FAIL_V(false);
	case INIT:
	case MAIN:
	case SELECT:
	case VISUAL:
	case CHOICE_PREVIEW:
	case REALTIME_LOGIC:
	case REALTIME_VISUAL:
		return true;
	}

	ERR_FAIL_V(false);
}
static bool safe_node_path(const Ref<JigsawContext> &context, const NodePath &node_path, Ref<JigsawError> &err) {
	if (node_path.is_empty()) {
		err = context->create_error("empty path given for this command");
		return false;
	}
	if (node_path.is_absolute()) {
		err = context->create_error("path cannot start with /");
		return false;
	}
	if (node_path.get_subname_count() != 0) {
		err = context->create_error("path cannot contain :");
		return false;
	}
	if (node_path == NodePath(".")) {
		// special case: allow . if it is the only path component
		return true;
	}
	static LazyStringName dot{"."};
	static LazyStringName dotdot{".."};
	for (int64_t i = 0; i < node_path.get_name_count(); i++) {
		StringName component = node_path.get_name(i);
		if (component == dot || component == dotdot) {
			err = context->create_error("path cannot contain ..");
			return false;
		}
	}
	return true;
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
	case CREATE_IN_STAGE:
	case CREATE_IN_HUD:
	{
		JigsawGlobal *global = context->get_global();

		Ref<JigsawParameterScene> scene_id;
		err = context->resolve_variable(_scene, scene_id, "scene_id");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		Ref<JigsawParameterFloat> x, y, z;
		err = context->resolve_variable(_x, x, "x");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		err = context->resolve_variable(_y, y, "y");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		err = context->resolve_variable(_z, z, "z");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		Ref<JigsawParameterFloat> pitch, yaw, roll;
		err = context->resolve_variable(_pitch, pitch, "pitch");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		err = context->resolve_variable(_yaw, yaw, "yaw");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		err = context->resolve_variable(_roll, roll, "roll");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		Node *scene = scene_id->instantiate_scene(true, true);
		if (unlikely(!scene)) {
			err = context->create_error("failed to instantiate scene", Array::make(scene_id));
			return JigsawExecutionState::ERROR;
		}
		Node3D *scene3d = Object::cast_to<Node3D>(scene);
		CRASH_COND_MSG(!scene3d, vformat("scene was not 3D (node type was %s)", scene->get_class()));

		if (_operation == CREATE_IN_STAGE) {
			context->get_global()->get_visual()->get_stage_viewport()->add_child(scene);
		} else if (_operation == CREATE_IN_HUD) {
			context->get_global()->get_visual()->add_child(scene);
		} else {
			CRASH_NOW_MSG("creating scene but not in HUD or stage!");
		}

		scene3d->set_position(Vector3(x->get_value(), y->get_value(), z->get_value()));
		scene3d->set_rotation_degrees(Vector3(pitch->get_value(), yaw->get_value(), roll->get_value()));

		Ref<JigsawSceneManipulator> manipulator;
		manipulator.instantiate();

		manipulator->set_scene_id(scene_id);
		manipulator->set_root(scene);

		Ref<AnimationRootNode> animation = scene_id->get_animation();
		if (animation.is_valid() && scene->get_node<AnimationPlayer>("AnimationPlayer")) {
			AnimationTree *tree = memnew(AnimationTree);
			tree->set_name("AnimationTree");
			tree->set_tree_root(animation);
			tree->set_root_node("..");
			tree->set_animation_player("../AnimationPlayer");
			scene->add_child(tree);
			manipulator->set_animation_tree(tree);
		}

		int64_t current_frame = global->get_current_frame();
		if (current_frame != -1) {
			manipulator->discard_rollback_data(0); // init rollback
			manipulator->set_created_frame(current_frame);
			manipulator->set_base_frame(current_frame);
			manipulator->set_current_frame(current_frame);
		}

		TypedArray<JigsawSceneManipulator> scenes = global->get_scenes();
		int64_t scene_index = 0;
		while (scenes.size() > scene_index && scenes[scene_index].get_type() != Variant::NIL) {
			scene_index++;
		}

		if (scene_index == scenes.size()) {
			scenes.append(manipulator);
		} else {
			scenes[scene_index] = manipulator;
		}
		global->set_scenes(scenes);

		return set_command_result(context, err, 0, JigsawParameterSceneInstance::make(scene_index));
	}
	case DESTROY:
	{
		JigsawGlobal *global = context->get_global();

		Ref<JigsawParameterSceneInstance> scene;
		err = context->resolve_variable(_scene, scene, "scene");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		TypedArray<JigsawSceneManipulator> scenes = global->get_scenes();
		Ref<JigsawSceneManipulator> manipulator = likely(scene->get_scene() >= 0 && scene->get_scene() < scenes.size()) ? scenes[scene->get_scene()] : Variant();
		Node *scene_root = likely(manipulator.is_valid()) ? manipulator->get_root_rollback() : nullptr;
		if (unlikely(!scene_root)) {
			err = context->create_error("cannot destroy invalid scene");
			return JigsawExecutionState::ERROR;
		}

		manipulator->destroy(global->get_current_frame());

		return JigsawExecutionState::CONTINUE;
	}
	case SET_VISIBLE:
	{
		JigsawGlobal *global = context->get_global();

		Ref<JigsawParameterSceneInstance> scene;
		err = context->resolve_variable(_scene, scene, "scene");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		Ref<JigsawParameterString> path_name;
		err = context->resolve_variable(_property_name, path_name, "path_name");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		Ref<JigsawParameterBoolean> visible;
		err = context->resolve_variable(_default_value, visible, "visible");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		TypedArray<JigsawSceneManipulator> scenes = global->get_scenes();
		Ref<JigsawSceneManipulator> manipulator = likely(scene->get_scene() >= 0 && scene->get_scene() < scenes.size()) ? scenes[scene->get_scene()] : Variant();
		Node *scene_root = likely(manipulator.is_valid()) ? manipulator->get_root_rollback() : nullptr;
		if (unlikely(!scene_root)) {
			err = context->create_error("cannot modify invalid scene");
			return JigsawExecutionState::ERROR;
		}

		NodePath node_path = path_name->get_string();
		if (unlikely(!safe_node_path(context, node_path, err))) {
			return JigsawExecutionState::ERROR;
		}

		manipulator->set_visible(node_path, visible->get_boolean(), global->get_current_frame());

		return JigsawExecutionState::CONTINUE;
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
	static_assert(CREATE_IN_STAGE == 2);
	static_assert(CREATE_IN_HUD == 3);
	static_assert(DESTROY == 4);
	static_assert(SET_VISIBLE == 5);

	return PackedStringArray{
		"Get property (string)",
		"Get property (float)",
		"Create in stage",
		"Create in HUD",
		"Destroy",
		"Set Visible",
	};
}

int64_t JigsawCommandScene::get_num_arguments() const {
	switch (_operation) {
	case GET_PROPERTY_STRING:
	case GET_PROPERTY_FLOAT:
		return 3;
	case CREATE_IN_STAGE:
	case CREATE_IN_HUD:
		return 7;
	case DESTROY:
		return 1;
	case SET_VISIBLE:
		return 3;
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
	case CREATE_IN_STAGE:
	case CREATE_IN_HUD:
		if (i == 0) {
			return _scene;
		} else if (i == 1) {
			return _x;
		} else if (i == 2) {
			return _y;
		} else if (i == 3) {
			return _z;
		} else if (i == 4) {
			return _pitch;
		} else if (i == 5) {
			return _yaw;
		} else if (i == 6) {
			return _roll;
		}
		break;
	case DESTROY:
		if (i == 0) {
			return _scene;
		}
		break;
	case SET_VISIBLE:
		if (i == 0) {
			return _scene;
		} else if (i == 1) {
			return _property_name;
		} else if (i == 2) {
			return _default_value;
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
	case CREATE_IN_STAGE:
	case CREATE_IN_HUD:
		if (i == 0) {
			return Array::make(JigsawParameterFileIDGLTF::make(PackedByteArray(), Dictionary()));
		} else if (i >= 1 && i <= 6) {
			return Array::make(JigsawParameterFloat::make(0.0));
		}
		break;
	case DESTROY:
		if (i == 0) {
			return Array::make(JigsawParameterSceneInstance::make(-1));
		}
		break;
	case SET_VISIBLE:
		if (i == 0) {
			return Array::make(JigsawParameterSceneInstance::make(-1));
		} else if (i == 1) {
			return Array::make(JigsawParameterString::make("."));
		} else if (i == 2) {
			return Array::make(JigsawParameterBoolean::make(true));
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
	case CREATE_IN_STAGE:
	case CREATE_IN_HUD:
		if (i == 0) {
			_scene = arg;
			emit_changed();
			return;
		} else if (i == 1) {
			_x = arg;
			emit_changed();
			return;
		} else if (i == 2) {
			_y = arg;
			emit_changed();
			return;
		} else if (i == 3) {
			_z = arg;
			emit_changed();
			return;
		} else if (i == 4) {
			_pitch = arg;
			emit_changed();
			return;
		} else if (i == 5) {
			_yaw = arg;
			emit_changed();
			return;
		} else if (i == 6) {
			_roll = arg;
			emit_changed();
			return;
		}
		break;
	case DESTROY:
		if (i == 0) {
			_scene = arg;
			emit_changed();
			return;
		}
		break;
	case SET_VISIBLE:
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
	case CREATE_IN_STAGE:
	case CREATE_IN_HUD:
		if (i == 0) {
			return "scene_id";
		} else if (i == 1) {
			return "x";
		} else if (i == 2) {
			return "y";
		} else if (i == 3) {
			return "z";
		} else if (i == 4) {
			return "pitch";
		} else if (i == 5) {
			return "yaw";
		} else if (i == 6) {
			return "roll";
		}
		break;
	case DESTROY:
		if (i == 0) {
			return "scene";
		}
		break;
	case SET_VISIBLE:
		if (i == 0) {
			return "scene";
		} else if (i == 1) {
			return "node_path";
		} else if (i == 2) {
			return "visible";
		}
		break;
	}

	ERR_FAIL_V("");
}

int64_t JigsawCommandScene::get_num_results() const {
	switch (_operation) {
	case GET_PROPERTY_STRING:
	case GET_PROPERTY_FLOAT:
	case CREATE_IN_STAGE:
	case CREATE_IN_HUD:
		return 1;
	case DESTROY:
	case SET_VISIBLE:
		return 0;
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
	case CREATE_IN_STAGE:
	case CREATE_IN_HUD:
		return Array::make(JigsawParameterSceneInstance::make(-1));
	case DESTROY:
	case SET_VISIBLE:
		break;
	}

	ERR_FAIL_V(TypedArray<JigsawParameter>());
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
	case CREATE_IN_STAGE:
	case CREATE_IN_HUD:
		if (i == 0) {
			return "scene";
		}
		break;
	case DESTROY:
	case SET_VISIBLE:
		break;
	}

	ERR_FAIL_V("");
}
