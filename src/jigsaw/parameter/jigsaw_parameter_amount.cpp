#include "jigsaw_parameter_amount.h"

void JigsawParameterAmount::_bind_methods() {
	BIND_PROPERTY(Variant::INT, amount);
	BIND_PROPERTY(Variant::INT, amount_inf);
	BIND_PROPERTY_IS(Variant::BOOL, nan);

	ClassDB::bind_static_method("JigsawParameterAmount", D_METHOD("make", "amount", "amount_inf"), &JigsawParameterAmount::make, DEFVAL(int64_t(0)));
	ClassDB::bind_static_method("JigsawParameterAmount", D_METHOD("make_nan"), &JigsawParameterAmount::make_nan);
}

IMPLEMENT_PROPERTY(JigsawParameterAmount, int64_t, amount);
IMPLEMENT_PROPERTY(JigsawParameterAmount, int64_t, amount_inf);
IMPLEMENT_PROPERTY_IS(JigsawParameterAmount, bool, nan);

String JigsawParameterAmount::_to_string() const {
	if (_nan) {
		return "NaN";
	}

	if (_amount_inf > 0) {
		return String::chr(0x221e);
	}

	if (_amount_inf < 0) {
		return "-" + String::chr(0x221e);
	}

	return String::num_int64(_amount);
}

Ref<JigsawParameterAmount> JigsawParameterAmount::make(int64_t amount, int64_t amount_inf) {
	Ref<JigsawParameterAmount> param;
	param.instantiate();
	param->set_amount(amount);
	param->set_amount_inf(amount_inf);
	return param;
}
Ref<JigsawParameterAmount> JigsawParameterAmount::make_nan() {
	Ref<JigsawParameterAmount> param;
	param.instantiate();
	param->set_nan(true);
	return param;
}
