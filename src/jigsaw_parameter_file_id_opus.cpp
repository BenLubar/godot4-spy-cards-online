#include "jigsaw_parameter_file_id_opus.h"

void JigsawParameterFileIDOpus::_bind_methods() {
	BIND_PROPERTY(Variant::PACKED_BYTE_ARRAY, file_id);
}

IMPLEMENT_PROPERTY(JigsawParameterFileIDOpus, PackedByteArray, file_id);
