#ifndef JIGSAW_PARAMETER_CHARACTER_INSTANCE_H
#define JIGSAW_PARAMETER_CHARACTER_INSTANCE_H

#include "jigsaw_parameter.h"

class JigsawParameterCharacterInstance : public JigsawParameter {
	GDCLASS(JigsawParameterCharacterInstance, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterCharacterInstance() = default;
	~JigsawParameterCharacterInstance() = default;

	DECLARE_PROPERTY(int64_t, character, = -1);

	Type get_type() const override { return CHARACTER_INSTANCE; }

	static Ref<JigsawParameterCharacterInstance> make(int64_t character);
};

#endif // JIGSAW_PARAMETER_CHARACTER_INSTANCE_H
