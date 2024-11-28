#include "protocol/embedded_data_file.h"

#include "util/format_helper.h"

#include <godot_cpp/classes/hashing_context.hpp>

void EmbeddedDataFile::_bind_methods() {
	BIND_PROPERTY(Variant::PACKED_BYTE_ARRAY, file_id);
	BIND_PROPERTY(Variant::PACKED_BYTE_ARRAY, data);
}

IMPLEMENT_PROPERTY(EmbeddedDataFile, PackedByteArray, file_id);
IMPLEMENT_PROPERTY(EmbeddedDataFile, PackedByteArray, data);

bool EmbeddedDataFile::validate_id() const {
	Ref<FormatHelper> file_id_fh = FormatHelper::read("file id", _file_id);
	PackedByteArray hash = file_id_fh->read_bytes(8);
	(void)file_id_fh->read_uvarint(); // opaque id
	int64_t file_type = file_id_fh->read_uvarint();
	ERR_FAIL_COND_V_MSG(file_type != 4 && file_type != 5 && file_type != 6, false, "invalid file type for embedded file");

	ERR_FAIL_COND_V_MSG(!file_id_fh->is_valid_eof(), false, "invalid embedded file ID");

	Ref<HashingContext> hc;
	hc.instantiate();
	hc->start(HashingContext::HASH_SHA256);
	hc->update(_data);
	PackedByteArray actual_hash = hc->finish();
	ERR_FAIL_COND_V_MSG(actual_hash.slice(0, 8) != hash, false, "embedded file failed checksum verification");

	return true;
}
