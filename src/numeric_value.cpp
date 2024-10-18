#include "numeric_value.h"

void NumericValue::_bind_methods() {
	BIND_PROPERTY(Variant::INT, amount);
	BIND_PROPERTY(Variant::INT, amount_inf);
	BIND_PROPERTY_IS(Variant::BOOL, nan);
}

IMPLEMENT_PROPERTY(NumericValue, int64_t, amount);
IMPLEMENT_PROPERTY(NumericValue, int64_t, amount_inf);
IMPLEMENT_PROPERTY_IS(NumericValue, bool, nan);
