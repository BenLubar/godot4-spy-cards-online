#ifndef DECK_H
#define DECK_H

#include "dry.h"

#include "enums/enums_card_def.h"

class Deck : public Object {
	GDCLASS(Deck, Object);

protected:
	static void _bind_methods();

public:
	static PackedByteArray encode(const TypedArray<enums::CardDef::Card> &cards);
	static TypedArray<enums::CardDef::Card> decode(const PackedByteArray &buf);
};

#endif // DECK_H
