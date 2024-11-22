#include "jigsaw_error.h"

#include "base32.h"
#include "format_helper.h"

void JigsawError::_bind_methods() {
	BIND_BITFIELD_FLAG(MAGIC);
	BIND_BITFIELD_FLAG(MAGIC_MASK);
	BIND_BITFIELD_FLAG(CONTAINS_GLOBAL_SNAPSHOT);

	BIND_PROPERTY(Variant::INT, flags1);
	BIND_PROPERTY(Variant::STRING, message);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawParameter, params);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawStackFrame, stack);

	ClassDB::bind_method(D_METHOD("marshal"), &JigsawError::marshal);
	ClassDB::bind_static_method("JigsawError", D_METHOD("unmarshal", "buf"), &JigsawError::unmarshal);
}

IMPLEMENT_PROPERTY(JigsawError, BitField<JigsawError::Flags1>, flags1);
IMPLEMENT_PROPERTY(JigsawError, String, message);
IMPLEMENT_PROPERTY(JigsawError, TypedArray<JigsawParameter>, params);
IMPLEMENT_PROPERTY(JigsawError, TypedArray<JigsawStackFrame>, stack);

String JigsawError::_to_string() const {
	return vformat("%cError: %s\n%s", 0x1F9E9, get_message(), Base32::encode_crockford(marshal()));
}

PackedByteArray JigsawError::marshal() const {
	Ref<FormatHelper> fh = FormatHelper::write("JigsawError::marshal");

	fh->write_uint32(get_flags1());
	fh->write_uvarint(ERROR_FORMAT_VERSION);
	fh->write_stringvar(get_message());

	return fh->get_buffer();
}

Ref<JigsawError> JigsawError::unmarshal(const PackedByteArray &buf) {
	Ref<FormatHelper> fh = FormatHelper::read("JigsawError::unmarshal", buf);

	Flags1 flags1 = static_cast<Flags1>(fh->read_uint32());
	ERR_FAIL_COND_V((flags1 & Flags1::MAGIC_MASK) != Flags1::MAGIC, Ref<JigsawError>());

	uint64_t version = fh->read_uvarint();
	ERR_FAIL_COND_V(version != ERROR_FORMAT_VERSION, Ref<JigsawError>());

	Ref<JigsawError> err;
	err.instantiate();
	err->set_flags1(flags1);
	err->set_message(fh->read_stringvar());

	ERR_FAIL_COND_V(!fh->is_valid_eof(), Ref<JigsawError>());
	return err;
}
