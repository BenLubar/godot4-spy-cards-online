#ifndef DECK_H
#define DECK_H

#include "dry.h"

using namespace godot;

class Deck : public Resource {
	GDCLASS(Deck, Resource);

protected:
	static void _bind_methods();

public:
	Deck();
	~Deck();

	DECLARE_PROPERTY(PackedInt64Array, cards);
};

#endif // DECK_H
