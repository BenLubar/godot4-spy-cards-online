#include "jigsaw_parameter_icon.h"

void JigsawParameterIcon::_bind_methods() {
	BIND_PROPERTY(Variant::INT, icon);

	ClassDB::bind_static_method("JigsawParameterIcon", D_METHOD("make", "icon"), &JigsawParameterIcon::make);
}

IMPLEMENT_PROPERTY(JigsawParameterIcon, IconDef::Icon, icon);

Ref<JigsawParameterIcon> JigsawParameterIcon::make(IconDef::Icon icon) {
	Ref<JigsawParameterIcon> param;
	param.instantiate();
	param->set_icon(icon);
	return param;
}
