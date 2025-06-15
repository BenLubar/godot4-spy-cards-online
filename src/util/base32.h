#pragma once

#include "dry.h"

class Base32 : public RefCounted {
	GDCLASS(Base32, RefCounted);

protected:
	static void _bind_methods();

private:
	uint8_t _lut[256];
public:
	const static LazyGlobal<Base32> base32_crockford;
	const static LazyGlobal<Base32> base32_cid;

	Base32();
	explicit Base32(const String &p_alphabet, const String &p_padding);

	DECLARE_PROPERTY(String, alphabet, = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567");
	DECLARE_PROPERTY(String, padding, = "=");

	bool is_valid(const String &p_data) const;
	String encode(const PackedByteArray &p_data) const;
	PackedByteArray decode(const String &p_data) const;

	static String clean_crockford(const String &p_data);
	static bool is_valid_cid(const String &p_data);
	static bool is_valid_crockford(const String &p_data);
	static PackedByteArray decode_cid(const String &p_data);
	static String encode_cid(const PackedByteArray &p_data);
	static PackedByteArray decode_crockford(const String &p_data);
	static String encode_crockford(const PackedByteArray &p_data);
	static constexpr int64_t encoded_len(int64_t n) {
		return (n / 5) * 8 + (((n % 5) * 8) + 4) / 5;
	}
	static constexpr int64_t decoded_len(int64_t n) {
		return (n / 8) * 5 + ((n % 8) * 5) / 8;
	}
};
