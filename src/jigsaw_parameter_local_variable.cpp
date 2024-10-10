#include "jigsaw_parameter_local_variable.h"

void JigsawParameterLocalVariable::_bind_methods() {
	BIND_PROPERTY(Variant::INT, frame);
	BIND_PROPERTY(Variant::INT, slot);
}

IMPLEMENT_PROPERTY(JigsawParameterLocalVariable, int64_t, frame);
IMPLEMENT_PROPERTY(JigsawParameterLocalVariable, int64_t, slot);
