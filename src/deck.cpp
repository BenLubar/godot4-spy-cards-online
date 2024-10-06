#include "deck.h"

void Deck::_bind_methods() {
	BIND_PROPERTY(Variant::ARRAY, cards);
}

IMPLEMENT_PROPERTY(Deck, TypedArray<CardDef::Card>, cards);
