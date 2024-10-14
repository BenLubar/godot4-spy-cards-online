#ifndef JIGSAW_COMMAND_H
#define JIGSAW_COMMAND_H

#include "dry.h"

class JigsawCommand;

#include "jigsaw_context.h"
#include "jigsaw_parameter.h"
#include "jigsaw_parameter_local_variable.h"

class JigsawCommand : public Resource {
	GDCLASS(JigsawCommand, Resource);

public:
	enum Type {
		CRASH = 0,
		COMMENT = 1,
		FUNCTION = 2,
		MATH = 3,
		LOG = 4,
		FORMAT_TEXT = 5,
		IF = 6,
	};

protected:
	static void _bind_methods();

public:
	virtual Type get_type() const = 0;
};
DECLARE_ENUM(JigsawCommand::Type);

#endif // JIGSAW_COMMAND_H
