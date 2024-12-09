#include "jigsaw/parameter/jigsaw_parameter_ui_icon_instance.h"

void JigsawParameterUIIconInstance::_bind_methods() {
	BIND_PROPERTY(Variant::INT, icon);

	ClassDB::bind_static_method("JigsawParameterUIIconInstance", D_METHOD("make", "icon"), &JigsawParameterUIIconInstance::make);
}

IMPLEMENT_PROPERTY(JigsawParameterUIIconInstance, int64_t, icon);

Ref<JigsawParameterUIIconInstance> JigsawParameterUIIconInstance::make(int64_t icon) {
	Ref<JigsawParameterUIIconInstance> param;
	param.instantiate();
	param->set_icon(icon);
	return param;
}
