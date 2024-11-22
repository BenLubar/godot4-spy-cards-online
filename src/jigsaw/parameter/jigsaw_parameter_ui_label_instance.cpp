#include "jigsaw_parameter_ui_label_instance.h"

void JigsawParameterUILabelInstance::_bind_methods() {
	BIND_PROPERTY(Variant::INT, label);
}

IMPLEMENT_PROPERTY(JigsawParameterUILabelInstance, int64_t, label);
