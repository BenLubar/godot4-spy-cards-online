#ifndef JIGSAW_PARAMETER_CID_OPUS_H
#define JIGSAW_PARAMETER_CID_OPUS_H

#include "jigsaw/parameter/jigsaw_parameter_audio.h"

class JigsawParameterCIDOpus : public JigsawParameterAudio {
	GDCLASS(JigsawParameterCIDOpus, JigsawParameterAudio);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(PackedByteArray, cid);

	Type get_type() const override { return CID_OPUS; }

	static Ref<JigsawParameterCIDOpus> make(PackedByteArray cid, float loop_start, float loop_end);
};

#endif // JIGSAW_PARAMETER_CID_OPUS_H
