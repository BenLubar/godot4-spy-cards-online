#include "stat_value.h"

void StatValue::_bind_methods() {
	BIND_PROPERTY_ENUM(enums::StatDef::Stat, stat);
	BIND_PROPERTY(Variant::INT, amount);
	BIND_PROPERTY(Variant::INT, amount_inf);
	BIND_PROPERTY_IS(Variant::BOOL, nan);
}

IMPLEMENT_PROPERTY(StatValue, enums::StatDef::Stat, stat);
IMPLEMENT_PROPERTY(StatValue, int64_t, amount);
IMPLEMENT_PROPERTY(StatValue, int64_t, amount_inf);
IMPLEMENT_PROPERTY_IS(StatValue, bool, nan);
