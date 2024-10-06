#ifndef JIGSAW_PARAMETER_SPRITE_INSTANCE_H
#define JIGSAW_PARAMETER_SPRITE_INSTANCE_H

#include "jigsaw_parameter.h"

class JigsawParameterSpriteInstance : public JigsawParameter {
	GDCLASS(JigsawParameterSpriteInstance, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterSpriteInstance() = default;
	~JigsawParameterSpriteInstance() = default;

	DECLARE_PROPERTY(int64_t, sprite, = -1);

	Type get_type() const override { return SPRITE_INSTANCE; }

	static JigsawParameterSpriteInstance *make(int64_t sprite);
};

#endif // JIGSAW_PARAMETER_SPRITE_INSTANCE_H
