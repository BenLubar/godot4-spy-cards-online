#include "jigsaw_parameter_scene_instance.h"

void JigsawParameterSceneInstance::_bind_methods() {
	BIND_PROPERTY(Variant::INT, scene);

	ClassDB::bind_static_method("JigsawParameterSceneInstance", D_METHOD("make", "scene"), &JigsawParameterSceneInstance::make);
}

IMPLEMENT_PROPERTY(JigsawParameterSceneInstance, int64_t, scene);

Ref<JigsawParameterSceneInstance> JigsawParameterSceneInstance::make(int64_t scene) {
	Ref<JigsawParameterSceneInstance> param;
	param.instantiate();
	param->set_scene(scene);
	return param;
}
