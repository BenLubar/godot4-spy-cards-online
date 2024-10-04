#ifndef FORMAT_HELPER_H
#define FORMAT_HELPER_H

#include "dry.h"

class FormatHelper : public RefCounted {
	GDCLASS(FormatHelper, RefCounted);

protected:
	static void _bind_methods();

private:
	int64_t _reserve_write(int64_t length);

public:
	FormatHelper();
	~FormatHelper();

	static FormatHelper *read(String name, PackedByteArray buf);
	static FormatHelper *write(String name);

	DECLARE_PROPERTY(String, debug_name, = "unnamed buffer");
	DECLARE_PROPERTY(PackedByteArray, buffer);
	DECLARE_PROPERTY(int64_t, offset, = 0);
	DECLARE_PROPERTY(bool, valid, = true);

	uint8_t peek_byte(int64_t offset) const;
	int64_t remaining_len() const;
	bool is_eof() const;
	bool is_valid_eof() const;

	PackedByteArray read_bytes(int64_t count);
	void write_bytes(PackedByteArray buf);
	PackedByteArray read_bytesvar();
	void write_bytesvar(PackedByteArray buf);

	String read_string1();
	void write_string1(String s);
	String read_stringvar();
	void write_stringvar(String s);

	uint64_t read_uvarint();
	void write_uvarint(uint64_t x);
	int64_t read_svarint();
	void write_svarint(int64_t x);

	uint8_t read_uint8();
	void write_uint8(uint8_t x);
	int8_t read_sint8();
	void write_sint8(int8_t x);

	uint16_t read_uint16();
	void write_uint16(uint16_t x);
	int16_t read_sint16();
	void write_sint16(int16_t x);

	uint32_t read_uint32();
	void write_uint32(uint32_t x);
	int32_t read_sint32();
	void write_sint32(int32_t x);

	uint64_t read_uint64();
	void write_uint64(uint64_t x);
	int64_t read_sint64();
	void write_sint64(int64_t x);

	float read_float32();
	void write_float32(float x);
	double read_float64();
	void write_float64(double x);

	Color read_color_rgb888();
	void write_color_rgb888(Color c);
	bool read_bool();
	void write_bool(bool b);
};

#endif // FORMAT_HELPER_H
