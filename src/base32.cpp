#include "base32.h"

void Base32::_bind_methods() {
	BIND_PROPERTY(Variant::STRING, alphabet);
	BIND_PROPERTY(Variant::STRING, padding);

	ClassDB::bind_method(D_METHOD("is_valid", "data"), &Base32::is_valid);
	ClassDB::bind_method(D_METHOD("encode", "data"), &Base32::encode);
	ClassDB::bind_method(D_METHOD("decode", "data"), &Base32::decode);

	ClassDB::bind_static_method("Base32", D_METHOD("clean_crockford", "data"), &Base32::clean_crockford);
	ClassDB::bind_static_method("Base32", D_METHOD("is_valid_cid", "data"), &Base32::is_valid_cid);
	ClassDB::bind_static_method("Base32", D_METHOD("is_valid_crockford", "data"), &Base32::is_valid_crockford);
	ClassDB::bind_static_method("Base32", D_METHOD("decode_cid", "data"), &Base32::decode_cid);
	ClassDB::bind_static_method("Base32", D_METHOD("encode_cid", "data"), &Base32::encode_cid);
	ClassDB::bind_static_method("Base32", D_METHOD("decode_crockford", "data"), &Base32::decode_crockford);
	ClassDB::bind_static_method("Base32", D_METHOD("encode_crockford", "data"), &Base32::encode_crockford);
	ClassDB::bind_static_method("Base32", D_METHOD("encoded_len", "len"), &Base32::encoded_len);
	ClassDB::bind_static_method("Base32", D_METHOD("decoded_len", "len"), &Base32::decoded_len);
}

Base32::Base32() {
	set_alphabet(alphabet);
}
Base32::Base32(const String &predefined_alphabet, const String &predefined_padding) {
	set_alphabet(predefined_alphabet);
	set_padding(predefined_padding);
}

String Base32::get_alphabet() const {
	return alphabet;
}
void Base32::set_alphabet(String new_alphabet) {
	ERR_FAIL_COND(new_alphabet.length() != 32);
	CharString new_alphabet_chars = new_alphabet.ascii();

	uint8_t new_lut[256];
	memset(new_lut, 255, sizeof(new_lut));
	for (int64_t i = 0; i < 32; i++) {
		ERR_FAIL_COND(new_alphabet_chars[i] == '\n' || new_alphabet_chars[i] == '\r');
		ERR_FAIL_COND(new_alphabet_chars[i] > 127);
		ERR_FAIL_COND(new_lut[new_alphabet_chars[i]] != 255);
		new_lut[new_alphabet_chars[i]] = i;
	}

	alphabet = new_alphabet;
	memcpy(lut, new_lut, sizeof(new_lut));
}
String Base32::get_padding() const {
	return padding;
}
void Base32::set_padding(String new_padding) {
	if (new_padding.is_empty()) {
		padding = new_padding;
		return;
	}

	ERR_FAIL_COND(new_padding.length() > 1);
	ERR_FAIL_COND(new_padding[0] == '\n' || new_padding[0] == '\r');
	ERR_FAIL_COND(new_padding[0] > 127);
	padding = new_padding;
}

bool Base32::is_valid(const String &data) const {
	String data_without_padding = data;
	if (!padding.is_empty()) {
		ERR_FAIL_COND_V(lut[padding[0]] != 255, false);
		if (data.length() % 8 != 0) {
			return false;
		}

		data_without_padding = data.rstrip(padding);
	}

	if (encoded_len(decoded_len(data_without_padding.length())) != data_without_padding.length()) {
		return false;
	}

	CharString ascii_data = data_without_padding.ascii();
	for (int64_t i = 0; i < ascii_data.length(); i++) {
		if (lut[ascii_data[i]] == 255) {
			return false;
		}
	}

	return true;
}
// encode/decode methods adapted from Go 1.23.2.
// Copyright 2011 The Go Authors. All rights reserved.
String Base32::encode(const PackedByteArray &data) const {
	// check padding validity early
	ERR_FAIL_COND_V(!padding.is_empty() && lut[padding[0]] != 255, String());

	CharString encode = alphabet.ascii();

	int64_t di = 0, si = 0;
	int64_t n = (data.size() / 5) * 5;
	PackedByteArray dst;
	dst.resize(padding.is_empty() ? encoded_len(data.size()) : (data.size() + 4) / 5 * 8);
	while (si < n) {
		uint32_t hi = (uint32_t(data[si + 0]) << 24) | (uint32_t(data[si + 1]) << 16) | (uint32_t(data[si+2]) << 8) | uint32_t(data[si + 3]);
		uint32_t lo = (hi << 8) | uint32_t(data[si + 4]);

		dst[di + 0] = encode[(hi >> 27) & 0x1f];
		dst[di + 1] = encode[(hi >> 22) & 0x1f];
		dst[di + 2] = encode[(hi >> 17) & 0x1f];
		dst[di + 3] = encode[(hi >> 12) & 0x1f];
		dst[di + 4] = encode[(hi >> 7) & 0x1f];
		dst[di + 5] = encode[(hi >> 2) & 0x1f];
		dst[di + 6] = encode[(lo >> 5) & 0x1f];
		dst[di + 7] = encode[(lo) & 0x1f];

		si += 5;
		di += 8;
	}

	// Add the remaining small block
	int64_t remain = data.size() - si;
	if (remain == 0) {
		return dst.get_string_from_ascii();
	}

	// Encode the remaining bytes in reverse order.
	uint32_t val = 0;
	switch (remain) {
	case 4:
		val |= uint32_t(data[si + 3]);
		dst[di + 6] = encode[(val << 3) & 0x1f];
		dst[di + 5] = encode[(val >> 2) & 0x1f];
		// fallthrough
	case 3:
		val |= uint32_t(data[si + 2]) << 8;
		dst[di + 4] = encode[(val >> 7) & 0x1f];
		// fallthrough
	case 2:
		val |= uint32_t(data[si + 1]) << 16;
		dst[di + 3] = encode[(val >> 12) & 0x1f];
		dst[di + 2] = encode[(val >> 17) & 0x1f];
		// fallthrough
	case 1:
		val |= uint32_t(data[si + 0]) << 24;
		dst[di + 1] = encode[(val >> 22) & 0x1f];
		dst[di + 0] = encode[(val >> 27) & 0x1f];
		break;
	}

	// Pad the final quantum
	if (!padding.is_empty()) {
		int64_t n_pad = (remain * 8 / 5) + 1;
		for (int64_t i = n_pad; i < 8; i++) {
			dst[di + i] = padding.ascii()[0];
		}
	}

	return dst.get_string_from_ascii();
}
PackedByteArray Base32::decode(const String &data) const {
	ERR_FAIL_COND_V(!is_valid(data), PackedByteArray());
	CharString src = padding.is_empty() ? data.ascii() : data.rstrip(padding).ascii();

	int64_t srci = 0;
	int64_t dsti = 0;
	int64_t olen = src.length();
	bool end = false;

	PackedByteArray dst;
	dst.resize(decoded_len(olen));

	PackedByteArray dbuf;
	dbuf.resize(8);

	while (srci < olen && !end) {
		int64_t dlen = 8;

		for (int64_t j = 0; j < 8; j++) {
			if (srci == olen) {
				dlen = j;
				end = true;
				break;
			}

			dbuf[j] = lut[src[srci]];
			ERR_FAIL_COND_V(dbuf[j] == 255, PackedByteArray());
			srci++;
		}

		if (dlen >= 8) {
			dst[dsti + 4] = (dbuf[6] << 5) | dbuf[7];
		}
		if (dlen >= 7) {
			dst[dsti + 3] = (dbuf[4] << 7) | (dbuf[5] << 2) | (dbuf[6] >> 3);
		}
		if (dlen >= 5) {
			dst[dsti + 2] = (dbuf[3] << 4) | (dbuf[4] >> 1);
		}
		if (dlen >= 4) {
			dst[dsti + 1] = (dbuf[1] << 6) | (dbuf[2] << 1) | (dbuf[3] >> 4);
		}
		if (dlen >= 2) {
			dst[dsti] = (dbuf[0] << 3) | (dbuf[1] >> 2);
		}
		dsti += 5;
	}

	return dst;
}

static Ref<Base32> Base32_crockford() {
	static Ref<Base32> encoding = memnew(Base32("0123456789ABCDEFGHJKMNPQRSTVWXYZ", ""));
	return encoding;
}
static Ref<Base32> Base32_cid() {
	static Ref<Base32> encoding = memnew(Base32("abcdefghijklmnopqrstuvwxyz234567", ""));
	return encoding;
}

String Base32::clean_crockford(const String &data) {
	String s = data.to_upper();
	s = s.replace("O", "0");
	s = s.replace("I", "1");
	s = s.replace("L", "1");
	s = s.replace(" ", "");
	s = s.replace("-", "");
	s = s.replace("_", "");
	return s;
}
bool Base32::is_valid_cid(const String &data) {
	String s = data.to_lower();
	if (!s.begins_with("b")) {
		return false;
	}

	return Base32_cid()->is_valid(s.substr(1));
}
bool Base32::is_valid_crockford(const String &data) {
	String s = clean_crockford(data);
	return Base32_crockford()->is_valid(s);
}
PackedByteArray Base32::decode_cid(const String &data) {
	String s = data.to_lower();
	ERR_FAIL_COND_V(!s.begins_with("b"), PackedByteArray());
	return Base32_cid()->decode(s.substr(1));
}
String Base32::encode_cid(const PackedByteArray &data) {
	return "b" + Base32_cid()->encode(data);
}
PackedByteArray Base32::decode_crockford(const String &data) {
	String s = clean_crockford(data);
	return Base32_crockford()->decode(s);
}
String Base32::encode_crockford(const PackedByteArray &data) {
	return Base32_crockford()->encode(data);
}
int64_t Base32::encoded_len(int64_t n) {
	return (n / 5) * 8 + (((n % 5) * 8) + 4) / 5;
}
int64_t Base32::decoded_len(int64_t n) {
	return (n / 8) * 5 + ((n % 8) * 5) / 8;
}
