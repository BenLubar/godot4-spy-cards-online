#ifndef JIGSAW_SIDE_H
#define JIGSAW_SIDE_H

#include "dry.h"

#include "enums/enums_card_def.h"
#include "enums/enums_character_def.h"

#include <godot_cpp/classes/hashing_context.hpp>

class JigsawSide;

#include "defs/numeric_value.h"
#include "jigsaw/parameter/jigsaw_parameter.h"

class JigsawSide : public Resource {
	GDCLASS(JigsawSide, Resource);

protected:
	static void _bind_methods();

public:
	// immutable state
	DECLARE_PROPERTY(String, player_name);
	DECLARE_PROPERTY(enums::CharacterDef::Character, character, = enums::CharacterDef::Character::NONE);
	DECLARE_PROPERTY(PackedArray<enums::CardDef::Card>, initial_deck);

	// mutable state (hybrid copy on write)
	DECLARE_PROPERTY(TypedArray<PackedInt32Array>, location_card_instances);
	DECLARE_PROPERTY(TypedArray<NumericValue>, stats);
	DECLARE_PROPERTY(VariableParameterDict, variables);

	void assign(const Ref<JigsawSide> &side);
	void write_hash(const Ref<HashingContext> &hash) const;

	DEFAULT_TO_STRING();
};

#endif // JIGSAW_SIDE_H
