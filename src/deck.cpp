#include "deck.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void Deck::_bind_methods() {
	BIND_PROPERTY(Variant::PACKED_INT64_ARRAY, cards);
}
Deck::Deck() {}
Deck::~Deck() {}

IMPLEMENT_PROPERTY(Deck, PackedInt64Array, cards);
