#include "jigsaw_parameter_color.h"

void JigsawParameterColor::_bind_methods() {
	BIND_PROPERTY(Variant::COLOR, color);
}

IMPLEMENT_PROPERTY(JigsawParameterColor, Color, color);
