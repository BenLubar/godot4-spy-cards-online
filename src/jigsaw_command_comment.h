#ifndef JIGSAW_COMMAND_COMMENT_H
#define JIGSAW_COMMAND_COMMENT_H

#include "jigsaw_command.h"

class JigsawCommandComment : public JigsawCommand {
	GDCLASS(JigsawCommandComment, JigsawCommand);

protected:
	static void _bind_methods();

public:
	JigsawCommandComment() = default;
	~JigsawCommandComment() = default;

	DECLARE_PROPERTY(TypedArray<JigsawParameter>, params); // any; ignored
	DECLARE_PROPERTY(Ref<JigsawCommandList>, command_list); // optional; ignored

	Type get_type() const override { return COMMENT; }
};

#endif // JIGSAW_COMMAND_COMMENT_H
