#include "jigsaw_parameter_cid_opus.h"

void JigsawParameterCIDOpus::_bind_methods() {
	BIND_PROPERTY(Variant::PACKED_BYTE_ARRAY, cid);

	ClassDB::bind_static_method("JigsawParameterCIDOpus", D_METHOD("make", "cid", "loop_start", "loop_end"), &JigsawParameterCIDOpus::make);
}

IMPLEMENT_PROPERTY(JigsawParameterCIDOpus, PackedByteArray, cid);

Ref<JigsawParameterCIDOpus> JigsawParameterCIDOpus::make(PackedByteArray cid, double loop_start, double loop_end) {
	Ref<JigsawParameterCIDOpus> param;
	param.instantiate();
	param->set_cid(cid);
	param->set_loop_start(loop_start);
	param->set_loop_end(loop_end);
	return param;
}
