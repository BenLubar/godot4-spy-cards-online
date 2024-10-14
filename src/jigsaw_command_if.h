#ifndef JIGSAW_COMMAND_IF_H
#define JIGSAW_COMMAND_IF_H

#include "jigsaw_command.h"

class JigsawCommandIf : public JigsawCommand {
	GDCLASS(JigsawCommandIf, JigsawCommand);

protected:
	static void _bind_methods();

public:
	JigsawCommandIf() = default;
	~JigsawCommandIf() = default;

	DECLARE_PROPERTY(Ref<JigsawParameter>, condition); // boolean
	DECLARE_PROPERTY(Ref<JigsawCommandList>, if_true);
	DECLARE_PROPERTY(Ref<JigsawCommandList>, if_false);

	Type get_type() const override { return IF; }
};

#endif // JIGSAW_COMMAND_IF_H
