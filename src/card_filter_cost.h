#ifndef CARD_FILTER_COST_H
#define CARD_FILTER_COST_H

#include "card_filter.h"

class CardFilterCost : public CardFilter {
	GDCLASS(CardFilterCost, CardFilter);

protected:
	static void _bind_methods();

public:
	CardFilterCost() = default;
	~CardFilterCost() = default;

	DECLARE_PROPERTY(enums::StatDef::Stat, stat, = enums::StatDef::Stat::NONE);
	DECLARE_PROPERTY(int64_t, amount, = 0);
	DECLARE_PROPERTY(int64_t, amount_inf, = 0);
	DECLARE_PROPERTY_IS(bool, nan, = false);

	Type get_type() const override { return COST; }
	bool _matches_costs(const TypedArray<StatValue> &costs) const;
	bool _matches_def(const Ref<JigsawContext> &ctx, enums::CardDef::Card card) const override;
	bool _matches_instance(const Ref<JigsawContext> &ctx, const Ref<CardInstance> &inst) const override;
};

#endif // CARD_FILTER_COST_H
