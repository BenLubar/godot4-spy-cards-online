#ifndef JIGSAW_PARAMETER_CHARACTER_H
#define JIGSAW_PARAMETER_CHARACTER_H

#include "jigsaw_parameter.h"

#include "enums_character_def.h"

class JigsawParameterCharacter : public JigsawParameter {
	GDCLASS(JigsawParameterCharacter, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterCharacter() = default;
	~JigsawParameterCharacter() = default;

	DECLARE_PROPERTY(enums::CharacterDef::Character, character, = enums::CharacterDef::Character::NONE);

	Type get_type() const override { return CHARACTER; }

	static Ref<JigsawParameterCharacter> make(enums::CharacterDef::Character character);
};

#endif // JIGSAW_PARAMETER_CHARACTER_H
