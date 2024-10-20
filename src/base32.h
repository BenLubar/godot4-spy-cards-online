#ifndef BASE32_H
#define BASE32_H

#include "dry.h"

class Base32 : public RefCounted {
	GDCLASS(Base32, RefCounted);

protected:
	static void _bind_methods();

private:
	uint8_t lut[256];
public:
	static LazyGlobal<Base32> base32_crockford;
	static LazyGlobal<Base32> base32_cid;

	Base32();
	explicit Base32(const String &predefined_alphabet, const String &predefined_padding);
	~Base32() = default;

	DECLARE_PROPERTY(String, alphabet, = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567");
	DECLARE_PROPERTY(String, padding, = "=");

	bool is_valid(const String &data) const;
	String encode(const PackedByteArray &data) const;
	PackedByteArray decode(const String &data) const;

	static String clean_crockford(const String &data);
	static bool is_valid_cid(const String &data);
	static bool is_valid_crockford(const String &data);
	static PackedByteArray decode_cid(const String &data);
	static String encode_cid(const PackedByteArray &data);
	static PackedByteArray decode_crockford(const String &data);
	static String encode_crockford(const PackedByteArray &data);
	static int64_t encoded_len(int64_t len);
	static int64_t decoded_len(int64_t len);
};

#endif // BASE32_H
