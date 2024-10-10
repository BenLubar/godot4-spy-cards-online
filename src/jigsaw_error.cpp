#include "jigsaw_error.h"

void JigsawError::_bind_methods() {
	BIND_BITFIELD_FLAG(MAGIC);
	BIND_BITFIELD_FLAG(MAGIC_MASK);
	BIND_BITFIELD_FLAG(CONTAINS_GLOBAL_SNAPSHOT);

	BIND_PROPERTY(Variant::INT, flags1);
	BIND_PROPERTY(Variant::STRING, message);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawParameter, params);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawStackFrame, stack);
}

IMPLEMENT_PROPERTY(JigsawError, BitField<JigsawError::Flags1>, flags1);
IMPLEMENT_PROPERTY(JigsawError, String, message);
IMPLEMENT_PROPERTY(JigsawError, TypedArray<JigsawParameter>, params);
IMPLEMENT_PROPERTY(JigsawError, TypedArray<JigsawStackFrame>, stack);
