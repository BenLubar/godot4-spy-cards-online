#ifndef DECK_H
#define DECK_H

#include "dry.h"

class Deck;

#include "card_def.h"

class Deck : public Resource {
	GDCLASS(Deck, Resource);

protected:
	static void _bind_methods();

public:
	Deck() = default;
	~Deck() = default;

	DECLARE_PROPERTY(TypedArray<CardDef::Card>, cards);
};

#endif // DECK_H
