#ifndef JIGSAW_STACK_FRAME_H
#define JIGSAW_STACK_FRAME_H

#include "dry.h"

class JigsawStackFrame;

#include "jigsaw_parameter.h"

class JigsawStackFrame : public RefCounted {
	GDCLASS(JigsawStackFrame, RefCounted);

protected:
	static void _bind_methods();

public:
	JigsawStackFrame() = default;
	~JigsawStackFrame() = default;

	DECLARE_PROPERTY(TypedArray<JigsawParameter>, local_variables);
	DECLARE_PROPERTY(int64_t, instruction_pointer, = -1);
};

#endif // JIGSAW_STACK_FRAME_H
