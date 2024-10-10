#include "jigsaw_parameter_character.h"

void JigsawParameterCharacter::_bind_methods() {
	BIND_PROPERTY(Variant::INT, character);
}

IMPLEMENT_PROPERTY(JigsawParameterCharacter, int64_t, character);
