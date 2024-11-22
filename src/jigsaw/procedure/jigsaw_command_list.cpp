#include "jigsaw_command_list.h"

void JigsawCommandList::_bind_methods() {
	BIND_PROPERTY(Variant::PACKED_STRING_ARRAY, local_variable_names);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawParameter, local_variables);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawCommand, list);
}

IMPLEMENT_PROPERTY(JigsawCommandList, PackedStringArray, local_variable_names);
IMPLEMENT_PROPERTY(JigsawCommandList, TypedArray<JigsawParameter>, local_variables);
IMPLEMENT_PROPERTY(JigsawCommandList, TypedArray<JigsawCommand>, list);
