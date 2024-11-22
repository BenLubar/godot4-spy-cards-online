#ifndef JIGSAW_ERROR_H
#define JIGSAW_ERROR_H

#include "dry.h"

class JigsawError;

#include "jigsaw/parameter/jigsaw_parameter.h"
#include "jigsaw/jigsaw_stack_frame.h"

class JigsawError : public Resource {
	GDCLASS(JigsawError, Resource);
	friend class JigsawContext;

public:
	// only the current error format version can be read
	static constexpr uint64_t ERROR_FORMAT_VERSION = 0;

	enum Flags1 : uint32_t {
		MAGIC = 0x80981566, // spells CRASH in Crockford base32
		MAGIC_MASK = 0x80ffffff,
		CONTAINS_GLOBAL_SNAPSHOT = 0x1000000,
		//UNUSED = 0x2000000,
		//UNUSED = 0x4000000,
		//UNUSED = 0x8000000,
	};

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(BitField<Flags1>, flags1, = MAGIC);
	DECLARE_PROPERTY(String, message);
	DECLARE_PROPERTY(TypedArray<JigsawParameter>, params);
	DECLARE_PROPERTY(TypedArray<JigsawStackFrame>, stack);

	String _to_string() const;
	PackedByteArray marshal() const;

	static Ref<JigsawError> unmarshal(const PackedByteArray &buf);
};
VARIANT_BITFIELD_CAST(JigsawError::Flags1);

#endif // JIGSAW_ERROR_H
