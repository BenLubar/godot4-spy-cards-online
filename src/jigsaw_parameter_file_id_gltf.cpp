#include "jigsaw_parameter_file_id_gltf.h"

void JigsawParameterFileIDGLTF::_bind_methods() {
	BIND_PROPERTY(Variant::PACKED_BYTE_ARRAY, file_id);

	ClassDB::bind_static_method("JigsawParameterFileIDGLTF", D_METHOD("make", "file_id", "json_data"), &JigsawParameterFileIDGLTF::make);
}

IMPLEMENT_PROPERTY(JigsawParameterFileIDGLTF, PackedByteArray, file_id);

JigsawParameterFileIDGLTF *JigsawParameterFileIDGLTF::make(PackedByteArray file_id, Dictionary json_data) {
	JigsawParameterFileIDGLTF *param = memnew(JigsawParameterFileIDGLTF);
	param->set_file_id(file_id);
	param->set_json_data(json_data);
	return param;
}
