#include "jigsaw_parameter_sprite_instance.h"

void JigsawParameterSpriteInstance::_bind_methods() {
	BIND_PROPERTY(Variant::INT, sprite);

	ClassDB::bind_static_method("JigsawParameterSpriteInstance", D_METHOD("make", "sprite"), &JigsawParameterSpriteInstance::make);
}

IMPLEMENT_PROPERTY(JigsawParameterSpriteInstance, int64_t, sprite);

JigsawParameterSpriteInstance *JigsawParameterSpriteInstance::make(int64_t sprite) {
	JigsawParameterSpriteInstance *param = memnew(JigsawParameterSpriteInstance);
	param->set_sprite(sprite);
	return param;
}
