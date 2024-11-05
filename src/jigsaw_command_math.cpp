#include "jigsaw_command_math.h"

#include "jigsaw_parameter_amount.h"
#include "jigsaw_parameter_boolean.h"
#include "jigsaw_parameter_float.h"

void JigsawCommandMath::_bind_methods() {
	BIND_ENUM_CONSTANT(EQUALS);
	BIND_ENUM_CONSTANT(LESS_THAN);

	BIND_PROPERTY_ENUM(JigsawCommandMath::Operation, op);
	BIND_PROPERTY_RESOURCE(JigsawParameter, lhs);
	BIND_PROPERTY_RESOURCE(JigsawParameter, rhs);
	BIND_PROPERTY_RESOURCE(JigsawParameterLocalVariable, output);
}

JigsawExecutionState JigsawCommandMath::set_boolean_result(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool b) const {
	err = context->set_local_variable(_output, JigsawParameterBoolean::make(b), "output");
	return likely(err.is_null()) ? JigsawExecutionState::CONTINUE : JigsawExecutionState::ERROR;
}

IMPLEMENT_PROPERTY(JigsawCommandMath, JigsawCommandMath::Operation, op);
IMPLEMENT_PROPERTY(JigsawCommandMath, Ref<JigsawParameter>, lhs);
IMPLEMENT_PROPERTY(JigsawCommandMath, Ref<JigsawParameter>, rhs);
IMPLEMENT_PROPERTY(JigsawCommandMath, Ref<JigsawParameterLocalVariable>, output);

JigsawExecutionState JigsawCommandMath::evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const {
	switch (_op) {
	case EQUALS:
	{
		Ref<JigsawParameterAmount> lhs, rhs;
		err = context->resolve_variable(_lhs, lhs, "lhs");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		err = context->resolve_variable(_rhs, rhs, "rhs");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		if (lhs->is_nan() || rhs->is_nan()) {
			return set_boolean_result(context, err, false);
		} else if (lhs->get_amount_inf() > 0) {
			return set_boolean_result(context, err, rhs->get_amount_inf() > 0);
		} else if (lhs->get_amount_inf() < 0) {
			return set_boolean_result(context, err, rhs->get_amount_inf() < 0);
		} else if (rhs->get_amount_inf() != 0) {
			return set_boolean_result(context, err, false);
		} else {
			return set_boolean_result(context, err, lhs->get_amount() == rhs->get_amount());
		}
	}
	case LESS_THAN:
	{
		Ref<JigsawParameterAmount> lhs, rhs;
		err = context->resolve_variable(_lhs, lhs, "lhs");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		err = context->resolve_variable(_rhs, rhs, "rhs");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		if (lhs->is_nan() || rhs->is_nan()) {
			return set_boolean_result(context, err, false);
		} else if (lhs->get_amount_inf() > 0) {
			return set_boolean_result(context, err, false);
		} else if (lhs->get_amount_inf() < 0) {
			return set_boolean_result(context, err, rhs->get_amount_inf() >= 0);
		} else if (rhs->get_amount_inf() != 0) {
			return set_boolean_result(context, err, rhs->get_amount_inf() > 0);
		} else {
			return set_boolean_result(context, err, lhs->get_amount() < rhs->get_amount());
		}
	}
	}

	err = context->create_error(vformat("internal error: unhandled math operation %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawCommandMath", "Operation", _op)));
	return JigsawExecutionState::ERROR;
}

int64_t JigsawCommandMath::get_num_configs() const {
	return 1;
}
String JigsawCommandMath::get_config_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "Operation";
}
String JigsawCommandMath::get_config_desc(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "";
}
int64_t JigsawCommandMath::get_config_value(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, 0);

	return _op;
}
void JigsawCommandMath::set_config_value(int64_t i, int64_t value) {
	ERR_FAIL_INDEX(i, 1);

	_op = static_cast<Operation>(value);
	emit_changed();
}
PackedStringArray JigsawCommandMath::get_config_options(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, PackedStringArray());

	// If these asserts fail, you have broken compatibility with existing game modes.
	static_assert(EQUALS == 0);
	static_assert(LESS_THAN == 1);

	return PackedStringArray{
		"Equals",
		"Less than",
	};
}

int64_t JigsawCommandMath::get_num_arguments() const {
	switch (_op) {
	case EQUALS:
	case LESS_THAN:
		return 2;
	}

	return 0;
}
Ref<JigsawParameter> JigsawCommandMath::get_argument(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), Ref<JigsawParameter>());

	if (i == 0) {
		return _lhs;
	} else if (i == 1) {
		return _rhs;
	}

	return Ref<JigsawParameter>();
}
TypedArray<JigsawParameter> JigsawCommandMath::get_argument_template(int64_t i, const Ref<JigsawContext> &context) const {
	return Array::make(JigsawParameterAmount::make(0));
}
void JigsawCommandMath::set_argument(int64_t i, const Ref<JigsawParameter> &arg) {
	ERR_FAIL_INDEX(i, get_num_arguments());

	if (i == 0) {
		_lhs = arg;
		emit_changed();
	} else if (i == 1) {
		_rhs = arg;
		emit_changed();
	}
}
String JigsawCommandMath::get_argument_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), "");

	if (i == 0) {
		return get_num_arguments() == 1 ? "value" : "lhs";
	} else if (i == 1) {
		return "rhs";
	}

	return "";
}

int64_t JigsawCommandMath::get_num_results() const {
	return 1;
}
Ref<JigsawParameterLocalVariable> JigsawCommandMath::get_result(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, Ref<JigsawParameterLocalVariable>());

	return _output;
}
TypedArray<JigsawParameter> JigsawCommandMath::get_result_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, 1, TypedArray<JigsawParameter>());

	switch (_op) {
	case EQUALS:
	case LESS_THAN:
		return Array::make(JigsawParameterBoolean::make(false));
	}

	return TypedArray<JigsawParameter>();
}
void JigsawCommandMath::set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) {
	ERR_FAIL_INDEX(i, 1);

	_output = result;
	emit_changed();
}
String JigsawCommandMath::get_result_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "result";
}
