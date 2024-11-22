#include "stat_value.h"

void StatValue::_bind_methods() {
	BIND_PROPERTY_ENUM(enums::StatDef::Stat, stat);
}

IMPLEMENT_PROPERTY(StatValue, enums::StatDef::Stat, stat);
