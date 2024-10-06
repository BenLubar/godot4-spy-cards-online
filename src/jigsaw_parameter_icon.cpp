#include "jigsaw_parameter_icon.h"

void JigsawParameterIcon::_bind_methods() {
	BIND_PROPERTY(Variant::INT, icon);
}

IMPLEMENT_PROPERTY(JigsawParameterIcon, IconDef::Icon, icon);
