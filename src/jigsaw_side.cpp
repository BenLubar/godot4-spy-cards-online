#include "jigsaw_side.h"

void JigsawSide::_bind_methods() {
	BIND_PROPERTY(Variant::STRING, player_name);
	BIND_PROPERTY_ENUM(enums::CharacterDef::Character, character);
	BIND_PROPERTY_ENUM_ARRAY(enums::CardDef::Card, initial_deck);

	BIND_PROPERTY_RESOURCE_ARRAY(JigsawSideLocation, locations);
	BIND_PROPERTY_RESOURCE_ARRAY(NumericValue, stats);
}

IMPLEMENT_PROPERTY_SIMPLE(JigsawSide, String, player_name);
IMPLEMENT_PROPERTY_SIMPLE(JigsawSide, enums::CharacterDef::Character, character);
IMPLEMENT_PROPERTY_SIMPLE(JigsawSide, TypedArray<enums::CardDef::Card>, initial_deck);

IMPLEMENT_PROPERTY_SIMPLE(JigsawSide, TypedArray<JigsawSideLocation>, locations);
IMPLEMENT_PROPERTY_SIMPLE(JigsawSide, TypedArray<NumericValue>, stats);
