#include "jigsaw_command_math.h"

#include "jigsaw_parameter_amount.h"
#include "jigsaw_parameter_boolean.h"
#include "jigsaw_parameter_float.h"

void JigsawCommandMath::_bind_methods() {
	BIND_ENUM_CONSTANT(EQUALS);
	BIND_ENUM_CONSTANT(LESS_THAN);
	BIND_ENUM_CONSTANT(ADD);
	BIND_ENUM_CONSTANT(SUBTRACT);
	BIND_ENUM_CONSTANT(MULTIPLY);
	BIND_ENUM_CONSTANT(DIVIDE);
	BIND_ENUM_CONSTANT(REMAINDER);
	BIND_ENUM_CONSTANT(AMOUNT_TO_FLOAT);
	BIND_ENUM_CONSTANT(FLOOR);
	BIND_ENUM_CONSTANT(CEIL);
	BIND_ENUM_CONSTANT(FLOAT_LESS_THAN);
	BIND_ENUM_CONSTANT(FLOAT_ADD);
	BIND_ENUM_CONSTANT(FLOAT_SUBTRACT);
	BIND_ENUM_CONSTANT(FLOAT_MULTIPLY);
	BIND_ENUM_CONSTANT(FLOAT_DIVIDE);
	BIND_ENUM_CONSTANT(FLOAT_REMAINDER);
	BIND_ENUM_CONSTANT(POW);
	BIND_ENUM_CONSTANT(SIN);
	BIND_ENUM_CONSTANT(COS);
	BIND_ENUM_CONSTANT(TAN);
	BIND_ENUM_CONSTANT(ATAN);
	BIND_ENUM_CONSTANT(ATAN2);

	BIND_PROPERTY_ENUM(JigsawCommandMath::Operation, op);
	BIND_PROPERTY_RESOURCE(JigsawParameter, lhs);
	BIND_PROPERTY_RESOURCE(JigsawParameter, rhs);
	BIND_PROPERTY_RESOURCE(JigsawParameterLocalVariable, output);
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
			return set_command_result(context, err, 0, JigsawParameterBoolean::make(false));
		} else if (lhs->get_amount_inf() > 0) {
			return set_command_result(context, err, 0, JigsawParameterBoolean::make(rhs->get_amount_inf() > 0));
		} else if (lhs->get_amount_inf() < 0) {
			return set_command_result(context, err, 0, JigsawParameterBoolean::make(rhs->get_amount_inf() < 0));
		} else if (rhs->get_amount_inf() != 0) {
			return set_command_result(context, err, 0, JigsawParameterBoolean::make(false));
		} else {
			return set_command_result(context, err, 0, JigsawParameterBoolean::make(lhs->get_amount() == rhs->get_amount()));
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
			return set_command_result(context, err, 0, JigsawParameterBoolean::make(false));
		} else if (lhs->get_amount_inf() > 0) {
			return set_command_result(context, err, 0, JigsawParameterBoolean::make(false));
		} else if (lhs->get_amount_inf() < 0) {
			return set_command_result(context, err, 0, JigsawParameterBoolean::make(rhs->get_amount_inf() >= 0));
		} else if (rhs->get_amount_inf() != 0) {
			return set_command_result(context, err, 0, JigsawParameterBoolean::make(rhs->get_amount_inf() > 0));
		} else {
			return set_command_result(context, err, 0, JigsawParameterBoolean::make(lhs->get_amount() < rhs->get_amount()));
		}
	}
	case ADD:
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

		if (lhs->is_nan() || rhs->is_nan() || (lhs->get_amount_inf() < 0 && rhs->get_amount_inf() > 0) || (lhs->get_amount_inf() > 0 && rhs->get_amount_inf() < 0)) {
			return set_command_result(context, err, 0, JigsawParameterAmount::make_nan());
		} else {
			int64_t amount;
			int64_t amount_inf;
#ifdef __GNUC__
			bool amount_overflow = __builtin_add_overflow(lhs->get_amount(), rhs->get_amount(), &amount);
			bool amount_inf_overflow = __builtin_add_overflow(lhs->get_amount_inf(), rhs->get_amount_inf(), &amount_inf);
#else
			amount = lhs->get_amount() + rhs->get_amount();
			amount_inf = lhs->get_amount_inf() + rhs->get_amount_inf();
			// TODO
			bool amount_overflow = false, amount_inf_overflow = false;
#endif
			if (unlikely(amount_overflow || amount_inf_overflow)) {
				return set_command_result(context, err, 0, JigsawParameterAmount::make_nan());
			} else {
				return set_command_result(context, err, 0, JigsawParameterAmount::make(amount, amount_inf));
			}
		}
	}
	case SUBTRACT:
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

		if (lhs->is_nan() || rhs->is_nan() || (lhs->get_amount_inf() < 0 && rhs->get_amount_inf() < 0) || (lhs->get_amount_inf() > 0 && rhs->get_amount_inf() > 0)) {
			return set_command_result(context, err, 0, JigsawParameterAmount::make_nan());
		} else {
			int64_t amount;
			int64_t amount_inf;
#ifdef __GNUC__
			bool amount_overflow = __builtin_sub_overflow(lhs->get_amount(), rhs->get_amount(), &amount);
			bool amount_inf_overflow = __builtin_sub_overflow(lhs->get_amount_inf(), rhs->get_amount_inf(), &amount_inf);
#else
			amount = lhs->get_amount() - rhs->get_amount();
			amount_inf = lhs->get_amount_inf() - rhs->get_amount_inf();
			// TODO
			bool amount_overflow = false, amount_inf_overflow = false;
#endif
			if (unlikely(amount_overflow || amount_inf_overflow)) {
				return set_command_result(context, err, 0, JigsawParameterAmount::make_nan());
			} else {
				return set_command_result(context, err, 0, JigsawParameterAmount::make(amount, amount_inf));
			}
		}
	}
	case MULTIPLY:
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

		if (lhs->is_nan() || rhs->is_nan() || (lhs->get_amount() == 0 && lhs->get_amount_inf() == 0 && rhs->get_amount_inf() != 0) || (rhs->get_amount() == 0 && rhs->get_amount_inf() == 0 && lhs->get_amount_inf() != 0)) {
			return set_command_result(context, err, 0, JigsawParameterAmount::make_nan());
		} else {
			int64_t amount;
#ifdef __GNUC__
			bool amount_overflow = __builtin_sub_overflow(lhs->get_amount(), rhs->get_amount(), &amount);
#else
			amount = lhs->get_amount() * rhs->get_amount();
			// TODO
			bool amount_overflow = false;
#endif
			// just figure out the sign of the infinite part and throw away the magnitude
			int64_t amount_inf_a = Math::sign(lhs->get_amount_inf()) * Math::sign(rhs->get_amount_inf());
			int64_t amount_inf_b = Math::sign(lhs->get_amount()) * Math::sign(rhs->get_amount_inf());
			int64_t amount_inf_c = Math::sign(lhs->get_amount_inf()) * Math::sign(rhs->get_amount());
			int64_t amount_inf_min = Math::min(amount_inf_a, Math::min(amount_inf_b, amount_inf_c));
			int64_t amount_inf_max = Math::max(amount_inf_a, Math::max(amount_inf_b, amount_inf_c));
			if (unlikely(amount_overflow || (amount_inf_min < 0 && amount_inf_max > 0))) {
				return set_command_result(context, err, 0, JigsawParameterAmount::make_nan());
			} else {
				return set_command_result(context, err, 0, JigsawParameterAmount::make(amount, Math::sign(amount_inf_min + amount_inf_max)));
			}
		}
	}
	case DIVIDE:
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

		if (lhs->is_nan() || rhs->is_nan() || (rhs->get_amount() == 0 && rhs->get_amount_inf() == 0) || (lhs->get_amount_inf() != 0 && rhs->get_amount_inf() != 0) || (lhs->get_amount() == INT64_MIN && lhs->get_amount_inf() == 0 && rhs->get_amount() == -1 && rhs->get_amount_inf() == 0) || (lhs->get_amount_inf() != 0 && rhs->get_amount_inf() != 0)) {
			return set_command_result(context, err, 0, JigsawParameterAmount::make_nan());
		} else if (lhs->get_amount_inf() != 0) {
			return set_command_result(context, err, 0, JigsawParameterAmount::make(0, Math::sign(lhs->get_amount_inf()) * Math::sign(rhs->get_amount())));
		} else if (rhs->get_amount_inf() != 0) {
			return set_command_result(context, err, 0, JigsawParameterAmount::make(0));
		} else {
			return set_command_result(context, err, 0, JigsawParameterAmount::make(lhs->get_amount() / rhs->get_amount()));
		}
	}
	case REMAINDER:
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

		if (lhs->is_nan() || rhs->is_nan() || (rhs->get_amount() == 0 && rhs->get_amount_inf() == 0) || (lhs->get_amount_inf() != 0 && rhs->get_amount_inf() != 0) || (lhs->get_amount() == INT64_MIN && rhs->get_amount() == -1 && rhs->get_amount_inf() == 0) || (lhs->get_amount_inf() != 0)) {
			return set_command_result(context, err, 0, JigsawParameterAmount::make_nan());
		} else if (rhs->get_amount_inf() != 0) {
			return set_command_result(context, err, 0, lhs);
		} else {
			return set_command_result(context, err, 0, JigsawParameterAmount::make(lhs->get_amount() % rhs->get_amount()));
		}
	}
	case AMOUNT_TO_FLOAT:
	{
		Ref<JigsawParameterAmount> value;
		err = context->resolve_variable(_lhs, value, "value");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		if (value->is_nan()) {
			return set_command_result(context, err, 0, JigsawParameterFloat::make(NAN));
		} else if (value->get_amount_inf() != 0) {
			return set_command_result(context, err, 0, JigsawParameterFloat::make(value->get_amount_inf() * INFINITY));
		} else {
			return set_command_result(context, err, 0, JigsawParameterFloat::make(value->get_amount()));
		}
	}
	case FLOOR:
	{
		Ref<JigsawParameterFloat> value;
		err = context->resolve_variable(_lhs, value, "value");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		if (Math::is_nan(value->get_value())) {
			return set_command_result(context, err, 0, JigsawParameterAmount::make_nan());
		} else if (Math::is_inf(value->get_value())) {
			return set_command_result(context, err, 0, JigsawParameterAmount::make(0, Math::sign(value->get_value())));
		} else {
			return set_command_result(context, err, 0, JigsawParameterAmount::make(static_cast<int64_t>(Math::floor(value->get_value()))));
		}
	}
	case CEIL:
	{
		Ref<JigsawParameterFloat> value;
		err = context->resolve_variable(_lhs, value, "value");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		if (Math::is_nan(value->get_value())) {
			return set_command_result(context, err, 0, JigsawParameterAmount::make_nan());
		} else if (Math::is_inf(value->get_value())) {
			return set_command_result(context, err, 0, JigsawParameterAmount::make(0, Math::sign(value->get_value())));
		} else {
			return set_command_result(context, err, 0, JigsawParameterAmount::make(static_cast<int64_t>(Math::ceil(value->get_value()))));
		}
	}
	case FLOAT_LESS_THAN:
	{
		Ref<JigsawParameterFloat> lhs, rhs;
		err = context->resolve_variable(_lhs, lhs, "lhs");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		err = context->resolve_variable(_rhs, rhs, "rhs");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		return set_command_result(context, err, 0, JigsawParameterBoolean::make(lhs->get_value() < rhs->get_value()));
	}
	case FLOAT_ADD:
	{
		Ref<JigsawParameterFloat> lhs, rhs;
		err = context->resolve_variable(_lhs, lhs, "lhs");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		err = context->resolve_variable(_rhs, rhs, "rhs");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		return set_command_result(context, err, 0, JigsawParameterFloat::make(lhs->get_value() + rhs->get_value()));
	}
	case FLOAT_SUBTRACT:
	{
		Ref<JigsawParameterFloat> lhs, rhs;
		err = context->resolve_variable(_lhs, lhs, "lhs");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		err = context->resolve_variable(_rhs, rhs, "rhs");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		return set_command_result(context, err, 0, JigsawParameterFloat::make(lhs->get_value() - rhs->get_value()));
	}
	case FLOAT_MULTIPLY:
	{
		Ref<JigsawParameterFloat> lhs, rhs;
		err = context->resolve_variable(_lhs, lhs, "lhs");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		err = context->resolve_variable(_rhs, rhs, "rhs");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		return set_command_result(context, err, 0, JigsawParameterFloat::make(lhs->get_value() * rhs->get_value()));
	}
	case FLOAT_DIVIDE:
	{
		Ref<JigsawParameterFloat> lhs, rhs;
		err = context->resolve_variable(_lhs, lhs, "lhs");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		err = context->resolve_variable(_rhs, rhs, "rhs");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		return set_command_result(context, err, 0, JigsawParameterFloat::make(lhs->get_value() / rhs->get_value()));
	}
	case FLOAT_REMAINDER:
	{
		Ref<JigsawParameterFloat> lhs, rhs;
		err = context->resolve_variable(_lhs, lhs, "lhs");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		err = context->resolve_variable(_rhs, rhs, "rhs");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		return set_command_result(context, err, 0, JigsawParameterFloat::make(Math::fposmod(lhs->get_value(), rhs->get_value())));
	}
	case POW:
	{
		Ref<JigsawParameterFloat> lhs, rhs;
		err = context->resolve_variable(_lhs, lhs, "lhs");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		err = context->resolve_variable(_rhs, rhs, "rhs");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		return set_command_result(context, err, 0, JigsawParameterFloat::make(Math::pow(lhs->get_value(), rhs->get_value())));
	}
	case SIN:
	{
		Ref<JigsawParameterFloat> value;
		err = context->resolve_variable(_lhs, value, "value");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		return set_command_result(context, err, 0, JigsawParameterFloat::make(Math::sin(value->get_value())));
	}
	case COS:
	{
		Ref<JigsawParameterFloat> value;
		err = context->resolve_variable(_lhs, value, "value");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		return set_command_result(context, err, 0, JigsawParameterFloat::make(Math::cos(value->get_value())));
	}
	case TAN:
	{
		Ref<JigsawParameterFloat> value;
		err = context->resolve_variable(_lhs, value, "value");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		return set_command_result(context, err, 0, JigsawParameterFloat::make(Math::tan(value->get_value())));
	}
	case ATAN:
	{
		Ref<JigsawParameterFloat> value;
		err = context->resolve_variable(_lhs, value, "value");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		return set_command_result(context, err, 0, JigsawParameterFloat::make(Math::atan(value->get_value())));
	}
	case ATAN2:
	{
		Ref<JigsawParameterFloat> lhs, rhs;
		err = context->resolve_variable(_lhs, lhs, "lhs");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		err = context->resolve_variable(_rhs, rhs, "rhs");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		return set_command_result(context, err, 0, JigsawParameterFloat::make(Math::atan2(lhs->get_value(), rhs->get_value())));
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
	static_assert(ADD == 2);
	static_assert(SUBTRACT == 3);
	static_assert(MULTIPLY == 4);
	static_assert(DIVIDE == 5);
	static_assert(REMAINDER == 6);
	static_assert(AMOUNT_TO_FLOAT == 7);
	static_assert(FLOOR == 8);
	static_assert(CEIL == 9);
	static_assert(FLOAT_LESS_THAN == 10);
	static_assert(FLOAT_ADD == 11);
	static_assert(FLOAT_SUBTRACT == 12);
	static_assert(FLOAT_MULTIPLY == 13);
	static_assert(FLOAT_DIVIDE == 14);
	static_assert(FLOAT_REMAINDER == 15);
	static_assert(POW == 16);
	static_assert(SIN == 17);
	static_assert(COS == 18);
	static_assert(TAN == 19);
	static_assert(ATAN == 20);
	static_assert(ATAN2 == 21);

	return PackedStringArray{
		"Equals (amount)",
		"Less than (amount)",
		"Add (amount)",
		"Subtract (amount)",
		"Multiply (amount)",
		"Divide (amount)",
		"Remainder (amount)",
		"Amount to float",
		"Round down to amount",
		"Round up to amount",
		"Less than (float)",
		"Add (float)",
		"Subtract (float)",
		"Multiply (float)",
		"Divide (float)",
		"Remainder (float)",
		"Exponent (float)",
		"Sine (float)",
		"Cosine (float)",
		"Tangent (float)",
		"Arctangent (float)",
		"Arctangent (y, x) (float)",
	};
}

int64_t JigsawCommandMath::get_num_arguments() const {
	switch (_op) {
	case EQUALS:
	case LESS_THAN:
	case ADD:
	case SUBTRACT:
	case MULTIPLY:
	case DIVIDE:
	case REMAINDER:
		return 2;
	case AMOUNT_TO_FLOAT:
	case FLOOR:
	case CEIL:
		return 1;
	case FLOAT_LESS_THAN:
	case FLOAT_ADD:
	case FLOAT_SUBTRACT:
	case FLOAT_MULTIPLY:
	case FLOAT_DIVIDE:
	case FLOAT_REMAINDER:
	case POW:
		return 2;
	case SIN:
	case COS:
	case TAN:
	case ATAN:
		return 1;
	case ATAN2:
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
	switch (_op) {
	case EQUALS:
	case LESS_THAN:
	case ADD:
	case SUBTRACT:
	case MULTIPLY:
	case DIVIDE:
	case REMAINDER:
	case AMOUNT_TO_FLOAT:
		return Array::make(JigsawParameterAmount::make(0));
	case FLOOR:
	case CEIL:
	case FLOAT_LESS_THAN:
	case FLOAT_ADD:
	case FLOAT_SUBTRACT:
	case FLOAT_MULTIPLY:
	case FLOAT_DIVIDE:
	case FLOAT_REMAINDER:
	case POW:
	case SIN:
	case COS:
	case TAN:
	case ATAN:
	case ATAN2:
		return Array::make(JigsawParameterFloat::make(0.0));
	}

	return TypedArray<JigsawParameter>();
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
	case ADD:
	case SUBTRACT:
	case MULTIPLY:
	case DIVIDE:
	case REMAINDER:
		return Array::make(JigsawParameterAmount::make(0));
	case AMOUNT_TO_FLOAT:
		return Array::make(JigsawParameterFloat::make(0.0));
	case FLOOR:
	case CEIL:
		return Array::make(JigsawParameterAmount::make(0));
	case FLOAT_LESS_THAN:
		return Array::make(JigsawParameterBoolean::make(false));
	case FLOAT_ADD:
	case FLOAT_SUBTRACT:
	case FLOAT_MULTIPLY:
	case FLOAT_DIVIDE:
	case FLOAT_REMAINDER:
	case POW:
	case SIN:
	case COS:
	case TAN:
	case ATAN:
	case ATAN2:
		return Array::make(JigsawParameterFloat::make(0.0));
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
