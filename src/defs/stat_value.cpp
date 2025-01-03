#include "stat_value.h"

void StatValue::_bind_methods() {
	BIND_PROPERTY_ENUM(enums::StatDef::Stat, stat);

	ClassDB::bind_static_method("StatValue", D_METHOD("make", "stat", "amount", "amount_inf"), &StatValue::make, DEFVAL(0));
	ClassDB::bind_static_method("StatValue", D_METHOD("make_nan", "stat"), &StatValue::make_nan);
}

IMPLEMENT_PROPERTY(StatValue, enums::StatDef::Stat, stat);

Ref<StatValue> StatValue::make(enums::StatDef::Stat stat, int64_t amount, int64_t amount_inf) {
	Ref<StatValue> value;
	value.instantiate();

	value->set_stat(stat);
	value->set_amount(amount);
	value->set_amount_inf(amount_inf);

	return value;
}
Ref<StatValue> StatValue::make_nan(enums::StatDef::Stat stat) {
	Ref<StatValue> value;
	value.instantiate();

	value->set_stat(stat);
	value->set_nan(true);

	return value;
}
