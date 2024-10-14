#include "jigsaw_command_function.h"

void JigsawCommandFunction::_bind_methods() {
	BIND_PROPERTY(Variant::INT, function_index);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawParameter, args);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawParameterLocalVariable, results);
}

IMPLEMENT_PROPERTY(JigsawCommandFunction, int64_t, function_index);
IMPLEMENT_PROPERTY(JigsawCommandFunction, TypedArray<JigsawParameter>, args);
IMPLEMENT_PROPERTY(JigsawCommandFunction, TypedArray<JigsawParameterLocalVariable>, results);
