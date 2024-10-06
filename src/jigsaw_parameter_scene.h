#ifndef JIGSAW_PARAMETER_SCENE_H
#define JIGSAW_PARAMETER_SCENE_H

#include "jigsaw_parameter.h"

#include <godot_cpp/classes/gltf_state.hpp>

class JigsawParameterScene : public JigsawParameter {
	GDCLASS(JigsawParameterScene, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterScene() = default;
	~JigsawParameterScene() = default;

	DECLARE_PROPERTY(Ref<GLTFState>, scene);
	DECLARE_PROPERTY(Dictionary, json_data);
};

#endif // JIGSAW_PARAMETER_SCENE_H
