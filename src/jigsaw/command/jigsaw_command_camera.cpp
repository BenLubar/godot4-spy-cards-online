#include "jigsaw/command/jigsaw_command_camera.h"

#include <godot_cpp/classes/environment.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>

#include "jigsaw/jigsaw_global.h"
#include "jigsaw/jigsaw_visual.h"
#include "jigsaw/parameter/jigsaw_parameter_color.h"
#include "jigsaw/parameter/jigsaw_parameter_float.h"

void JigsawCommandCamera::_bind_methods() {
	BIND_ENUM_CONSTANT(RESERVED0);
	BIND_ENUM_CONSTANT(RESERVED1);
	BIND_ENUM_CONSTANT(RESERVED2);
	BIND_ENUM_CONSTANT(RESERVED3);
	BIND_ENUM_CONSTANT(RESERVED4);
	BIND_ENUM_CONSTANT(RESERVED5);
	BIND_ENUM_CONSTANT(RESERVED6);
	BIND_ENUM_CONSTANT(RESERVED7);
	BIND_ENUM_CONSTANT(RESERVED8);
	BIND_ENUM_CONSTANT(RESERVED9);
	BIND_ENUM_CONSTANT(SET_BACKGROUND_COLOR);
	BIND_ENUM_CONSTANT(SET_AMBIENT_LIGHT_INTENSITY);

	BIND_PROPERTY_ENUM(JigsawCommandCamera::Operation, operation);
	BIND_PROPERTY_RESOURCE(JigsawParameter, value);
}

IMPLEMENT_PROPERTY(JigsawCommandCamera, JigsawCommandCamera::Operation, operation);
IMPLEMENT_PROPERTY(JigsawCommandCamera, Ref<JigsawParameter>, value);

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
		return true;
	case REALTIME_LOGIC:
	case REALTIME_VISUAL:
		return false; // TODO: rollback support
	}

	ERR_FAIL_V(false);
}
JigsawExecutionState JigsawCommandCamera::evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const {
	switch (_operation) {
	case RESERVED0:
	case RESERVED1:
	case RESERVED2:
	case RESERVED3:
	case RESERVED4:
	case RESERVED5:
	case RESERVED6:
	case RESERVED7:
	case RESERVED8:
	case RESERVED9:
		break;
	case SET_BACKGROUND_COLOR:
	{
		Ref<JigsawParameterColor> color;
		err = context->resolve_variable(_value, color, "color");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		Ref<Environment> environment = context->get_global()->get_visual()->get_stage_viewport()->get_world_3d()->get_environment();
		if (color->get_color().a <= 0.0f) {
			environment->set_background(Environment::BG_CLEAR_COLOR);
		} else {
			environment->set_background(Environment::BG_COLOR);
			environment->set_bg_color(color->get_color());
		}

		return JigsawExecutionState::DONE;
	}
	case SET_AMBIENT_LIGHT_INTENSITY:
	{
		Ref<JigsawParameterFloat> intensity;
		err = context->resolve_variable(_value, intensity, "intensity");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		if (unlikely(!Math::is_finite(intensity->get_value()))) {
			err = context->create_error(vformat("cannot set ambient light intensity to %f", intensity->get_value()));
			return JigsawExecutionState::ERROR;
		}

		Ref<Environment> environment = context->get_global()->get_visual()->get_stage_viewport()->get_world_3d()->get_environment();
		environment->set_ambient_light_sky_contribution(intensity->get_value());

		return JigsawExecutionState::DONE;
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
	static_assert(RESERVED0 == 0);
	static_assert(RESERVED1 == 1);
	static_assert(RESERVED2 == 2);
	static_assert(RESERVED3 == 3);
	static_assert(RESERVED4 == 4);
	static_assert(RESERVED5 == 5);
	static_assert(RESERVED6 == 6);
	static_assert(RESERVED7 == 7);
	static_assert(RESERVED8 == 8);
	static_assert(RESERVED9 == 9);
	static_assert(SET_BACKGROUND_COLOR == 10);
	static_assert(SET_AMBIENT_LIGHT_INTENSITY == 11);

	return PackedStringArray{
		"RESERVED0",
		"RESERVED1",
		"RESERVED2",
		"RESERVED3",
		"RESERVED4",
		"RESERVED5",
		"RESERVED6",
		"RESERVED7",
		"RESERVED8",
		"RESERVED9",
		"Set background color",
		"Set ambient light intensity",
	};
}

int64_t JigsawCommandCamera::get_num_arguments() const {
	switch (_operation) {
	case RESERVED0:
	case RESERVED1:
	case RESERVED2:
	case RESERVED3:
	case RESERVED4:
	case RESERVED5:
	case RESERVED6:
	case RESERVED7:
	case RESERVED8:
	case RESERVED9:
		break;
	case SET_BACKGROUND_COLOR:
	case SET_AMBIENT_LIGHT_INTENSITY:
		return 1;
	}

	ERR_FAIL_V(0);
}
Ref<JigsawParameter> JigsawCommandCamera::get_argument(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), Ref<JigsawParameter>());
	switch (_operation) {
	case RESERVED0:
	case RESERVED1:
	case RESERVED2:
	case RESERVED3:
	case RESERVED4:
	case RESERVED5:
	case RESERVED6:
	case RESERVED7:
	case RESERVED8:
	case RESERVED9:
		break;
	case SET_BACKGROUND_COLOR:
	case SET_AMBIENT_LIGHT_INTENSITY:
		if (i == 0) {
			return _value;
		}

		break;
	}

	ERR_FAIL_V(Ref<JigsawParameter>());
}
TypedArray<JigsawParameter> JigsawCommandCamera::get_argument_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), TypedArray<JigsawParameter>());
	switch (_operation) {
	case RESERVED0:
	case RESERVED1:
	case RESERVED2:
	case RESERVED3:
	case RESERVED4:
	case RESERVED5:
	case RESERVED6:
	case RESERVED7:
	case RESERVED8:
	case RESERVED9:
		break;
	case SET_BACKGROUND_COLOR:
		if (i == 0) {
			return Array::make(JigsawParameterColor::make(Color()));
		}

		break;
	case SET_AMBIENT_LIGHT_INTENSITY:
		if (i == 0) {
			return Array::make(JigsawParameterFloat::make(1.0f));
		}

		break;
	}

	ERR_FAIL_V(TypedArray<JigsawParameter>());
}
void JigsawCommandCamera::set_argument(int64_t i, const Ref<JigsawParameter> &arg) {
	ERR_FAIL_INDEX(i, get_num_arguments());
	switch (_operation) {
	case RESERVED0:
	case RESERVED1:
	case RESERVED2:
	case RESERVED3:
	case RESERVED4:
	case RESERVED5:
	case RESERVED6:
	case RESERVED7:
	case RESERVED8:
	case RESERVED9:
		break;
	case SET_BACKGROUND_COLOR:
	case SET_AMBIENT_LIGHT_INTENSITY:
		if (i == 0) {
			_value = arg;
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
	case RESERVED0:
	case RESERVED1:
	case RESERVED2:
	case RESERVED3:
	case RESERVED4:
	case RESERVED5:
	case RESERVED6:
	case RESERVED7:
	case RESERVED8:
	case RESERVED9:
		break;
	case SET_BACKGROUND_COLOR:
		if (i == 0) {
			return "color";
		}

		break;
	case SET_AMBIENT_LIGHT_INTENSITY:
		if (i == 0) {
			return "intensity";
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
