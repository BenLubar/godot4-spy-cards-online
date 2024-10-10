#ifndef JIGSAW_REUSABLE_COMMAND_LIST_H
#define JIGSAW_REUSABLE_COMMAND_LIST_H

#include "dry.h"

class JigsawReusableCommandList;

#include "jigsaw_command_list.h"
#include "jigsaw_parameter.h"

class JigsawReusableCommandList : public Resource {
	GDCLASS(JigsawReusableCommandList, Resource);

protected:
	static void _bind_methods();

public:
	JigsawReusableCommandList() = default;
	~JigsawReusableCommandList() = default;

	DECLARE_PROPERTY(Ref<JigsawCommandList>, commands);
	DECLARE_PROPERTY(String, editor_name);
	DECLARE_PROPERTY(TypedArray<JigsawParameter>, arguments);
	DECLARE_PROPERTY(PackedStringArray, argument_names);
	DECLARE_PROPERTY(TypedArray<JigsawParameter>, results);
	DECLARE_PROPERTY(PackedStringArray, result_names);
};

#endif // JIGSAW_REUSABLE_COMMAND_LIST_H
