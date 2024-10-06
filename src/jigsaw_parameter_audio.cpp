#include "jigsaw_parameter_audio.h"

void JigsawParameterAudio::_bind_methods() {
	BIND_PROPERTY(Variant::FLOAT, loop_start);
	BIND_PROPERTY(Variant::FLOAT, loop_end);
	BIND_PROPERTY(Variant::OBJECT, audio, PROPERTY_HINT_RESOURCE_TYPE, "AudioStreamWAV", PROPERTY_USAGE_NONE);
}

IMPLEMENT_PROPERTY(JigsawParameterAudio, double, loop_start);
IMPLEMENT_PROPERTY(JigsawParameterAudio, double, loop_end);
IMPLEMENT_PROPERTY(JigsawParameterAudio, Ref<AudioStreamWAV>, audio);
