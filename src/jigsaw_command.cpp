#include "jigsaw_command.h"

void JigsawCommand::_bind_methods() {
	BIND_ENUM_CONSTANT(CRASH);
	BIND_ENUM_CONSTANT(COMMENT);
	BIND_ENUM_CONSTANT(REUSABLE_COMMAND_LIST);
	BIND_ENUM_CONSTANT(MATH);
	BIND_ENUM_CONSTANT(LOG);
	BIND_ENUM_CONSTANT(FORMAT_TEXT);
	BIND_ENUM_CONSTANT(IF);

	ClassDB::bind_method(D_METHOD("get_type"), &JigsawCommand::get_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "type"), "", "get_type");
}
