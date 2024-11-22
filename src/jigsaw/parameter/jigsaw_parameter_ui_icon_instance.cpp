#include "jigsaw_parameter_ui_icon_instance.h"

void JigsawParameterUIIconInstance::_bind_methods() {
	BIND_PROPERTY(Variant::INT, icon);
}

IMPLEMENT_PROPERTY(JigsawParameterUIIconInstance, int64_t, icon);
