#include "jigsaw_parameter_float.h"

void JigsawParameterFloat::_bind_methods() {
	BIND_PROPERTY(Variant::FLOAT, value);
}

IMPLEMENT_PROPERTY(JigsawParameterFloat, double, value);
