#include "jigsaw_parameter_amount.h"

void JigsawParameterAmount::_bind_methods() {
	BIND_PROPERTY(Variant::INT, amount);
	BIND_PROPERTY(Variant::INT, amount_inf);
	BIND_PROPERTY_IS(Variant::BOOL, nan);
}

IMPLEMENT_PROPERTY(JigsawParameterAmount, int64_t, amount);
IMPLEMENT_PROPERTY(JigsawParameterAmount, int64_t, amount_inf);
IMPLEMENT_PROPERTY_IS(JigsawParameterAmount, bool, nan);
