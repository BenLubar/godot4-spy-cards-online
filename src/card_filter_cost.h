#ifndef CARD_FILTER_COST_H
#define CARD_FILTER_COST_H

#include "dry.h"

#include "card_filter.h"

class CardFilterCost : public CardFilter {
	GDCLASS(CardFilterCost, CardFilter);

protected:
	static void _bind_methods();

public:
	CardFilterCost() = default;
	~CardFilterCost() = default;

	DECLARE_PROPERTY(StatDef::Stat, stat, = StatDef::Stat::NONE);
	DECLARE_PROPERTY(int64_t, amount, = 0);
	DECLARE_PROPERTY(int64_t, amount_inf, = 0);
	DECLARE_PROPERTY_IS(bool, nan, = false);

	Type get_type() const override { return COST; }
};

#endif // CARD_FILTER_COST_H
