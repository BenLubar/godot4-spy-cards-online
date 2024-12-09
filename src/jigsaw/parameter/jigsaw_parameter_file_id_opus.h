#ifndef JIGSAW_PARAMETER_FILE_ID_OPUS_H
#define JIGSAW_PARAMETER_FILE_ID_OPUS_H

#include "jigsaw/parameter/jigsaw_parameter_audio.h"

class JigsawParameterFileIDOpus : public JigsawParameterAudio {
	GDCLASS(JigsawParameterFileIDOpus, JigsawParameterAudio);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(PackedByteArray, file_id);

	Type get_type() const override { return FILE_ID_OPUS; }

	static Ref<JigsawParameterFileIDOpus> make(PackedByteArray file_id, float loop_start, float loop_end);
};

#endif // JIGSAW_PARAMETER_FILE_ID_OPUS_H
