#include "jigsaw_parameter_amount.h"

void JigsawParameterAmount::_bind_methods() {
	BIND_PROPERTY(Variant::INT, amount);
	BIND_PROPERTY(Variant::INT, amount_inf);
	BIND_PROPERTY_IS(Variant::BOOL, nan);

	ClassDB::bind_static_method("JigsawParameterAmount", D_METHOD("make", "amount"), &JigsawParameterAmount::make);
	ClassDB::bind_static_method("JigsawParameterAmount", D_METHOD("make_inf", "amount_inf"), &JigsawParameterAmount::make_inf);
	ClassDB::bind_static_method("JigsawParameterAmount", D_METHOD("make_nan"), &JigsawParameterAmount::make_nan);
}

IMPLEMENT_PROPERTY(JigsawParameterAmount, int64_t, amount);
IMPLEMENT_PROPERTY(JigsawParameterAmount, int64_t, amount_inf);
IMPLEMENT_PROPERTY_IS(JigsawParameterAmount, bool, nan);

JigsawParameterAmount *JigsawParameterAmount::make(int64_t amount) {
	JigsawParameterAmount *param = memnew(JigsawParameterAmount);
	param->set_amount(amount);
	return param;
}
JigsawParameterAmount *JigsawParameterAmount::make_inf(int64_t amount_inf) {
	JigsawParameterAmount *param = memnew(JigsawParameterAmount);
	param->set_amount_inf(amount_inf);
	return param;
}
JigsawParameterAmount *JigsawParameterAmount::make_nan() {
	JigsawParameterAmount *param = memnew(JigsawParameterAmount);
	param->set_nan(true);
	return param;
}
