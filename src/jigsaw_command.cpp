#include "jigsaw_command.h"

void JigsawCommand::_bind_methods() {
	BIND_ENUM_CONSTANT(CRASH);
	BIND_ENUM_CONSTANT(COMMENT);
	BIND_ENUM_CONSTANT(REUSABLE_COMMAND_LIST);
	BIND_ENUM_CONSTANT(MATH);

	ClassDB::bind_method(D_METHOD("get_type"), &JigsawCommand::get_type);
}
