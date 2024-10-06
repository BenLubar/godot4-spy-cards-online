#ifndef JIGSAW_COMMAND_H
#define JIGSAW_COMMAND_H

#include "dry.h"

class JigsawCommand : public Resource {
	GDCLASS(JigsawCommand, Resource);

public:
	enum Type {
		CRASH = 0,
		COMMENT = 1,
		REUSABLE_COMMAND_LIST = 2,
		MATH = 3,
	};

protected:
	static void _bind_methods();

public:
	virtual Type get_type() const = 0;
};
DECLARE_ENUM(JigsawCommand::Type);

#endif // JIGSAW_COMMAND_H
