#ifndef JIGSAW_PARAMETER_CHARACTER_H
#define JIGSAW_PARAMETER_CHARACTER_H

#include "jigsaw_parameter.h"

class JigsawParameterCharacter : public JigsawParameter {
	GDCLASS(JigsawParameterCharacter, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterCharacter() = default;
	~JigsawParameterCharacter() = default;

	DECLARE_PROPERTY(int64_t, character, = -1);

	Type get_type() const override { return CHARACTER; }

	static Ref<JigsawParameterCharacter> make(int64_t character);
};

#endif // JIGSAW_PARAMETER_CHARACTER_H
