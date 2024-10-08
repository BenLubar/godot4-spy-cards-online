#ifndef JIGSAW_PARAMETER_SCENE_INSTANCE_H
#define JIGSAW_PARAMETER_SCENE_INSTANCE_H

#include "jigsaw_parameter.h"

class JigsawParameterSceneInstance : public JigsawParameter {
	GDCLASS(JigsawParameterSceneInstance, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterSceneInstance() = default;
	~JigsawParameterSceneInstance() = default;

	DECLARE_PROPERTY(int64_t, scene, = -1);

	Type get_type() const override { return SCENE_INSTANCE; }

	static Ref<JigsawParameterSceneInstance> make(int64_t scene);
};

#endif // JIGSAW_PARAMETER_SCENE_INSTANCE_H
