#include "jigsaw/command/jigsaw_command_color.h"

#include "jigsaw/parameter/jigsaw_parameter_amount.h"
#include "jigsaw/parameter/jigsaw_parameter_color.h"
#include "jigsaw/parameter/jigsaw_parameter_float.h"
#include "jigsaw/parameter/jigsaw_parameter_string.h"

void JigsawCommandColor::_bind_methods() {
	BIND_ENUM_CONSTANT(CREATE_FLOAT);
	BIND_ENUM_CONSTANT(CREATE_AMOUNT);
	BIND_ENUM_CONSTANT(CREATE_STRING);

	BIND_PROPERTY_ENUM(JigsawCommandColor::Operation, operation);
	BIND_PROPERTY_RESOURCE(JigsawParameter, red);
	BIND_PROPERTY_RESOURCE(JigsawParameter, green);
	BIND_PROPERTY_RESOURCE(JigsawParameter, blue);
	BIND_PROPERTY_RESOURCE(JigsawParameter, alpha);
	BIND_PROPERTY_RESOURCE(JigsawParameterLocalVariable, output);
}

IMPLEMENT_PROPERTY(JigsawCommandColor, JigsawCommandColor::Operation, operation);
IMPLEMENT_PROPERTY(JigsawCommandColor, Ref<JigsawParameter>, red);
IMPLEMENT_PROPERTY(JigsawCommandColor, Ref<JigsawParameter>, green);
IMPLEMENT_PROPERTY(JigsawCommandColor, Ref<JigsawParameter>, blue);
IMPLEMENT_PROPERTY(JigsawCommandColor, Ref<JigsawParameter>, alpha);
IMPLEMENT_PROPERTY(JigsawCommandColor, Ref<JigsawParameterLocalVariable>, output);

static float amount255_to_float1(const Ref<JigsawParameterAmount> &amount) {
	if (amount->is_nan()) {
		return NAN;
	}

	if (amount->get_amount_inf() < 0) {
		return 0.0f;
	}
	if (amount->get_amount_inf() > 0) {
		return 1.0f;
	}

	return CLAMP(amount->get_amount(), 0, 255) / 255.0f;
}

bool JigsawCommandColor::allowed_in_mode(enums::JigsawProcedure::Mode mode, bool any_config) const {
	return true;
}
JigsawExecutionState JigsawCommandColor::evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const {
	switch (_operation) {
	case CREATE_FLOAT:
	{
#define CHECK_NAN(m_value, m_name) \
		if (unlikely(Math::is_nan((m_value)))) { \
			err = context->create_error(vformat("color %s component is not a number", m_name)); \
			return JigsawExecutionState::ERROR; \
		}

		Ref<JigsawParameterFloat> red, green, blue, alpha;
		err = context->resolve_variable(_red, red, "red");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		err = context->resolve_variable(_green, green, "green");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		err = context->resolve_variable(_blue, blue, "blue");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		err = context->resolve_variable(_alpha, alpha, "alpha");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		CHECK_NAN(red->get_value(), "red");
		CHECK_NAN(green->get_value(), "green");
		CHECK_NAN(blue->get_value(), "blue");
		CHECK_NAN(alpha->get_value(), "alpha");

		return set_command_result(context, err, 0, JigsawParameterColor::make(Color(red->get_value(), green->get_value(), blue->get_value(), alpha->get_value())));
	}
	case CREATE_AMOUNT:
	{
		Ref<JigsawParameterAmount> red, green, blue, alpha;
		err = context->resolve_variable(_red, red, "red");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		err = context->resolve_variable(_green, green, "green");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		err = context->resolve_variable(_blue, blue, "blue");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		err = context->resolve_variable(_alpha, alpha, "alpha");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		float r = amount255_to_float1(red);
		float g = amount255_to_float1(green);
		float b = amount255_to_float1(blue);
		float a = amount255_to_float1(alpha);

		CHECK_NAN(r, "red");
		CHECK_NAN(g, "green");
		CHECK_NAN(b, "blue");
		CHECK_NAN(a, "alpha");

		return set_command_result(context, err, 0, JigsawParameterColor::make(Color(r, g, b, a)));
	}
#undef CHECK_NAN
	case CREATE_STRING:
	{
		Ref<JigsawParameterString> alpha;
		err = context->resolve_variable(_alpha, alpha, "alpha");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		if (!Color::html_is_valid(alpha->get_string())) {
			err = context->create_error("invalid hex color code", Array::make(alpha));
			return JigsawExecutionState::ERROR;
		}

		return set_command_result(context, err, 0, JigsawParameterColor::make(Color(alpha->get_string())));
	}
	}

	err = context->create_error(vformat("internal error: unhandled color operation %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawCommandColor", "Operation", _operation)));
	return JigsawExecutionState::ERROR;
}

int64_t JigsawCommandColor::get_num_configs() const {
	return 1;
}
String JigsawCommandColor::get_config_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "Operation";
}
String JigsawCommandColor::get_config_desc(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "";
}
int64_t JigsawCommandColor::get_config_value(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, 0);

	return _operation;
}
void JigsawCommandColor::set_config_value(int64_t i, int64_t value) {
	ERR_FAIL_INDEX(i, 1);

	_operation = static_cast<Operation>(value);
	emit_changed();
}
PackedStringArray JigsawCommandColor::get_config_options(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, PackedStringArray());

	// If these asserts fail, you have broken compatibility with existing game modes.
	static_assert(CREATE_FLOAT == 0);
	static_assert(CREATE_AMOUNT == 1);
	static_assert(CREATE_STRING == 2);

	return PackedStringArray{
		"Create (float between 0 and 1)",
		"Create (amount between 0 and 255)",
		"Create (hexadecimal string)",
	};
}

int64_t JigsawCommandColor::get_num_arguments() const {
	switch (_operation) {
	case CREATE_FLOAT:
	case CREATE_AMOUNT:
		return 4;
	case CREATE_STRING:
		return 1;
	}

	ERR_FAIL_V(0);
}
Ref<JigsawParameter> JigsawCommandColor::get_argument(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), Ref<JigsawParameter>());

	switch (_operation) {
	case CREATE_FLOAT:
	case CREATE_AMOUNT:
		if (i == 0) {
			return _red;
		} else if (i == 1) {
			return _green;
		} else if (i == 2) {
			return _blue;
		} else if (i == 3) {
			return _alpha;
		}

		break;
	case CREATE_STRING:
		if (i == 0) {
			return _alpha;
		}

		break;
	}

	ERR_FAIL_V(Ref<JigsawParameter>());
}
TypedArray<JigsawParameter> JigsawCommandColor::get_argument_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), TypedArray<JigsawParameter>());

	switch (_operation) {
	case CREATE_FLOAT:
		return Array::make(JigsawParameterFloat::make(0.0));
	case CREATE_AMOUNT:
		return Array::make(JigsawParameterAmount::make(0));
	case CREATE_STRING:
		return Array::make(JigsawParameterString::make("#000000"));
	}

	return TypedArray<JigsawParameter>();
}
void JigsawCommandColor::set_argument(int64_t i, const Ref<JigsawParameter> &arg) {
	ERR_FAIL_INDEX(i, get_num_arguments());

	switch (_operation) {
	case CREATE_FLOAT:
	case CREATE_AMOUNT:
		if (i == 0) {
			_red = arg;
			emit_changed();
			return;
		} else if (i == 1) {
			_green = arg;
			emit_changed();
			return;
		} else if (i == 2) {
			_blue = arg;
			emit_changed();
			return;
		} else if (i == 3) {
			_alpha = arg;
			emit_changed();
			return;
		}

		break;
	case CREATE_STRING:
		if (i == 0) {
			_alpha = arg;
			emit_changed();
			return;
		}

		break;
	}

	ERR_FAIL();
}
String JigsawCommandColor::get_argument_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), "");

	switch (_operation) {
	case CREATE_FLOAT:
	case CREATE_AMOUNT:
		if (i == 0) {
			return "red";
		} else if (i == 1) {
			return "green";
		} else if (i == 2) {
			return "blue";
		} else if (i == 3) {
			return "alpha";
		}

		break;
	case CREATE_STRING:
		if (i == 0) {
			return "code";
		}

		break;
	}

	ERR_FAIL_V("");
}

int64_t JigsawCommandColor::get_num_results() const {
	return 1;
}
Ref<JigsawParameterLocalVariable> JigsawCommandColor::get_result(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_results(), Ref<JigsawParameterLocalVariable>());

	return _output;
}
TypedArray<JigsawParameter> JigsawCommandColor::get_result_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, get_num_results(), TypedArray<JigsawParameter>());

	return Array::make(JigsawParameterColor::make(Color()));
}
void JigsawCommandColor::set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) {
	ERR_FAIL_INDEX(i, get_num_results());

	_output = result;
	emit_changed();
}
String JigsawCommandColor::get_result_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_results(), "");

	return "color";
}
