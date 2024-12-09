#include "jigsaw/parameter/jigsaw_parameter_ui_label_instance.h"

void JigsawParameterUILabelInstance::_bind_methods() {
	BIND_PROPERTY(Variant::INT, label);

	ClassDB::bind_static_method("JigsawParameterUILabelInstance", D_METHOD("make", "label"), &JigsawParameterUILabelInstance::make);
}

IMPLEMENT_PROPERTY(JigsawParameterUILabelInstance, int64_t, label);

Ref<JigsawParameterUILabelInstance> JigsawParameterUILabelInstance::make(int64_t label) {
	Ref<JigsawParameterUILabelInstance> param;
	param.instantiate();
	param->set_label(label);
	return param;
}
