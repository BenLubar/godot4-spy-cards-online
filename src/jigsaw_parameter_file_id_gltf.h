#ifndef JIGSAW_PARAMETER_FILE_ID_GLTF_H
#define JIGSAW_PARAMETER_FILE_ID_GLTF_H

#include "jigsaw_parameter_scene.h"

class JigsawParameterFileIDGLTF : public JigsawParameterScene {
	GDCLASS(JigsawParameterFileIDGLTF, JigsawParameterScene);

protected:
	static void _bind_methods();

public:
	JigsawParameterFileIDGLTF() = default;
	~JigsawParameterFileIDGLTF() = default;

	DECLARE_PROPERTY(PackedByteArray, file_id);

	Type get_type() const override { return FILE_ID_GLTF; }

	static Ref<JigsawParameterFileIDGLTF> make(PackedByteArray file_id, Dictionary json_data);
};

#endif // JIGSAW_PARAMETER_FILE_ID_GLTF_H
