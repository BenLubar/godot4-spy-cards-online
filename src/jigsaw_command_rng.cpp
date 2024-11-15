#include "jigsaw_command_rng.h"

#include "jigsaw_global.h"
#include "jigsaw_parameter_amount.h"
#include "jigsaw_parameter_float.h"

void JigsawCommandRNG::_bind_methods() {
	BIND_ENUM_CONSTANT(FLOAT);
	BIND_ENUM_CONSTANT(AMOUNT);
	BIND_ENUM_CONSTANT(SCOPE);

	BIND_PROPERTY_ENUM(JigsawCommandRNG::Operation, operation);
	BIND_PROPERTY_RESOURCE(JigsawParameter, min);
	BIND_PROPERTY_RESOURCE(JigsawParameter, max);
	BIND_PROPERTY_RESOURCE(JigsawParameterLocalVariable, output);
	BIND_PROPERTY_RESOURCE(JigsawCommandList, scope);
}

IMPLEMENT_PROPERTY(JigsawCommandRNG, JigsawCommandRNG::Operation, operation);
IMPLEMENT_PROPERTY(JigsawCommandRNG, Ref<JigsawParameter>, min);
IMPLEMENT_PROPERTY(JigsawCommandRNG, Ref<JigsawParameter>, max);
IMPLEMENT_PROPERTY(JigsawCommandRNG, Ref<JigsawParameterLocalVariable>, output);
IMPLEMENT_PROPERTY(JigsawCommandRNG, Ref<JigsawCommandList>, scope);

JigsawExecutionState JigsawCommandRNG::evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const {
	Ref<::RNG> rng = context->get_global()->get_rng();
	if (rng.is_null()) {
		err = context->create_error("RNG is not available in this context");
		return JigsawExecutionState::ERROR;
	}

	switch (_operation) {
	case FLOAT:
	{
		Ref<JigsawParameterFloat> min;
		Ref<JigsawParameterFloat> max;

		err = context->resolve_variable(_min, min, "min");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		err = context->resolve_variable(_max, max, "max");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		if (unlikely(Math::is_nan(min->get_value()))) {
			err = context->create_error("minimum value is not a number");
			return JigsawExecutionState::ERROR;
		}
		if (unlikely(Math::is_nan(max->get_value()))) {
			err = context->create_error("maximum value is not a number");
			return JigsawExecutionState::ERROR;
		}
		if (unlikely(!Math::is_finite(min->get_value()))) {
			err = context->create_error("minimum value is infinite");
			return JigsawExecutionState::ERROR;
		}
		if (unlikely(!Math::is_finite(max->get_value()))) {
			err = context->create_error("maximum value is infinite");
			return JigsawExecutionState::ERROR;
		}
		if (unlikely(min->get_value() > max->get_value())) {
			err = context->create_error("minimum value is greater than maximum value");
			return JigsawExecutionState::ERROR;
		}

		return set_command_result(context, err, 0, JigsawParameterFloat::make(rng->next_range_float(min->get_value(), max->get_value())));
	}
	case AMOUNT:
	{
		Ref<JigsawParameterAmount> min;
		Ref<JigsawParameterAmount> max;

		err = context->resolve_variable(_min, min, "min");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		err = context->resolve_variable(_max, max, "max");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		if (unlikely(min->is_nan())) {
			err = context->create_error("minimum value is not a number");
			return JigsawExecutionState::ERROR;
		}
		if (unlikely(max->is_nan())) {
			err = context->create_error("maximum value is not a number");
			return JigsawExecutionState::ERROR;
		}
		if (unlikely(min->get_amount_inf() != 0)) {
			err = context->create_error("minimum value is infinite");
			return JigsawExecutionState::ERROR;
		}
		if (unlikely(max->get_amount_inf() != 0)) {
			err = context->create_error("maximum value is infinite");
			return JigsawExecutionState::ERROR;
		}
		if (unlikely(min->get_amount() > max->get_amount())) {
			err = context->create_error("minimum value is greater than maximum value");
			return JigsawExecutionState::ERROR;
		}

		return set_command_result(context, err, 0, JigsawParameterAmount::make(rng->next_range_int(min->get_amount(), max->get_amount())));
	}
	case SCOPE:
	{
		if (unlikely(_rng_saved.is_valid())) {
			err = context->create_error("cannot create RNG scope recursively with the same command instance");
			return JigsawExecutionState::ERROR;
		}

		_rng_saved = rng;
		context->get_global()->set_rng(rng->duplicate());

		err = context->append_stack_frame(_scope, 0);

		if (unlikely(err.is_valid())) {
			context->get_global()->set_rng(rng);
			_rng_saved = Ref<::RNG>();
		}

		return unlikely(err.is_valid()) ? JigsawExecutionState::ERROR : JigsawExecutionState::CONTINUE;
	}
	}

	err = context->create_error(vformat("internal error: unhandled RNG operation %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawCommandRNG", "Operation", _operation)));
	return JigsawExecutionState::ERROR;
}
Ref<JigsawError> JigsawCommandRNG::pop_stack_frame(const Ref<JigsawContext> &context, const Ref<JigsawStackFrame> &popped_frame) const {
	ERR_FAIL_COND_V(_rng_saved.is_null(), context->create_error("internal error: saved RNG is missing"));

	context->get_global()->set_rng(_rng_saved);
	_rng_saved = Ref<::RNG>();

	return Ref<JigsawError>();
}

int64_t JigsawCommandRNG::get_num_configs() const {
	return 1;
}
String JigsawCommandRNG::get_config_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "Operation";
}
String JigsawCommandRNG::get_config_desc(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	if (i == 0 && _operation == SCOPE) {
		return "Revert changes to the RNG's state once the Scope ends. The same sequence of random data will be generated inside the scope as immediately after the scope.";
	}

	return "Warning: If you do not call the random number generator in the same way for every player, the match will desync.";
}
int64_t JigsawCommandRNG::get_config_value(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, 0);

	return _operation;
}
void JigsawCommandRNG::set_config_value(int64_t i, int64_t value) {
	ERR_FAIL_INDEX(i, 1);

	_operation = static_cast<Operation>(value);
	emit_changed();
}
PackedStringArray JigsawCommandRNG::get_config_options(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, PackedStringArray());

	// If these asserts fail, you have broken compatibility with existing game modes.
	static_assert(FLOAT == 0);
	static_assert(AMOUNT == 1);
	static_assert(SCOPE == 2);

	return PackedStringArray{
		"Float",
		"Amount",
		"Scope",
	};
}

int64_t JigsawCommandRNG::get_num_arguments() const {
	switch (_operation) {
	case FLOAT:
	case AMOUNT:
		return 2;
	case SCOPE:
		return 0;
	}

	return 0;
}
Ref<JigsawParameter> JigsawCommandRNG::get_argument(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), Ref<JigsawParameter>());

	if (i == 0) {
		return _min;
	} else if (i == 1) {
		return _max;
	}

	return Ref<JigsawParameter>();
}
TypedArray<JigsawParameter> JigsawCommandRNG::get_argument_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), TypedArray<JigsawParameter>());

	switch (_operation) {
	case FLOAT:
		return Array::make(JigsawParameterFloat::make(0.0));
	case AMOUNT:
		return Array::make(JigsawParameterAmount::make(0));
	}

	return TypedArray<JigsawParameter>();
}
void JigsawCommandRNG::set_argument(int64_t i, const Ref<JigsawParameter> &arg) {
	ERR_FAIL_INDEX(i, get_num_arguments());

	if (i == 0) {
		_min = arg;
		emit_changed();
	} else if (i == 1) {
		_max = arg;
		emit_changed();
	}
}
String JigsawCommandRNG::get_argument_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), "");

	if (i == 0) {
		return "min";
	} else if (i == 1) {
		return "max";
	}

	return "";
}

int64_t JigsawCommandRNG::get_num_results() const {
	switch (_operation) {
	case FLOAT:
	case AMOUNT:
		return 1;
	case SCOPE:
		return 0;
	}

	return 0;
}
Ref<JigsawParameterLocalVariable> JigsawCommandRNG::get_result(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_results(), Ref<JigsawParameterLocalVariable>());

	return _output;
}
TypedArray<JigsawParameter> JigsawCommandRNG::get_result_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, get_num_results(), TypedArray<JigsawParameter>());

	switch (_operation) {
	case FLOAT:
		return Array::make(JigsawParameterFloat::make(0.0));
	case AMOUNT:
		return Array::make(JigsawParameterAmount::make(0));
	}

	return TypedArray<JigsawParameter>();
}
void JigsawCommandRNG::set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) {
	ERR_FAIL_INDEX(i, get_num_results());

	_output = result;
	emit_changed();
}
String JigsawCommandRNG::get_result_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_results(), "");

	return "value";
}

int64_t JigsawCommandRNG::get_num_branches() const {
	switch (_operation) {
	case FLOAT:
	case AMOUNT:
		return 0;
	case SCOPE:
		return 1;
	}

	return 0;
}
Ref<JigsawCommandList> JigsawCommandRNG::get_branch(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, Ref<JigsawCommandList>());

	return _scope;
}
void JigsawCommandRNG::set_branch(int64_t i, const Ref<JigsawCommandList> &commands) {
	ERR_FAIL_INDEX(i, 1);

	_scope = commands;
	emit_changed();
}
String JigsawCommandRNG::get_branch_name(int64_t i) const {
	return "Alternate timeline";
}
