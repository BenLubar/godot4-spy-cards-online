#include "jigsaw_parameter_color.h"

void JigsawParameterColor::_bind_methods() {
	BIND_PROPERTY(Variant::COLOR, color);

	ClassDB::bind_static_method("JigsawParameterColor", D_METHOD("make", "color"), &JigsawParameterColor::make);
}

IMPLEMENT_PROPERTY(JigsawParameterColor, Color, color);

JigsawParameterColor *JigsawParameterColor::make(Color color) {
	JigsawParameterColor *param = memnew(JigsawParameterColor);
	param->set_color(color);
	return param;
}
