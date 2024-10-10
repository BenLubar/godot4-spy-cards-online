#include "jigsaw_command_if.h"

void JigsawCommandIf::_bind_methods() {
	BIND_PROPERTY_RESOURCE(JigsawParameter, condition);
	BIND_PROPERTY_RESOURCE(JigsawCommandList, if_true);
	BIND_PROPERTY_RESOURCE(JigsawCommandList, if_false);
}

IMPLEMENT_PROPERTY(JigsawCommandIf, Ref<JigsawParameter>, condition);
IMPLEMENT_PROPERTY(JigsawCommandIf, Ref<JigsawCommandList>, if_true);
IMPLEMENT_PROPERTY(JigsawCommandIf, Ref<JigsawCommandList>, if_false);
