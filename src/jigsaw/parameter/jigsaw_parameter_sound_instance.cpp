#include "jigsaw/parameter/jigsaw_parameter_sound_instance.h"

void JigsawParameterSoundInstance::_bind_methods() {
	BIND_PROPERTY(Variant::INT, sound);

	ClassDB::bind_static_method("JigsawParameterSoundInstance", D_METHOD("make", "sound"), &JigsawParameterSoundInstance::make);
}

IMPLEMENT_PROPERTY(JigsawParameterSoundInstance, int64_t, sound);

Ref<JigsawParameterSoundInstance> JigsawParameterSoundInstance::make(int64_t sound) {
	Ref<JigsawParameterSoundInstance> param;
	param.instantiate();
	param->set_sound(sound);
	return param;
}
