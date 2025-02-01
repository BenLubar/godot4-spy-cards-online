#include "jigsaw/parameter/jigsaw_parameter_file_id_gltf.h"

#include "util/base32.h"

void JigsawParameterFileIDGLTF::_bind_methods() {
	BIND_PROPERTY(Variant::PACKED_BYTE_ARRAY, file_id);

	ClassDB::bind_static_method("JigsawParameterFileIDGLTF", D_METHOD("make", "file_id", "json_data"), &JigsawParameterFileIDGLTF::make);
}

IMPLEMENT_PROPERTY(JigsawParameterFileIDGLTF, PackedByteArray, file_id);

String JigsawParameterFileIDGLTF::_to_string() const {
	return Base32::encode_crockford(get_file_id());
}

Ref<JigsawParameterFileIDGLTF> JigsawParameterFileIDGLTF::make(PackedByteArray file_id, Dictionary json_data) {
	Ref<JigsawParameterFileIDGLTF> param;
	param.instantiate();
	param->set_file_id(file_id);
	param->set_json_data(json_data);
	return param;
}
