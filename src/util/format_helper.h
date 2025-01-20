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
	static Ref<FormatHelper> read(const String &name, const PackedByteArray &buf);
	static Ref<FormatHelper> write(const String &name);

	DECLARE_PROPERTY(String, debug_name, = "unnamed buffer");
	DECLARE_PROPERTY(PackedByteArray, buffer);
	DECLARE_PROPERTY(int64_t, offset, = 0);
	DECLARE_PROPERTY_IS(bool, valid, = true);

	uint8_t peek_byte(int64_t offset) const;
	int64_t remaining_len() const;
	bool is_eof() const;
	bool is_valid_eof() const;

	PackedByteArray read_bytes(int64_t count);
	void write_bytes(const PackedByteArray &buf);
	PackedByteArray read_bytesvar();
	void write_bytesvar(const PackedByteArray &buf);

	String read_string1();
	void write_string1(const String &s);
	String read_stringvar();
	void write_stringvar(const String &s);

	uint64_t read_uvarint();
	uint32_t read_uvarint32();
	void write_uvarint(uint64_t x);
	int64_t read_svarint();
	int32_t read_svarint32();
	void write_svarint(int64_t x);

	template<typename E, typename = std::enable_if_t<std::is_enum_v<E> && E::NONE == -1>>
	E read_id();
	template<typename E, typename = std::enable_if_t<std::is_enum_v<E> && E::NONE == -1>>
	void write_id(E id);

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

	DEFAULT_TO_STRING();
};

template<typename E, typename>
E FormatHelper::read_id() {
	static_assert(std::is_enum_v<E>);
	static_assert(E::NONE == -1);

	if constexpr (sizeof(E) == 4) {
		return static_cast<E>(int32_t(read_uvarint32()) - 1);
	} else {
		return static_cast<E>(int64_t(read_uvarint()) - 1);
	}
}
template<typename E, typename>
void FormatHelper::write_id(E id) {
	static_assert(std::is_enum_v<E>);
	static_assert(E::NONE == -1);

	if constexpr (sizeof(E) == 4) {
		write_uvarint(uint32_t(int32_t(id) + 1));
	} else {
		write_uvarint(uint64_t(int64_t(id) + 1));
	}
}

#endif // FORMAT_HELPER_H
