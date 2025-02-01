#ifndef JIGSAW_PARAMETER_SCENE_H
#define JIGSAW_PARAMETER_SCENE_H

#include "jigsaw/parameter/jigsaw_parameter.h"

#include <godot_cpp/classes/animation_root_node.hpp>
#include <godot_cpp/classes/gltf_state.hpp>

class JigsawParameterScene : public JigsawParameter {
	GDCLASS(JigsawParameterScene, JigsawParameter);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(Ref<GLTFState>, scene);
	DECLARE_PROPERTY(Ref<AnimationRootNode>, animation);
	DECLARE_PROPERTY(Dictionary, json_data);

	Node *instantiate_scene(bool static_lights, bool allow_shadows) const;
	static Ref<AnimationRootNode> parse_animation_data(const String &raw);
	static String serialize_animation_data(const Ref<AnimationRootNode> &node);
};

#endif // JIGSAW_PARAMETER_SCENE_H
