#ifndef CARD_FILTER_CARD_H
#define CARD_FILTER_CARD_H

#include "card_filter.h"

class CardFilterCard : public CardFilter {
	GDCLASS(CardFilterCard, CardFilter);

protected:
	static void _bind_methods();

public:
	CardFilterCard() = default;
	~CardFilterCard() = default;

	DECLARE_PROPERTY(enums::CardDef::Card, card, = enums::CardDef::Card::NONE);

	Type get_type() const override { return CARD; }
	bool _matches_def(const Ref<JigsawContext> &ctx, enums::CardDef::Card card) const override;
	bool _matches_instance(const Ref<JigsawContext> &ctx, const Ref<CardInstance> &inst) const override;
};

#endif // CARD_FILTER_CARD_H
