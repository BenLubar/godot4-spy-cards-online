#ifndef JIGSAW_SIDE_H
#define JIGSAW_SIDE_H

#include "dry.h"

#include "enums/enums_card_def.h"
#include "enums/enums_character_def.h"

class JigsawSide;

#include "jigsaw/jigsaw_side_location.h"
#include "defs/numeric_value.h"

#include <godot_cpp/templates/hash_map.hpp>

class JigsawSide : public RefCounted {
	GDCLASS(JigsawSide, RefCounted);

protected:
	static void _bind_methods();

public:
	// immutable state
	DECLARE_PROPERTY(String, player_name);
	DECLARE_PROPERTY(enums::CharacterDef::Character, character, = enums::CharacterDef::Character::NONE);
	DECLARE_PROPERTY(TypedArray<enums::CardDef::Card>, initial_deck);

	// mutable state
	DECLARE_PROPERTY(TypedArray<JigsawSideLocation>, locations);
	DECLARE_PROPERTY(TypedArray<NumericValue>, stats);

	HashMap<enums::VariableDef::Variable, Ref<JigsawParameter>> _variables;

	DEFAULT_TO_STRING();
};

#endif // JIGSAW_SIDE_H
