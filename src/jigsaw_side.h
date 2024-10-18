#ifndef JIGSAW_SIDE_H
#define JIGSAW_SIDE_H

#include "dry.h"

#include "enums_card_def.h"
#include "enums_character_def.h"

class JigsawSide;

#include "jigsaw_side_location.h"
#include "numeric_value.h"

class JigsawSide : public RefCounted {
	GDCLASS(JigsawSide, RefCounted);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(String, player_name);
	DECLARE_PROPERTY(enums::CharacterDef::Character, character, = enums::CharacterDef::Character::NONE);
	DECLARE_PROPERTY(TypedArray<enums::CardDef::Card>, initial_deck);

	DECLARE_PROPERTY(TypedArray<JigsawSideLocation>, locations);
	DECLARE_PROPERTY(TypedArray<NumericValue>, stats);
};

#endif // JIGSAW_SIDE_H
