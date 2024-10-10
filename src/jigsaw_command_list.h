#ifndef JIGSAW_COMMAND_LIST_H
#define JIGSAW_COMMAND_LIST_H

#include "dry.h"

class JigsawCommandList;

#include "jigsaw_command.h"
#include "jigsaw_parameter.h"

class JigsawCommandList : public Resource {
	GDCLASS(JigsawCommandList, Resource);

protected:
	static void _bind_methods();

public:
	JigsawCommandList() = default;
	~JigsawCommandList() = default;

	DECLARE_PROPERTY(PackedStringArray, local_variable_names);
	DECLARE_PROPERTY(TypedArray<JigsawParameter>, local_variables);
	DECLARE_PROPERTY(TypedArray<JigsawCommand>, list);
};

#endif // JIGSAW_COMMAND_LIST_H
