#ifndef JIGSAW_COMMAND_REUSABLE_COMMAND_LIST_H
#define JIGSAW_COMMAND_REUSABLE_COMMAND_LIST_H

#include "dry.h"

class JigsawCommandReusableCommandList;

#include "jigsaw_command.h"
#include "jigsaw_parameter.h"
#include "jigsaw_parameter_local_variable.h"

class JigsawCommandReusableCommandList : public JigsawCommand {
	GDCLASS(JigsawCommandReusableCommandList, JigsawCommand);

protected:
	static void _bind_methods();

public:
	JigsawCommandReusableCommandList() = default;
	~JigsawCommandReusableCommandList() = default;

	DECLARE_PROPERTY(int64_t, command_list_index, = -1);
	DECLARE_PROPERTY(TypedArray<JigsawParameter>, args);
	DECLARE_PROPERTY(TypedArray<JigsawParameterLocalVariable>, results);

	Type get_type() const override { return REUSABLE_COMMAND_LIST; }
};

#endif // JIGSAW_COMMAND_REUSABLE_COMMAND_LIST_H
