#include "jigsaw_command_reusable_command_list.h"

void JigsawCommandReusableCommandList::_bind_methods() {
	BIND_PROPERTY(Variant::INT, command_list_index);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawParameter, args);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawParameterLocalVariable, results);
}

IMPLEMENT_PROPERTY(JigsawCommandReusableCommandList, int64_t, command_list_index);
IMPLEMENT_PROPERTY(JigsawCommandReusableCommandList, TypedArray<JigsawParameter>, args);
IMPLEMENT_PROPERTY(JigsawCommandReusableCommandList, TypedArray<JigsawParameterLocalVariable>, results);
