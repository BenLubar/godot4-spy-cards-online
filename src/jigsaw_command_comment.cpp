#include "jigsaw_command_comment.h"

void JigsawCommandComment::_bind_methods() {
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawParameter, params);
	BIND_PROPERTY_RESOURCE(JigsawCommandList, command_list);
}

IMPLEMENT_PROPERTY(JigsawCommandComment, TypedArray<JigsawParameter>, params);
IMPLEMENT_PROPERTY(JigsawCommandComment, Ref<JigsawCommandList>, command_list);
