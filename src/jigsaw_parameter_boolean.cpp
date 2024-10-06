#include "jigsaw_parameter_boolean.h"

void JigsawParameterBoolean::_bind_methods() {
	BIND_PROPERTY(Variant::BOOL, boolean);

	ClassDB::bind_static_method("JigsawParameterBoolean", D_METHOD("make", "boolean"), &JigsawParameterBoolean::make);
}

IMPLEMENT_PROPERTY(JigsawParameterBoolean, bool, boolean);

JigsawParameterBoolean *JigsawParameterBoolean::make(bool boolean) {
	JigsawParameterBoolean *param = memnew(JigsawParameterBoolean);
	param->set_boolean(boolean);
	return param;
}
