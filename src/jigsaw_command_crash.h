#ifndef JIGSAW_COMMAND_CRASH_H
#define JIGSAW_COMMAND_CRASH_H

#include "dry.h"

class JigsawCommandCrash;

#include "jigsaw_command.h"
#include "jigsaw_parameter.h"

class JigsawCommandCrash : public JigsawCommand {
	GDCLASS(JigsawCommandCrash, JigsawCommand);

protected:
	static void _bind_methods();

public:
	JigsawCommandCrash() = default;
	~JigsawCommandCrash() = default;

	DECLARE_PROPERTY(Ref<JigsawParameter>, message); // string
	DECLARE_PROPERTY(Ref<JigsawParameter>, params); // ordered_list

	Type get_type() const override { return CRASH; }
};

#endif // JIGSAW_COMMAND_CRASH_H
