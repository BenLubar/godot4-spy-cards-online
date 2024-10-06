#ifndef JIGSAW_PARAMETER_AUDIO_H
#define JIGSAW_PARAMETER_AUDIO_H

#include "jigsaw_parameter.h"

#include <godot_cpp/classes/audio_stream_wav.hpp>

class JigsawParameterAudio : public JigsawParameter {
	GDCLASS(JigsawParameterAudio, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterAudio() = default;
	~JigsawParameterAudio() = default;

	DECLARE_PROPERTY(double, loop_start, = 0.0);
	DECLARE_PROPERTY(double, loop_end, = 0.0);
	DECLARE_PROPERTY(Ref<AudioStreamWAV>, audio);
};

#endif // JIGSAW_PARAMETER_AUDIO_H
