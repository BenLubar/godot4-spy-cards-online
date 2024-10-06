#include "jigsaw_parameter_sprite_instance.h"

void JigsawParameterSpriteInstance::_bind_methods() {
	BIND_PROPERTY(Variant::INT, sprite);
}

IMPLEMENT_PROPERTY(JigsawParameterSpriteInstance, int64_t, sprite);
