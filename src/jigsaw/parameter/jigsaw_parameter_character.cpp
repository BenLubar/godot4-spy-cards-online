#include "jigsaw_parameter_character.h"

void JigsawParameterCharacter::_bind_methods() {
	BIND_PROPERTY_ENUM(enums::CharacterDef::Character, character);

	ClassDB::bind_static_method("JigsawParameterCharacter", D_METHOD("make", "character"), &JigsawParameterCharacter::make);
}

IMPLEMENT_PROPERTY(JigsawParameterCharacter, enums::CharacterDef::Character, character);

Ref<JigsawParameterCharacter> JigsawParameterCharacter::make(enums::CharacterDef::Character character) {
	Ref<JigsawParameterCharacter> param;
	param.instantiate();
	param->set_character(character);
	return param;
}
