#include "dry.h"

String _spy_cards_enum_property_hint(const String &type_name, bool is_bitfield) {
	// TODO: https://github.com/godotengine/godot-proposals/issues/12228
	if (type_name == "Key" || type_name == "KeyLocation" || type_name == "KeyModifierMask" || type_name == "MouseButton" || type_name == "MouseButtonMask" || type_name == "JoyAxis" || type_name == "JoyButton") {
		return String();
	}

	const PackedStringArray class_name_parts = type_name.split(".");
	DEV_ASSERT_ASSUME(class_name_parts.size() == 2);

	const StringName class_name = class_name_parts[0];
	const StringName enum_name = class_name_parts[1];

	DEV_ASSERT(ClassDB::class_has_enum(class_name, enum_name, true));
	DEV_ASSERT(is_bitfield == ClassDB::is_class_enum_bitfield(class_name, enum_name, true));

	const PackedStringArray enum_constants = ClassDB::class_get_enum_constants(class_name, enum_name, true);
	DEV_ASSERT(!enum_constants.is_empty());

	PackedStringArray enum_hint_parts;
	enum_hint_parts.resize(enum_constants.size());
	for (int64_t i = 0; i < enum_constants.size(); i++) {
		DEV_ASSERT(ClassDB::class_get_integer_constant_enum(class_name, enum_constants[i], true) == enum_name);
		enum_hint_parts[i] = vformat("%s:%d", enum_constants[i], ClassDB::class_get_integer_constant(class_name, enum_constants[i]));
	}

	return String(",").join(enum_hint_parts);
}

// These four varint-related functions and the comment
// following this one are derived from Go 1.24.4.
//
// Copyright 2011 The Go Authors. All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

// This file implements "varint" encoding of 64-bit integers.
// The encoding is:
// - unsigned integers are serialized 7 bits at a time, starting with the
//   least significant bits
// - the most significant bit (msb) in each output byte indicates if there
//   is a continuation byte (msb = 1)
// - signed integers are mapped to unsigned integers using "zig-zag"
//   encoding: Positive values x are written as 2*x + 0, negative values
//   are written as 2*(^x) + 1; that is, negative numbers are complemented
//   and whether to complement is encoded in bit 0.
//
// Design note:
// At most 10 bytes are needed for 64-bit values. The encoding could
// be more dense: a full 64-bit value needs an extra byte just to hold bit 63.
// Instead, the msb of the previous byte could be used to hold bit 63 since we
// know there can't be more than 64 bits. This is a trivial improvement and
// would reduce the maximum encoding length to 9 bytes. However, it breaks the
// invariant that the msb is always the "continuation bit" and thus makes the
// format incompatible with a varint encoding for larger numbers (say 128-bit).

Pair<uint64_t, int64_t> uvarint(const PackedByteArray &p_buffer, int64_t p_offset) {
	constexpr static int64_t MaxVarintLen64 = 10;

	Pair<uint64_t, int64_t> result(0, 0);

	uint_fast8_t shift = 0;
	for (int64_t i = 0; i < MaxVarintLen64; i++) {
		if (p_offset + i >= p_buffer.size()) {
			// buffer too short
			result.first = 0;
			result.second = 0;
			return result;
		}

		const uint8_t b = p_buffer[p_offset + i];
		if (b < 0x80) {
			if (i == MaxVarintLen64 - 1 && b > 1) {
				// overflow
				result.first = 0;
				result.second = -(i + 1);
				return result;
			}

			result.first |= uint64_t(b) << shift;
			result.second = i + 1;
			return result;
		}

		result.first |= uint64_t(b & 0x7f) << shift;
		shift += 7;
	}

	// overflow
	result.first = 0;
	result.second = -(MaxVarintLen64 + 1);
	return result;
}

Pair<int64_t, int64_t> svarint(const PackedByteArray &p_buffer, int64_t p_offset) {
	const Pair<uint64_t, int64_t> result = uvarint(p_buffer, p_offset); // ok to continue in presence of error
	Pair<int64_t, int64_t> signed_result(result.first >> 1, result.second);
	if (result.first & 1) {
		signed_result.first = ~signed_result.first;
	}
	return signed_result;
}

void append_uvarint(PackedByteArray &p_buffer, uint64_t p_value) {
	while (p_value >= 0x80) {
		p_buffer.append(uint8_t(p_value) | 0x80);
		p_value >>= 7;
	}

	p_buffer.append(p_value);
}

void append_svarint(PackedByteArray &p_buffer, int64_t p_value) {
	uint64_t value = uint64_t(p_value) << 1;
	if (p_value < 0) {
		value = ~value;
	}

	append_uvarint(p_buffer, value);
}
