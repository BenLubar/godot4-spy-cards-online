#include "card_filter_cost.h"

void CardFilterCost::_bind_methods() {
	BIND_PROPERTY_ENUM(StatDef::Stat, stat);
	BIND_PROPERTY(Variant::INT, amount);
	BIND_PROPERTY(Variant::INT, amount_inf);
	BIND_PROPERTY_IS(Variant::BOOL, nan);
}

IMPLEMENT_PROPERTY(CardFilterCost, StatDef::Stat, stat);
IMPLEMENT_PROPERTY(CardFilterCost, int64_t, amount);
IMPLEMENT_PROPERTY(CardFilterCost, int64_t, amount_inf);
IMPLEMENT_PROPERTY_IS(CardFilterCost, bool, nan);

Ref<CardFilter> CardFilter::make_cost(StatDef::Stat stat, int64_t amount) {
	CardFilterCost *filter = memnew(CardFilterCost);
	filter->set_stat(stat);
	filter->set_amount(amount);
	return filter;
}
