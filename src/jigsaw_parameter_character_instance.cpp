#include "jigsaw_parameter_character_instance.h"

void JigsawParameterCharacterInstance::_bind_methods() {
	BIND_PROPERTY(Variant::INT, character);
}

IMPLEMENT_PROPERTY(JigsawParameterCharacterInstance, int64_t, character);
