#include "jigsaw/parameter/jigsaw_parameter_cid_opus.h"

#include "util/base32.h"

void JigsawParameterCIDOpus::_bind_methods() {
	BIND_PROPERTY(Variant::PACKED_BYTE_ARRAY, cid);

	ClassDB::bind_static_method("JigsawParameterCIDOpus", D_METHOD("make", "cid", "loop_start", "loop_end"), &JigsawParameterCIDOpus::make);
}

IMPLEMENT_PROPERTY(JigsawParameterCIDOpus, PackedByteArray, cid);

String JigsawParameterCIDOpus::_to_string() const {
	return Base32::encode_cid(get_cid());
}

Ref<JigsawParameterCIDOpus> JigsawParameterCIDOpus::make(PackedByteArray cid, float loop_start, float loop_end) {
	Ref<JigsawParameterCIDOpus> param;
	param.instantiate();
	param->set_cid(cid);
	param->set_loop_start(loop_start);
	param->set_loop_end(loop_end);
	return param;
}
