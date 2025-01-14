#ifndef DECK_H
#define DECK_H

#include "dry.h"

#include "enums/enums_card_def.h"

class Deck : public Object {
	GDCLASS(Deck, Object);

protected:
	static void _bind_methods();

public:
	static PackedByteArray encode(const PackedArray<enums::CardDef::Card> &cards);
	static PackedArray<enums::CardDef::Card> decode(const PackedByteArray &buf);
};

#endif // DECK_H
