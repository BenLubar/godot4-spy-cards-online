#include "jigsaw/parameter/jigsaw_parameter_stat_value.h"

void JigsawParameterStatValue::_bind_methods() {
	BIND_PROPERTY_ENUM(enums::StatDef::Stat, stat);
	BIND_PROPERTY(Variant::INT, amount);
	BIND_PROPERTY(Variant::INT, amount_inf);
	BIND_PROPERTY_IS(Variant::BOOL, nan);

	ClassDB::bind_static_method("JigsawParameterStatValue", D_METHOD("make", "stat", "amount", "amount_inf"), &JigsawParameterStatValue::make, DEFVAL(0));
	ClassDB::bind_static_method("JigsawParameterStatValue", D_METHOD("make_nan", "stat"), &JigsawParameterStatValue::make_nan);
}

IMPLEMENT_PROPERTY(JigsawParameterStatValue, enums::StatDef::Stat, stat);
IMPLEMENT_PROPERTY(JigsawParameterStatValue, int64_t, amount);
IMPLEMENT_PROPERTY(JigsawParameterStatValue, int64_t, amount_inf);
IMPLEMENT_PROPERTY_IS(JigsawParameterStatValue, bool, nan);

Ref<JigsawParameterStatValue> JigsawParameterStatValue::make(enums::StatDef::Stat stat, int64_t amount, int64_t amount_inf) {
	Ref<JigsawParameterStatValue> param;
	param.instantiate();
	param->set_stat(stat);
	param->set_amount(amount);
	param->set_amount_inf(amount_inf);
	return param;
}

Ref<JigsawParameterStatValue> JigsawParameterStatValue::make_nan(enums::StatDef::Stat stat) {
	Ref<JigsawParameterStatValue> param;
	param.instantiate();
	param->set_stat(stat);
	param->set_nan(true);
	return param;
}

Ref<JigsawParameterStatValue> JigsawParameterStatValue::convert(const Ref<StatValue> &stat_value) {
	ERR_FAIL_COND_V(stat_value.is_null(), Ref<JigsawParameterStatValue>());

	Ref<JigsawParameterStatValue> param;
	param.instantiate();
	param->set_stat(stat_value->get_stat());
	if (stat_value->is_nan()) {
		param->set_nan(true);
	} else {
		param->set_amount(stat_value->get_amount());
		param->set_amount_inf(stat_value->get_amount_inf());
	}

	return param;
}
