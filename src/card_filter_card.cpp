#include "card_filter_card.h"

void CardFilterCard::_bind_methods() {
	BIND_PROPERTY_ENUM(CardDef::Card, card);
}

IMPLEMENT_PROPERTY(CardFilterCard, CardDef::Card, card);

Ref<CardFilter> CardFilter::make_card(CardDef::Card card) {
	CardFilterCard *filter = memnew(CardFilterCard);
	filter->set_card(card);
	return filter;
}
