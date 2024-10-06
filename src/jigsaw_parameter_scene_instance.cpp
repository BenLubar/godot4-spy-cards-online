#include "jigsaw_parameter_scene_instance.h"

void JigsawParameterSceneInstance::_bind_methods() {
	BIND_PROPERTY(Variant::INT, scene);
}

IMPLEMENT_PROPERTY(JigsawParameterSceneInstance, int64_t, scene);
