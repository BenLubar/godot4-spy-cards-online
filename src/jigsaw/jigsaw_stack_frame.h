#ifndef JIGSAW_STACK_FRAME_H
#define JIGSAW_STACK_FRAME_H

#include "dry.h"

class JigsawStackFrame;

#include "jigsaw/procedure/jigsaw_command_list.h"
#include "jigsaw/parameter/jigsaw_parameter.h"

class JigsawStackFrame : public RefCounted {
	GDCLASS(JigsawStackFrame, RefCounted);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(int64_t, branch, = 0);
	DECLARE_PROPERTY(Ref<JigsawCommandList>, commands);
	DECLARE_PROPERTY(TypedArray<JigsawParameter>, local_variables);
	DECLARE_PROPERTY(int64_t, instruction_pointer, = -1);

	DEFAULT_TO_STRING();
};

#endif // JIGSAW_STACK_FRAME_H
