#include "jigsaw_parameter_boolean.h"

void JigsawParameterBoolean::_bind_methods() {
	BIND_PROPERTY(Variant::BOOL, boolean);
}

IMPLEMENT_PROPERTY(JigsawParameterBoolean, bool, boolean);
