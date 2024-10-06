#include "jigsaw_parameter_cid_opus.h"

void JigsawParameterCIDOpus::_bind_methods() {
	BIND_PROPERTY(Variant::PACKED_BYTE_ARRAY, cid);
}

IMPLEMENT_PROPERTY(JigsawParameterCIDOpus, PackedByteArray, cid);
