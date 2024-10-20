#include "jigsaw_function.h"

void JigsawFunction::_bind_methods() {
	BIND_PROPERTY(Variant::STRING, editor_name);
	BIND_PROPERTY(Variant::STRING, editor_description);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawParameter, arguments);
	BIND_PROPERTY(Variant::PACKED_STRING_ARRAY, argument_names);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawParameter, results);
	BIND_PROPERTY(Variant::PACKED_STRING_ARRAY, result_names);
}

IMPLEMENT_PROPERTY(JigsawFunction, String, editor_name);
IMPLEMENT_PROPERTY(JigsawFunction, String, editor_description);
IMPLEMENT_PROPERTY(JigsawFunction, TypedArray<JigsawParameter>, arguments);
IMPLEMENT_PROPERTY(JigsawFunction, PackedStringArray, argument_names);
IMPLEMENT_PROPERTY(JigsawFunction, TypedArray<JigsawParameter>, results);
IMPLEMENT_PROPERTY(JigsawFunction, PackedStringArray, result_names);
