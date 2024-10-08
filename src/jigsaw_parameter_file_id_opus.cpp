#include "jigsaw_parameter_file_id_opus.h"

void JigsawParameterFileIDOpus::_bind_methods() {
	BIND_PROPERTY(Variant::PACKED_BYTE_ARRAY, file_id);

	ClassDB::bind_static_method("JigsawParameterFileIDOpus", D_METHOD("make", "file_id", "loop_start", "loop_end"), &JigsawParameterFileIDOpus::make);
}

IMPLEMENT_PROPERTY(JigsawParameterFileIDOpus, PackedByteArray, file_id);

Ref<JigsawParameterFileIDOpus> JigsawParameterFileIDOpus::make(PackedByteArray file_id, double loop_start, double loop_end) {
	Ref<JigsawParameterFileIDOpus> param;
	param.instantiate();
	param->set_file_id(file_id);
	param->set_loop_start(loop_start);
	param->set_loop_end(loop_end);
	return param;
}
