#include "jigsaw/command/jigsaw_command_camera.h"

#include <godot_cpp/classes/environment.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>

#include "jigsaw/jigsaw_global.h"
#include "jigsaw/jigsaw_visual.h"
#include "jigsaw/parameter/jigsaw_parameter_boolean.h"
#include "jigsaw/parameter/jigsaw_parameter_color.h"
#include "jigsaw/parameter/jigsaw_parameter_float.h"
#include "jigsaw/parameter/jigsaw_parameter_icon.h"
#include "jigsaw/parameter/jigsaw_parameter_ordered_list.h"

#include "util/player_preferences_helper.h"

void JigsawCommandCamera::_bind_methods() {
	BIND_ENUM_CONSTANT(SET_TARGET_POSITION);
	BIND_ENUM_CONSTANT(SET_TARGET_ROTATION);
	BIND_ENUM_CONSTANT(SET_CAMERA_OFFSET);
	BIND_ENUM_CONSTANT(SET_CAMERA_ROTATION_OFFSET);
	BIND_ENUM_CONSTANT(SET_TARGET_POSITION_EXPRESSION);
	BIND_ENUM_CONSTANT(SET_TARGET_ROTATION_EXPRESSION);
	BIND_ENUM_CONSTANT(SET_CAMERA_OFFSET_EXPRESSION);
	BIND_ENUM_CONSTANT(SET_CAMERA_ROTATION_OFFSET_EXPRESSION);
	BIND_ENUM_CONSTANT(SET_SIMPLE_BACKGROUND);
	BIND_ENUM_CONSTANT(SET_BACKGROUND_COLOR);
	BIND_ENUM_CONSTANT(SET_AMBIENT_LIGHT_INTENSITY);

	BIND_PROPERTY_ENUM(JigsawCommandCamera::Operation, operation);
	BIND_PROPERTY_RESOURCE(JigsawParameter, value);
	BIND_PROPERTY_RESOURCE(JigsawParameter, stretch);
	BIND_PROPERTY_RESOURCE(JigsawParameter, force);
	BIND_PROPERTY_RESOURCE(JigsawParameter, expression);
	BIND_PROPERTY_RESOURCE(JigsawParameter, x);
	BIND_PROPERTY_RESOURCE(JigsawParameter, y);
	BIND_PROPERTY_RESOURCE(JigsawParameter, z);
	BIND_PROPERTY_RESOURCE(JigsawParameter, pitch);
	BIND_PROPERTY_RESOURCE(JigsawParameter, yaw);
	BIND_PROPERTY_RESOURCE(JigsawParameter, roll);
	BIND_PROPERTY_RESOURCE(JigsawParameter, duration);
}

IMPLEMENT_PROPERTY(JigsawCommandCamera, JigsawCommandCamera::Operation, operation);
IMPLEMENT_PROPERTY(JigsawCommandCamera, Ref<JigsawParameter>, value);
IMPLEMENT_PROPERTY(JigsawCommandCamera, Ref<JigsawParameter>, stretch);
IMPLEMENT_PROPERTY(JigsawCommandCamera, Ref<JigsawParameter>, force);
IMPLEMENT_PROPERTY(JigsawCommandCamera, Ref<JigsawParameter>, expression);
IMPLEMENT_PROPERTY(JigsawCommandCamera, Ref<JigsawParameter>, x);
IMPLEMENT_PROPERTY(JigsawCommandCamera, Ref<JigsawParameter>, y);
IMPLEMENT_PROPERTY(JigsawCommandCamera, Ref<JigsawParameter>, z);
IMPLEMENT_PROPERTY(JigsawCommandCamera, Ref<JigsawParameter>, pitch);
IMPLEMENT_PROPERTY(JigsawCommandCamera, Ref<JigsawParameter>, yaw);
IMPLEMENT_PROPERTY(JigsawCommandCamera, Ref<JigsawParameter>, roll);
IMPLEMENT_PROPERTY(JigsawCommandCamera, Ref<JigsawParameter>, duration);

bool JigsawCommandCamera::allowed_in_mode(enums::JigsawProcedure::Mode mode, bool any_config) const {
	using namespace enums::JigsawProcedure;

	switch (mode) {
	case FUNCTIONAL:
		return false;
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
JigsawExecutionState JigsawCommandCamera::evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const {
	Ref<JigsawGenericNode> visual = context->get_global()->get_visual_rollback();
	int64_t current_frame = context->get_global()->get_current_frame();

	switch (_operation) {
#define GET_FINITE_FLOAT_PARAM(m_name) \
		Ref<JigsawParameterFloat> m_name; \
		err = context->resolve_variable(_##m_name, m_name, #m_name); \
		if (unlikely(err.is_valid())) { \
			return JigsawExecutionState::ERROR; \
		} \
		if (unlikely(!Math::is_finite(m_name->get_value()))) { \
			err = context->create_error(#m_name " is not a number", Array::make(m_name)); \
			return JigsawExecutionState::ERROR; \
		}
#define GET_DURATION_PARAM() \
		GET_FINITE_FLOAT_PARAM(duration); \
		if (unlikely(duration->get_value() < 0.0)) { \
			err = context->create_error("duration is negative", Array::make(duration)); \
			return JigsawExecutionState::ERROR; \
		}
	case SET_TARGET_POSITION:
	{
		GET_FINITE_FLOAT_PARAM(x);
		GET_FINITE_FLOAT_PARAM(y);
		GET_FINITE_FLOAT_PARAM(z);
		GET_DURATION_PARAM();

		visual->set_property("stage_camera_target_position_expr", Ref<JigsawParameterExpression>(), 0.0f, current_frame);
		visual->set_property("stage_camera_target_position_expr_args", PackedFloat64Array(), 0.0f, current_frame);
		visual->set_property("stage_camera_target_position", Vector3(x->get_value(), y->get_value(), z->get_value()), duration->get_value(), current_frame);

		return JigsawExecutionState::CONTINUE;
	}
	case SET_TARGET_ROTATION:
	{
		GET_FINITE_FLOAT_PARAM(pitch);
		GET_FINITE_FLOAT_PARAM(yaw);
		GET_FINITE_FLOAT_PARAM(roll);
		GET_DURATION_PARAM();

		visual->set_property("stage_camera_target_rotation_expr", Ref<JigsawParameterExpression>(), 0.0f, current_frame);
		visual->set_property("stage_camera_target_rotation_expr_args", PackedFloat64Array(), 0.0f, current_frame);
		visual->set_property("stage_camera_target_rotation_degrees", Vector3(pitch->get_value(), yaw->get_value(), roll->get_value()), duration->get_value(), current_frame);

		return JigsawExecutionState::CONTINUE;
	}
	case SET_CAMERA_OFFSET:
	{
		GET_FINITE_FLOAT_PARAM(x);
		GET_FINITE_FLOAT_PARAM(y);
		GET_FINITE_FLOAT_PARAM(z);
		GET_DURATION_PARAM();

		visual->set_property("stage_camera_offset_expr", Ref<JigsawParameterExpression>(), 0.0f, current_frame);
		visual->set_property("stage_camera_offset_expr_args", PackedFloat64Array(), 0.0f, current_frame);
		visual->set_property("stage_camera_offset", Vector3(x->get_value(), y->get_value(), z->get_value()), duration->get_value(), current_frame);

		return JigsawExecutionState::CONTINUE;
	}
	case SET_CAMERA_ROTATION_OFFSET:
	{
		GET_FINITE_FLOAT_PARAM(pitch);
		GET_FINITE_FLOAT_PARAM(yaw);
		GET_FINITE_FLOAT_PARAM(roll);
		GET_DURATION_PARAM();

		visual->set_property("stage_camera_rotation_offset_expr", Ref<JigsawParameterExpression>(), 0.0f, current_frame);
		visual->set_property("stage_camera_rotation_offset_expr_args", PackedFloat64Array(), 0.0f, current_frame);
		visual->set_property("stage_camera_rotation_offset_degrees", Vector3(pitch->get_value(), yaw->get_value(), roll->get_value()), duration->get_value(), current_frame);

		return JigsawExecutionState::CONTINUE;
	}
#define EXPRESSION_PREAMBLE() \
		Ref<JigsawParameterExpression> function; \
		err = context->resolve_variable(_expression, function, "function"); \
		if (unlikely(err.is_valid())) { \
			return JigsawExecutionState::ERROR; \
		} \
		if (unlikely(function->get_parsed_expression().is_null())) { \
			if (function->get_expression().is_empty()) { \
				function = Ref<JigsawParameterExpression>(); \
			} else { \
				err = context->create_error("invalid expression", Array::make(function)); \
				return JigsawExecutionState::ERROR; \
			} \
		} \
		Ref<JigsawParameterOrderedList> parameters; \
		err = context->resolve_variable(_value, parameters, "parameters"); \
		if (unlikely(err.is_valid())) { \
			return JigsawExecutionState::ERROR; \
		} \
		PackedFloat64Array params; \
		if (!parameters->is_template()) { \
			TypedArray<JigsawParameter> param_list = parameters->get_list(); \
			params.resize(param_list.size()); \
			for (int64_t i = 0; i < param_list.size(); i++) { \
				Ref<JigsawParameterFloat> param = param_list[i]; \
				if (unlikely(param.is_null())) { \
					err = context->create_error("parameter list must only contain float values"); \
					return JigsawExecutionState::ERROR; \
				} \
				params[i] = param->get_value(); \
			} \
		} \
		if (unlikely(function.is_null() && !params.is_empty())) { \
			err = context->create_error("cannot set parameter list without expression"); \
			return JigsawExecutionState::ERROR; \
		} \
		if (PlayerPreferences::prefers_reduced_motion()) { \
			return JigsawExecutionState::CONTINUE; \
		}
	case SET_TARGET_POSITION_EXPRESSION:
	{
		EXPRESSION_PREAMBLE();

		visual->stop_property("stage_camera_target_position", current_frame);
		visual->set_property("stage_camera_target_position_expr", function, 0.0f, current_frame);
		visual->set_property("stage_camera_target_position_expr_args", params, 0.0f, current_frame);

		return JigsawExecutionState::CONTINUE;
	}
	case SET_TARGET_ROTATION_EXPRESSION:
	{
		EXPRESSION_PREAMBLE();

		visual->stop_property("stage_camera_target_rotation_degrees", current_frame);
		visual->set_property("stage_camera_target_rotation_expr", function, 0.0f, current_frame);
		visual->set_property("stage_camera_target_rotation_expr_args", params, 0.0f, current_frame);

		return JigsawExecutionState::CONTINUE;
	}
	case SET_CAMERA_OFFSET_EXPRESSION:
	{
		EXPRESSION_PREAMBLE();

		visual->stop_property("stage_camera_offset", current_frame);
		visual->set_property("stage_camera_offset_expr", function, 0.0f, current_frame);
		visual->set_property("stage_camera_offset_expr_args", params, 0.0f, current_frame);

		return JigsawExecutionState::CONTINUE;
	}
	case SET_CAMERA_ROTATION_OFFSET_EXPRESSION:
	{
		EXPRESSION_PREAMBLE();

		visual->stop_property("stage_camera_rotation_offset_degrees");
		visual->set_property("stage_camera_rotation_offset_expr", function, 0.0f, current_frame);
		visual->set_property("stage_camera_rotation_offset_expr_args", params, 0.0f, current_frame);

		return JigsawExecutionState::CONTINUE;
	}
	case SET_SIMPLE_BACKGROUND:
	{
		Ref<JigsawParameterIcon> texture;
		err = context->resolve_variable(_value, texture, "texture");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		Ref<JigsawParameterBoolean> stretch;
		err = context->resolve_variable(_stretch, stretch, "stretch");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		Ref<JigsawParameterBoolean> force;
		err = context->resolve_variable(_force, force, "force");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		Ref<Texture2D> texture2d = context->get_global()->get_mode()->get_icon_texture(texture->get_icon());
		visual->set_property("simple_background", texture2d, 0.0f, current_frame);
		visual->set_property("simple_background_stretch", stretch->get_boolean(), 0.0f, current_frame);
		visual->set_property("force_simple_background", force->get_boolean(), 0.0f, current_frame);

		return JigsawExecutionState::CONTINUE;
	}
	case SET_BACKGROUND_COLOR:
	{
		Ref<JigsawParameterColor> color;
		err = context->resolve_variable(_value, color, "color");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		GET_DURATION_PARAM();

		visual->set_property("scene_background_color", color->get_color(), duration->get_value(), current_frame);

		return JigsawExecutionState::CONTINUE;
	}
	case SET_AMBIENT_LIGHT_INTENSITY:
	{
		const Ref<JigsawParameter> &_intensity = _value;
		GET_FINITE_FLOAT_PARAM(intensity);
		GET_DURATION_PARAM();

		visual->set_property("ambient_light_intensity", intensity->get_value(), duration->get_value(), current_frame);

		return JigsawExecutionState::CONTINUE;
	}
	}

	err = context->create_error(vformat("internal error: unhandled camera operation %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawCommandCamera", "Operation", _operation)));
	return JigsawExecutionState::ERROR;
}

int64_t JigsawCommandCamera::get_num_configs() const {
	return 1;
}
String JigsawCommandCamera::get_config_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "Operation";
}
String JigsawCommandCamera::get_config_desc(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "";
}
int64_t JigsawCommandCamera::get_config_value(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, 0);

	return _operation;
}
void JigsawCommandCamera::set_config_value(int64_t i, int64_t value) {
	ERR_FAIL_INDEX(i, 1);

	_operation = static_cast<Operation>(value);
	emit_changed();
}
PackedStringArray JigsawCommandCamera::get_config_options(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, PackedStringArray());

	// If these asserts fail, you have broken compatibility with existing game modes.
	static_assert(SET_TARGET_POSITION == 0);
	static_assert(SET_TARGET_ROTATION == 1);
	static_assert(SET_CAMERA_OFFSET == 2);
	static_assert(SET_CAMERA_ROTATION_OFFSET == 3);
	static_assert(SET_TARGET_POSITION_EXPRESSION == 4);
	static_assert(SET_TARGET_ROTATION_EXPRESSION == 5);
	static_assert(SET_CAMERA_OFFSET_EXPRESSION == 6);
	static_assert(SET_CAMERA_ROTATION_OFFSET_EXPRESSION == 7);
	static_assert(SET_SIMPLE_BACKGROUND == 8);
	static_assert(SET_BACKGROUND_COLOR == 9);
	static_assert(SET_AMBIENT_LIGHT_INTENSITY == 10);

	return PackedStringArray{
		"Set camera target position",
		"Set camera target rotation",
		"Set camera offset",
		"Set camera rotation offset",
		"Set camera target position function",
		"Set camera target rotation function",
		"Set camera offset function",
		"Set camera rotation offset function",
		"Set simple background",
		"Set background color",
		"Set ambient light intensity",
	};
}

int64_t JigsawCommandCamera::get_num_arguments() const {
	switch (_operation) {
	case SET_TARGET_POSITION:
	case SET_TARGET_ROTATION:
	case SET_CAMERA_OFFSET:
	case SET_CAMERA_ROTATION_OFFSET:
		return 4;
	case SET_TARGET_POSITION_EXPRESSION:
	case SET_TARGET_ROTATION_EXPRESSION:
	case SET_CAMERA_OFFSET_EXPRESSION:
	case SET_CAMERA_ROTATION_OFFSET_EXPRESSION:
		return 2;
	case SET_SIMPLE_BACKGROUND:
		return 3;
	case SET_BACKGROUND_COLOR:
	case SET_AMBIENT_LIGHT_INTENSITY:
		return 2;
	}

	ERR_FAIL_V(0);
}
Ref<JigsawParameter> JigsawCommandCamera::get_argument(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), Ref<JigsawParameter>());
	switch (_operation) {
	case SET_TARGET_POSITION:
		if (i == 0) {
			return _x;
		} else if (i == 1) {
			return _y;
		} else if (i == 2) {
			return _z;
		} else if (i == 3) {
			return _duration;
		}

		break;
	case SET_TARGET_ROTATION:
		if (i == 0) {
			return _pitch;
		} else if (i == 1) {
			return _yaw;
		} else if (i == 2) {
			return _roll;
		} else if (i == 3) {
			return _duration;
		}

		break;
	case SET_CAMERA_OFFSET:
		if (i == 0) {
			return _x;
		} else if (i == 1) {
			return _y;
		} else if (i == 2) {
			return _z;
		} else if (i == 3) {
			return _duration;
		}

		break;
	case SET_CAMERA_ROTATION_OFFSET:
		if (i == 0) {
			return _pitch;
		} else if (i == 1) {
			return _yaw;
		} else if (i == 2) {
			return _roll;
		} else if (i == 3) {
			return _duration;
		}

		break;
	case SET_TARGET_POSITION_EXPRESSION:
	case SET_TARGET_ROTATION_EXPRESSION:
	case SET_CAMERA_OFFSET_EXPRESSION:
	case SET_CAMERA_ROTATION_OFFSET_EXPRESSION:
		if (i == 0) {
			return _expression;
		} else if (i == 1) {
			return _value;
		}

		break;
	case SET_SIMPLE_BACKGROUND:
		if (i == 0) {
			return _value;
		} else if (i == 1) {
			return _stretch;
		} else if (i == 2) {
			return _force;
		}

		break;
	case SET_BACKGROUND_COLOR:
	case SET_AMBIENT_LIGHT_INTENSITY:
		if (i == 0) {
			return _value;
		} else if (i == 1) {
			return _duration;
		}

		break;
	}

	ERR_FAIL_V(Ref<JigsawParameter>());
}
TypedArray<JigsawParameter> JigsawCommandCamera::get_argument_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), TypedArray<JigsawParameter>());
	switch (_operation) {
	case SET_TARGET_POSITION:
	case SET_TARGET_ROTATION:
	case SET_CAMERA_OFFSET:
	case SET_CAMERA_ROTATION_OFFSET:
		if (i == 0) {
			return Array::make(JigsawParameterFloat::make(0.0));
		} else if (i == 1) {
			return Array::make(JigsawParameterFloat::make(0.0));
		} else if (i == 2) {
			return Array::make(JigsawParameterFloat::make(0.0));
		} else if (i == 3) {
			return Array::make(JigsawParameterFloat::make(0.0));
		}

		break;
	case SET_TARGET_POSITION_EXPRESSION:
	case SET_TARGET_ROTATION_EXPRESSION:
	case SET_CAMERA_OFFSET_EXPRESSION:
	case SET_CAMERA_ROTATION_OFFSET_EXPRESSION:
		if (i == 0) {
			return Array::make(JigsawParameterExpression::make("Vector3(0, 0, 0)", PackedStringArray{"time", "params"}));
		} else if (i == 1) {
			return Array::make(JigsawParameterOrderedList::make_template(Array::make(
				JigsawParameterFloat::make(0.0)
			)));
		}

		break;
	case SET_SIMPLE_BACKGROUND:
		if (i == 0) {
			return Array::make(JigsawParameterIcon::make(enums::IconDef::NONE));
		} else if (i == 1) {
			return Array::make(JigsawParameterBoolean::make(false));
		} else if (i == 2) {
			return Array::make(JigsawParameterBoolean::make(false));
		}

		break;
	case SET_BACKGROUND_COLOR:
		if (i == 0) {
			return Array::make(JigsawParameterColor::make(Color()));
		} else if (i == 1) {
			return Array::make(JigsawParameterFloat::make(0.0));
		}

		break;
	case SET_AMBIENT_LIGHT_INTENSITY:
		if (i == 0) {
			return Array::make(JigsawParameterFloat::make(1.0));
		} else if (i == 1) {
			return Array::make(JigsawParameterFloat::make(0.0));
		}

		break;
	}

	ERR_FAIL_V(TypedArray<JigsawParameter>());
}
void JigsawCommandCamera::set_argument(int64_t i, const Ref<JigsawParameter> &arg) {
	ERR_FAIL_INDEX(i, get_num_arguments());
	switch (_operation) {
	case SET_TARGET_POSITION:
		if (i == 0) {
			_x = arg;
			emit_changed();
			return;
		} else if (i == 1) {
			_y = arg;
			emit_changed();
			return;
		} else if (i == 2) {
			_z = arg;
			emit_changed();
			return;
		} else if (i == 3) {
			_duration = arg;
			emit_changed();
			return;
		}

		break;
	case SET_TARGET_ROTATION:
		if (i == 0) {
			_pitch = arg;
			emit_changed();
			return;
		} else if (i == 1) {
			_yaw = arg;
			emit_changed();
			return;
		} else if (i == 2) {
			_roll = arg;
			emit_changed();
			return;
		} else if (i == 3) {
			_duration = arg;
			emit_changed();
			return;
		}

		break;
	case SET_CAMERA_OFFSET:
		if (i == 0) {
			_x = arg;
			emit_changed();
			return;
		} else if (i == 1) {
			_y = arg;
			emit_changed();
			return;
		} else if (i == 2) {
			_z = arg;
			emit_changed();
			return;
		} else if (i == 3) {
			_duration = arg;
			emit_changed();
			return;
		}

		break;
	case SET_CAMERA_ROTATION_OFFSET:
		if (i == 0) {
			_pitch = arg;
			emit_changed();
			return;
		} else if (i == 1) {
			_yaw = arg;
			emit_changed();
			return;
		} else if (i == 2) {
			_roll = arg;
			emit_changed();
			return;
		} else if (i == 3) {
			_duration = arg;
			emit_changed();
			return;
		}

		break;
	case SET_TARGET_POSITION_EXPRESSION:
	case SET_TARGET_ROTATION_EXPRESSION:
	case SET_CAMERA_OFFSET_EXPRESSION:
	case SET_CAMERA_ROTATION_OFFSET_EXPRESSION:
		if (i == 0) {
			_expression = arg;
			emit_changed();
			return;
		} else if (i == 1) {
			_value = arg;
			emit_changed();
			return;
		}

		break;
	case SET_SIMPLE_BACKGROUND:
		if (i == 0) {
			_value = arg;
			emit_changed();
			return;
		} else if (i == 1) {
			_stretch = arg;
			emit_changed();
			return;
		} else if (i == 2) {
			_force = arg;
			emit_changed();
			return;
		}

		break;
	case SET_BACKGROUND_COLOR:
	case SET_AMBIENT_LIGHT_INTENSITY:
		if (i == 0) {
			_value = arg;
			emit_changed();
			return;
		} else if (i == 1) {
			_duration = arg;
			emit_changed();
			return;
		}

		break;
	}

	ERR_FAIL();
}
String JigsawCommandCamera::get_argument_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), "");
	switch (_operation) {
	case SET_TARGET_POSITION:
		if (i == 0) {
			return "x";
		} else if (i == 1) {
			return "y";
		} else if (i == 2) {
			return "z";
		} else if (i == 3) {
			return "duration";
		}

		break;
	case SET_TARGET_ROTATION:
		if (i == 0) {
			return "pitch";
		} else if (i == 1) {
			return "yaw";
		} else if (i == 2) {
			return "roll";
		} else if (i == 3) {
			return "duration";
		}

		break;
	case SET_CAMERA_OFFSET:
		if (i == 0) {
			return "x";
		} else if (i == 1) {
			return "y";
		} else if (i == 2) {
			return "z";
		} else if (i == 3) {
			return "duration";
		}

		break;
	case SET_CAMERA_ROTATION_OFFSET:
		if (i == 0) {
			return "pitch";
		} else if (i == 1) {
			return "yaw";
		} else if (i == 2) {
			return "roll";
		} else if (i == 3) {
			return "duration";
		}

		break;
	case SET_TARGET_POSITION_EXPRESSION:
	case SET_TARGET_ROTATION_EXPRESSION:
	case SET_CAMERA_OFFSET_EXPRESSION:
	case SET_CAMERA_ROTATION_OFFSET_EXPRESSION:
		if (i == 0) {
			return "function";
		} else if (i == 1) {
			return "parameters";
		}

		break;
	case SET_SIMPLE_BACKGROUND:
		if (i == 0) {
			return "texture";
		} else if (i == 1) {
			return "stretch";
		} if (i == 2) {
			return "force";
		}

		break;
	case SET_BACKGROUND_COLOR:
		if (i == 0) {
			return "color";
		} else if (i == 1) {
			return "duration";
		}

		break;
	case SET_AMBIENT_LIGHT_INTENSITY:
		if (i == 0) {
			return "intensity";
		} else if (i == 1) {
			return "duration";
		}

		break;
	}

	ERR_FAIL_V("");
}

int64_t JigsawCommandCamera::get_num_results() const {
	return 0;
}
Ref<JigsawParameterLocalVariable> JigsawCommandCamera::get_result(int64_t i) const {
	ERR_FAIL_V(Ref<JigsawParameterLocalVariable>());
}
TypedArray<JigsawParameter> JigsawCommandCamera::get_result_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_V(TypedArray<JigsawParameter>());
}
void JigsawCommandCamera::set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) {
	ERR_FAIL();
}
String JigsawCommandCamera::get_result_name(int64_t i) const {
	ERR_FAIL_V("");
}
