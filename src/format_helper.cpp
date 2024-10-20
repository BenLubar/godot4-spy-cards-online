#include "format_helper.h"

void FormatHelper::_bind_methods() {
	ClassDB::bind_static_method("FormatHelper", D_METHOD("read", "name", "buf"), &FormatHelper::read);
	ClassDB::bind_static_method("FormatHelper", D_METHOD("write", "name"), &FormatHelper::write);

	BIND_PROPERTY(Variant::STRING, debug_name);
	BIND_PROPERTY(Variant::PACKED_BYTE_ARRAY, buffer);
	BIND_PROPERTY(Variant::INT, offset);
	BIND_PROPERTY(Variant::BOOL, valid);

	ClassDB::bind_method(D_METHOD("peek_byte", "offset"), &FormatHelper::peek_byte);
	ClassDB::bind_method(D_METHOD("remaining_len"), &FormatHelper::remaining_len);
	ClassDB::bind_method(D_METHOD("is_eof"), &FormatHelper::is_eof);
	ClassDB::bind_method(D_METHOD("is_valid_eof"), &FormatHelper::is_valid_eof);

	ClassDB::bind_method(D_METHOD("read_bytes", "count"), &FormatHelper::read_bytes);
	ClassDB::bind_method(D_METHOD("write_bytes", "buf"), &FormatHelper::write_bytes);
	ClassDB::bind_method(D_METHOD("read_bytesvar"), &FormatHelper::read_bytesvar);
	ClassDB::bind_method(D_METHOD("write_bytesvar", "buf"), &FormatHelper::write_bytesvar);

	ClassDB::bind_method(D_METHOD("read_string1"), &FormatHelper::read_string1);
	ClassDB::bind_method(D_METHOD("write_string1", "s"), &FormatHelper::write_string1);
	ClassDB::bind_method(D_METHOD("read_stringvar"), &FormatHelper::read_stringvar);
	ClassDB::bind_method(D_METHOD("write_stringvar", "s"), &FormatHelper::write_stringvar);

	ClassDB::bind_method(D_METHOD("read_uvarint"), &FormatHelper::read_uvarint);
	ClassDB::bind_method(D_METHOD("write_uvarint", "x"), &FormatHelper::write_uvarint);
	ClassDB::bind_method(D_METHOD("read_svarint"), &FormatHelper::read_svarint);
	ClassDB::bind_method(D_METHOD("write_svarint", "x"), &FormatHelper::write_svarint);

	ClassDB::bind_method(D_METHOD("read_uint8"), &FormatHelper::read_uint8);
	ClassDB::bind_method(D_METHOD("write_uint8", "x"), &FormatHelper::write_uint8);
	ClassDB::bind_method(D_METHOD("read_sint8"), &FormatHelper::read_sint8);
	ClassDB::bind_method(D_METHOD("write_sint8", "x"), &FormatHelper::write_sint8);

	ClassDB::bind_method(D_METHOD("read_uint16"), &FormatHelper::read_uint16);
	ClassDB::bind_method(D_METHOD("write_uint16", "x"), &FormatHelper::write_uint16);
	ClassDB::bind_method(D_METHOD("read_sint16"), &FormatHelper::read_sint16);
	ClassDB::bind_method(D_METHOD("write_sint16", "x"), &FormatHelper::write_sint16);

	ClassDB::bind_method(D_METHOD("read_uint32"), &FormatHelper::read_uint32);
	ClassDB::bind_method(D_METHOD("write_uint32", "x"), &FormatHelper::write_uint32);
	ClassDB::bind_method(D_METHOD("read_sint32"), &FormatHelper::read_sint32);
	ClassDB::bind_method(D_METHOD("write_sint32", "x"), &FormatHelper::write_sint32);

	ClassDB::bind_method(D_METHOD("read_uint64"), &FormatHelper::read_uint64);
	ClassDB::bind_method(D_METHOD("write_uint64", "x"), &FormatHelper::write_uint64);
	ClassDB::bind_method(D_METHOD("read_sint64"), &FormatHelper::read_sint64);
	ClassDB::bind_method(D_METHOD("write_sint64", "x"), &FormatHelper::write_sint64);

	ClassDB::bind_method(D_METHOD("read_float32"), &FormatHelper::read_float32);
	ClassDB::bind_method(D_METHOD("write_float32", "x"), &FormatHelper::write_float32);
	ClassDB::bind_method(D_METHOD("read_float64"), &FormatHelper::read_float64);
	ClassDB::bind_method(D_METHOD("write_float64", "x"), &FormatHelper::write_float64);

	ClassDB::bind_method(D_METHOD("read_color_rgb888"), &FormatHelper::read_color_rgb888);
	ClassDB::bind_method(D_METHOD("write_color_rgb888", "c"), &FormatHelper::write_color_rgb888);
	ClassDB::bind_method(D_METHOD("read_bool"), &FormatHelper::read_bool);
	ClassDB::bind_method(D_METHOD("write_bool", "b"), &FormatHelper::write_bool);
}

int64_t FormatHelper::_reserve_write(int64_t length) {
	int64_t off = _buffer.size();
	_buffer.resize(off + length);
	return off;
}

Ref<FormatHelper> FormatHelper::read(String name, PackedByteArray buf) {
	Ref<FormatHelper> fh;
	fh.instantiate();
	fh->set_debug_name(name);
	fh->set_buffer(buf);
	return fh;
}
Ref<FormatHelper> FormatHelper::write(String name) {
	Ref<FormatHelper> fh;
	fh.instantiate();
	fh->set_debug_name(name);
	return fh;
}

IMPLEMENT_PROPERTY_SIMPLE(FormatHelper, String, debug_name);
IMPLEMENT_PROPERTY_SIMPLE(FormatHelper, PackedByteArray, buffer);
IMPLEMENT_PROPERTY_SIMPLE(FormatHelper, int64_t, offset);
IMPLEMENT_PROPERTY_SIMPLE(FormatHelper, bool, valid);

uint8_t FormatHelper::peek_byte(int64_t offset2) const {
	ERR_FAIL_INDEX_V_MSG(_offset + offset2, _buffer.size(), 0, vformat("%s: cannot peek outside of buffer", _debug_name));
	return _buffer[_offset + offset2];
}
int64_t FormatHelper::remaining_len() const {
	return Math::max(_buffer.size() - _offset, int64_t(0));
}
bool FormatHelper::is_eof() const {
	return _buffer.size() <= _offset;
}
bool FormatHelper::is_valid_eof() const {
	return _valid && _buffer.size() == _offset;
}

PackedByteArray FormatHelper::read_bytes(int64_t count) {
	ERR_FAIL_COND_V_MSG(count < 0, PackedByteArray(), vformat("%s: count was negative", _debug_name));

	PackedByteArray slice = _buffer.slice(_offset, _offset + count);

	if (count > remaining_len()) {
		if (_valid) {
			WARN_PRINT(vformat("%s: read past end of buffer", _debug_name));
		}
		_valid = false;
		slice.resize(count);
		_offset = _buffer.size();
	} else {
		_offset += count;
	}

	return slice;
}
void FormatHelper::write_bytes(const PackedByteArray &buf) {
	_buffer.append_array(buf);
}
PackedByteArray FormatHelper::read_bytesvar() {
	int64_t count = read_uvarint();
	return read_bytes(count);
}
void FormatHelper::write_bytesvar(const PackedByteArray &buf) {
	write_uvarint(buf.size());
	write_bytes(buf);
}

String FormatHelper::read_string1() {
	int64_t length = read_uint8();
	return read_bytes(length).get_string_from_utf8();
}
void FormatHelper::write_string1(const String &s) {
	ERR_FAIL_COND_MSG(s.length() > 255, vformat("%s: string too long for write_string1", _debug_name));

	write_uint8(uint8_t(s.length()));
	write_bytes(s.to_utf8_buffer());
}
String FormatHelper::read_stringvar() {
	return read_bytesvar().get_string_from_utf8();
}
void FormatHelper::write_stringvar(const String &s) {
	write_bytesvar(s.to_utf8_buffer());
}

uint64_t FormatHelper::read_uvarint() {
	ERR_FAIL_COND_V(!_valid, 0);

	uint64_t x = 0;
	uint64_t shift = 0;

	for (int64_t i = 0; i < 10; i++) {
		uint64_t b = read_uint8();
		ERR_FAIL_COND_V(!_valid, x);

		if (b < 0x80) {
			if (i == 10 - 1 && b > 1) {
				_valid = false;
				WARN_PRINT(vformat("%s: uvarint overflow", _debug_name));
			}

			return x | (b << shift);
		}

		x |= (b & 0x7f) << shift;
		shift += 7;
	}

	_valid = false;
	WARN_PRINT(vformat("%s: uvarint overflow", _debug_name));

	return x;
}
void FormatHelper::write_uvarint(uint64_t x) {
	while (x >= 0x80) {
		_buffer.append(uint8_t(x) | 0x80u);
		x >>= 7;
	}
	_buffer.append(uint8_t(x));
}
int64_t FormatHelper::read_svarint() {
	uint64_t x = read_uvarint();
	if (x & 1) {
		return int64_t(~x) >> 1;
	}

	return int64_t(x) >> 1;
}
void FormatHelper::write_svarint(int64_t x) {
	if (x < 0) {
		write_uvarint((~uint64_t(x) << 1) | 1);
	} else {
		write_uvarint(uint64_t(x) << 1);
	}
}

uint8_t FormatHelper::read_uint8() {
	return read_bytes(1).decode_u8(0);
}
void FormatHelper::write_uint8(uint8_t x) {
	_buffer.append(x);
}
int8_t FormatHelper::read_sint8() {
	return read_bytes(1).decode_s8(0);
}
void FormatHelper::write_sint8(int8_t x) {
	_buffer.append(uint8_t(x));
}

uint16_t FormatHelper::read_uint16() {
	return read_bytes(2).decode_u16(0);
}
void FormatHelper::write_uint16(uint16_t x) {
	int64_t off = _reserve_write(2);
	_buffer.encode_u16(off, x);
}
int16_t FormatHelper::read_sint16() {
	return read_bytes(2).decode_s16(0);
}
void FormatHelper::write_sint16(int16_t x) {
	int64_t off = _reserve_write(2);
	_buffer.encode_s16(off, x);
}

uint32_t FormatHelper::read_uint32() {
	return read_bytes(4).decode_u32(0);
}
void FormatHelper::write_uint32(uint32_t x) {
	int64_t off = _reserve_write(4);
	_buffer.encode_u32(off, x);
}
int32_t FormatHelper::read_sint32() {
	return read_bytes(4).decode_s32(0);
}
void FormatHelper::write_sint32(int32_t x) {
	int64_t off = _reserve_write(4);
	_buffer.encode_s32(off, x);
}

uint64_t FormatHelper::read_uint64() {
	return read_bytes(8).decode_u64(0);
}
void FormatHelper::write_uint64(uint64_t x) {
	int64_t off = _reserve_write(8);
	_buffer.encode_u64(off, x);
}
int64_t FormatHelper::read_sint64() {
	return read_bytes(8).decode_s64(0);
}
void FormatHelper::write_sint64(int64_t x) {
	int64_t off = _reserve_write(8);
	_buffer.encode_s64(off, x);
}

float FormatHelper::read_float32() {
	return read_bytes(4).decode_float(0);
}
void FormatHelper::write_float32(float x) {
	int64_t off = _reserve_write(4);
	_buffer.encode_float(off, x);
	
}
double FormatHelper::read_float64() {
	return read_bytes(8).decode_double(0);
}
void FormatHelper::write_float64(double x) {
	int64_t off = _reserve_write(8);
	_buffer.encode_double(off, x);
}

Color FormatHelper::read_color_rgb888() {
	PackedByteArray buf = read_bytes(3);

	Color c;
	c.set_r8(buf[0]);
	c.set_g8(buf[1]);
	c.set_b8(buf[2]);

	return c;
}
void FormatHelper::write_color_rgb888(Color c) {
	int64_t off = _reserve_write(3);
	_buffer[off] = c.get_r8();
	_buffer[off + 1] = c.get_g8();
	_buffer[off + 2] = c.get_b8();
}
bool FormatHelper::read_bool() {
	uint8_t x = read_uint8();
	if (x == 0) {
		return false;
	} else if (x == 1) {
		return true;
	} else {
		_valid = false;
		WARN_PRINT(vformat("%s: out-of-range value for boolean", _debug_name));
		return true;
	}
}
void FormatHelper::write_bool(bool b) {
	_buffer.append(b ? 1 : 0);
}
