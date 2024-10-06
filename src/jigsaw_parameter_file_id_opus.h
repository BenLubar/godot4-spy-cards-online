#ifndef JIGSAW_PARAMETER_FILE_ID_OPUS_H
#define JIGSAW_PARAMETER_FILE_ID_OPUS_H

#include "jigsaw_parameter_audio.h"

class JigsawParameterFileIDOpus : public JigsawParameterAudio {
	GDCLASS(JigsawParameterFileIDOpus, JigsawParameterAudio);

protected:
	static void _bind_methods();

public:
	JigsawParameterFileIDOpus() = default;
	~JigsawParameterFileIDOpus() = default;

	DECLARE_PROPERTY(PackedByteArray, file_id);

	Type get_type() const override { return FILE_ID_OPUS; }

	static JigsawParameterFileIDOpus *make(PackedByteArray file_id, double loop_start, double loop_end);
};

#endif // JIGSAW_PARAMETER_FILE_ID_OPUS_H
