#ifndef JIGSAW_COMMAND_FUNCTION_H
#define JIGSAW_COMMAND_FUNCTION_H

#include "jigsaw_command.h"

class JigsawCommandFunction : public JigsawCommand {
	GDCLASS(JigsawCommandFunction, JigsawCommand);

protected:
	static void _bind_methods();

public:
	JigsawCommandFunction() = default;
	~JigsawCommandFunction() = default;

	DECLARE_PROPERTY(int64_t, function_index, = -1);
	DECLARE_PROPERTY(TypedArray<JigsawParameter>, args);
	DECLARE_PROPERTY(TypedArray<JigsawParameterLocalVariable>, results);

	Type get_type() const override { return FUNCTION; }
};

#endif // JIGSAW_COMMAND_FUNCTION_H
