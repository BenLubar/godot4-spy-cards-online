#include "jigsaw_parameter_file_id_gltf.h"

void JigsawParameterFileIDGLTF::_bind_methods() {
	BIND_PROPERTY(Variant::PACKED_BYTE_ARRAY, file_id);
}

IMPLEMENT_PROPERTY(JigsawParameterFileIDGLTF, PackedByteArray, file_id);
