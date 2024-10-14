#ifndef CARD_FILTER_AND_H
#define CARD_FILTER_AND_H

#include "card_filter.h"

class CardFilterAnd : public CardFilter {
	GDCLASS(CardFilterAnd, CardFilter);

protected:
	static void _bind_methods();

public:
	CardFilterAnd() = default;
	~CardFilterAnd() = default;

	DECLARE_PROPERTY(TypedArray<CardFilter>, list);

	Type get_type() const override { return AND; }
	bool _matches_def(const Ref<JigsawContext> &ctx, enums::CardDef::Card card) const override;
	bool _matches_instance(const Ref<JigsawContext> &ctx, const Ref<CardInstance> &inst) const override;
};

#endif // CARD_FILTER_AND_H
