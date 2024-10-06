#ifndef CARD_FILTER_CARD_H
#define CARD_FILTER_CARD_H

#include "dry.h"

#include "card_filter.h"

class CardFilterCard : public CardFilter {
	GDCLASS(CardFilterCard, CardFilter);

protected:
	static void _bind_methods();

public:
	CardFilterCard() = default;
	~CardFilterCard() = default;

	DECLARE_PROPERTY(CardDef::Card, card, = CardDef::Card::NONE);

	Type get_type() const override { return CARD; }
};

#endif // CARD_FILTER_CARD_H
