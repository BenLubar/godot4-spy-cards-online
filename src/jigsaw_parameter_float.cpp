#include "jigsaw_parameter_float.h"

void JigsawParameterFloat::_bind_methods() {
	BIND_PROPERTY(Variant::FLOAT, value);

	ClassDB::bind_static_method("JigsawParameterFloat", D_METHOD("make", "value"), &JigsawParameterFloat::make);
}

IMPLEMENT_PROPERTY(JigsawParameterFloat, double, value);

Ref<JigsawParameterFloat> JigsawParameterFloat::make(double value) {
	Ref<JigsawParameterFloat> param;
	param.instantiate();
	param->set_value(value);
	return param;
}
