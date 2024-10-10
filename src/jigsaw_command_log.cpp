#include "jigsaw_command_log.h"

void JigsawCommandLog::_bind_methods() {
	BIND_PROPERTY_RESOURCE(JigsawParameter, message);
}

IMPLEMENT_PROPERTY(JigsawCommandLog, Ref<JigsawParameter>, message);
