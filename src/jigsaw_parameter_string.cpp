#include "jigsaw_parameter_string.h"

void JigsawParameterString::_bind_methods() {
	BIND_PROPERTY(Variant::STRING, string);
}

IMPLEMENT_PROPERTY(JigsawParameterString, String, string);
