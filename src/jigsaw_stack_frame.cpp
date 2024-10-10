#include "jigsaw_stack_frame.h"

void JigsawStackFrame::_bind_methods() {
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawParameter, local_variables);
	BIND_PROPERTY(Variant::INT, instruction_pointer);
}

IMPLEMENT_PROPERTY_SIMPLE(JigsawStackFrame, TypedArray<JigsawParameter>, local_variables);
IMPLEMENT_PROPERTY_SIMPLE(JigsawStackFrame, int64_t, instruction_pointer);
