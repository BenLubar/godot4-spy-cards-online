#include "jigsaw_parameter_audio.h"

void JigsawParameterAudio::_bind_methods() {
	BIND_PROPERTY(Variant::FLOAT, loop_start);
	BIND_PROPERTY(Variant::FLOAT, loop_end);
	BIND_PROPERTY_RESOURCE_NOT_SAVED(AudioStreamWAV, audio);
}

IMPLEMENT_PROPERTY(JigsawParameterAudio, double, loop_start);
IMPLEMENT_PROPERTY(JigsawParameterAudio, double, loop_end);
IMPLEMENT_PROPERTY(JigsawParameterAudio, Ref<AudioStreamWAV>, audio);
