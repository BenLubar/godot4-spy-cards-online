#include "jigsaw_parameter_scene.h"

void JigsawParameterScene::_bind_methods() {
	BIND_PROPERTY(Variant::OBJECT, scene, PROPERTY_HINT_RESOURCE_TYPE, "GLTFState", PROPERTY_USAGE_NONE);
	BIND_PROPERTY(Variant::DICTIONARY, json_data);
}

IMPLEMENT_PROPERTY(JigsawParameterScene, Ref<GLTFState>, scene);
IMPLEMENT_PROPERTY(JigsawParameterScene, Dictionary, json_data);
