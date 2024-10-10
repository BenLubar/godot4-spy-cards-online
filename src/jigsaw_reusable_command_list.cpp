#include "jigsaw_reusable_command_list.h"

void JigsawReusableCommandList::_bind_methods() {
	BIND_PROPERTY_RESOURCE(JigsawCommandList, commands);
	BIND_PROPERTY(Variant::STRING, editor_name);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawParameter, arguments);
	BIND_PROPERTY(Variant::PACKED_STRING_ARRAY, argument_names);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawParameter, results);
	BIND_PROPERTY(Variant::PACKED_STRING_ARRAY, result_names);
}

IMPLEMENT_PROPERTY(JigsawReusableCommandList, Ref<JigsawCommandList>, commands);
IMPLEMENT_PROPERTY(JigsawReusableCommandList, String, editor_name);
IMPLEMENT_PROPERTY(JigsawReusableCommandList, TypedArray<JigsawParameter>, arguments);
IMPLEMENT_PROPERTY(JigsawReusableCommandList, PackedStringArray, argument_names);
IMPLEMENT_PROPERTY(JigsawReusableCommandList, TypedArray<JigsawParameter>, results);
IMPLEMENT_PROPERTY(JigsawReusableCommandList, PackedStringArray, result_names);
