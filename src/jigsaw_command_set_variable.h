#ifndef JIGSAW_COMMAND_SET_VARIABLE_H
#define JIGSAW_COMMAND_SET_VARIABLE_H

#include "jigsaw_command.h"

class JigsawCommandSetVariable : public JigsawCommand {
	GDCLASS(JigsawCommandSetVariable, JigsawCommand);

protected:
	static void _bind_methods();

	Type get_type() const override { return SET_VARIABLE; }
};

#endif // JIGSAW_COMMAND_SET_VARIABLE_H
