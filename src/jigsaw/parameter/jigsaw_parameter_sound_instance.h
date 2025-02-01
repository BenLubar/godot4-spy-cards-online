#ifndef JIGSAW_PARAMETER_SOUND_INSTANCE_H
#define JIGSAW_PARAMETER_SOUND_INSTANCE_H

#include "jigsaw/parameter/jigsaw_parameter.h"

class JigsawParameterSoundInstance : public JigsawParameter {
	GDCLASS(JigsawParameterSoundInstance, JigsawParameter);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(int64_t, sound, = -1);

	Type get_type() const override { return SOUND_INSTANCE; }

	static Ref<JigsawParameterSoundInstance> make(int64_t sound);
};

#endif // JIGSAW_PARAMETER_SOUND_INSTANCE_H
