#ifndef JIGSAW_COMMAND_LOG_H
#define JIGSAW_COMMAND_LOG_H

#include "dry.h"

class JigsawCommandLog;

#include "jigsaw_command.h"
#include "jigsaw_parameter.h"

class JigsawCommandLog : public JigsawCommand {
	GDCLASS(JigsawCommandLog, JigsawCommand);

protected:
	static void _bind_methods();

public:
	JigsawCommandLog() = default;
	~JigsawCommandLog() = default;

	DECLARE_PROPERTY(Ref<JigsawParameter>, message); // formatted_text

	Type get_type() const override { return LOG; }
};

#endif // JIGSAW_COMMAND_LOG_H
