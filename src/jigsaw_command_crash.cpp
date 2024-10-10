#include "jigsaw_command_crash.h"

void JigsawCommandCrash::_bind_methods() {
	BIND_PROPERTY_RESOURCE(JigsawParameter, message);
	BIND_PROPERTY_RESOURCE(JigsawParameter, params);
}

IMPLEMENT_PROPERTY(JigsawCommandCrash, Ref<JigsawParameter>, message);
IMPLEMENT_PROPERTY(JigsawCommandCrash, Ref<JigsawParameter>, params);
