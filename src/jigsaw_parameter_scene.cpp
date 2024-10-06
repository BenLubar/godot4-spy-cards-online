#include "jigsaw_parameter_scene.h"

void JigsawParameterScene::_bind_methods() {
	BIND_PROPERTY_RESOURCE_NOT_SAVED(GLTFState, scene);
	BIND_PROPERTY(Variant::DICTIONARY, json_data);
}

IMPLEMENT_PROPERTY(JigsawParameterScene, Ref<GLTFState>, scene);
IMPLEMENT_PROPERTY(JigsawParameterScene, Dictionary, json_data);
