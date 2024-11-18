#include "jigsaw_parameter_character_instance.h"

void JigsawParameterCharacterInstance::_bind_methods() {
	BIND_PROPERTY(Variant::INT, character);
}

IMPLEMENT_PROPERTY(JigsawParameterCharacterInstance, int64_t, character);

Ref<JigsawParameterCharacterInstance> JigsawParameterCharacterInstance::make(int64_t character) {
	Ref<JigsawParameterCharacterInstance> param;
	param.instantiate();
	param->set_character(character);
	return param;
}
