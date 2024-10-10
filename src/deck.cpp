#include "deck.h"

void Deck::_bind_methods() {
	BIND_PROPERTY_ENUM_ARRAY(CardDef::Card, cards);
}

IMPLEMENT_PROPERTY(Deck, TypedArray<CardDef::Card>, cards);
