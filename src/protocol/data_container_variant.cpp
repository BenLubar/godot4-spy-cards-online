#include "protocol/data_container.h"

bool DataContainer::_decode_godot_variant(const Ref<FormatHelper> &fh, bool &value) {
	value = fh->read_bool();
	return fh->is_valid();
}
bool DataContainer::_encode_godot_variant(const Ref<FormatHelper> &fh, const bool &value) const {
	fh->write_bool(value);
	return fh->is_valid();
}
bool DataContainer::_decode_godot_variant(const Ref<FormatHelper> &fh, PackedByteArray &value) {
	value = fh->read_bytesvar();
	return fh->is_valid();
}
bool DataContainer::_encode_godot_variant(const Ref<FormatHelper> &fh, const PackedByteArray &value) const {
	fh->write_bytesvar(value);
	return fh->is_valid();
}
bool DataContainer::_decode_godot_variant(const Ref<FormatHelper> &fh, String &value) {
	value = fh->read_stringvar();
	return fh->is_valid();
}
bool DataContainer::_encode_godot_variant(const Ref<FormatHelper> &fh, const String &value) const {
	fh->write_stringvar(value);
	return fh->is_valid();
}
bool DataContainer::_decode_godot_variant(const Ref<FormatHelper> &fh, PackedStringArray &value) {
	value.resize(fh->read_uvarint());
	for (int64_t i = 0; i < value.size(); i++) {
		value[i] = fh->read_stringvar();
	}
	return fh->is_valid();
}
bool DataContainer::_encode_godot_variant(const Ref<FormatHelper> &fh, const PackedStringArray &value) const {
	fh->write_uvarint(value.size());
	for (int64_t i = 0; i < value.size(); i++) {
		fh->write_stringvar(value[i]);
	}
	return fh->is_valid();
}
bool DataContainer::_decode_godot_variant(const Ref<FormatHelper> &fh, double &value) {
	value = fh->read_float64();
	return fh->is_valid();
}
bool DataContainer::_encode_godot_variant(const Ref<FormatHelper> &fh, const double &value) const {
	fh->write_float64(value);
	return fh->is_valid();
}
bool DataContainer::_decode_godot_variant(const Ref<FormatHelper> &fh, float &value) {
	value = fh->read_float32();
	return fh->is_valid();
}
bool DataContainer::_encode_godot_variant(const Ref<FormatHelper> &fh, const float &value) const {
	fh->write_float32(value);
	return fh->is_valid();
}
static_assert(std::is_same_v<godot::real_t, float>);
bool DataContainer::_decode_godot_variant(const Ref<FormatHelper> &fh, Vector2 &value) {
	value.x = fh->read_float32();
	value.y = fh->read_float32();
	return fh->is_valid();
}
bool DataContainer::_encode_godot_variant(const Ref<FormatHelper> &fh, const Vector2 &value) const {
	fh->write_float32(value.x);
	fh->write_float32(value.y);
	return fh->is_valid();
}
bool DataContainer::_decode_godot_variant(const Ref<FormatHelper> &fh, Vector3 &value) {
	value.x = fh->read_float32();
	value.y = fh->read_float32();
	value.z = fh->read_float32();
	return fh->is_valid();
}
bool DataContainer::_encode_godot_variant(const Ref<FormatHelper> &fh, const Vector3 &value) const {
	fh->write_float32(value.x);
	fh->write_float32(value.y);
	fh->write_float32(value.z);
	return fh->is_valid();
}
bool DataContainer::_decode_godot_variant(const Ref<FormatHelper> &fh, Vector4 &value) {
	value.x = fh->read_float32();
	value.y = fh->read_float32();
	value.z = fh->read_float32();
	value.w = fh->read_float32();
	return fh->is_valid();
}
bool DataContainer::_encode_godot_variant(const Ref<FormatHelper> &fh, const Vector4 &value) const {
	fh->write_float32(value.x);
	fh->write_float32(value.y);
	fh->write_float32(value.z);
	fh->write_float32(value.w);
	return fh->is_valid();
}
bool DataContainer::_decode_godot_variant(const Ref<FormatHelper> &fh, Color &value) {
	uint8_t alpha = fh->read_uint8();
	if (alpha == 69) { // arbitrarily chosen as an unlikely alpha value
		value.r = fh->read_float32();
		value.g = fh->read_float32();
		value.b = fh->read_float32();
		value.a = fh->read_float32();
	} else {
		value.set_r8(fh->read_uint8());
		value.set_g8(fh->read_uint8());
		value.set_b8(fh->read_uint8());
		value.set_a8(alpha);
	}
	return fh->is_valid();
}
bool DataContainer::_encode_godot_variant(const Ref<FormatHelper> &fh, const Color &value) const {
	Color value8;
	value8.set_r8(value.get_r8());
	value8.set_g8(value.get_g8());
	value8.set_b8(value.get_b8());
	value8.set_a8(value.get_a8());
	if (!value.is_equal_approx(value8) || value.get_a8() == 69) {
		fh->write_uint8(69); // arbitrarily chosen as an unlikely alpha value
		fh->write_float32(value.r);
		fh->write_float32(value.g);
		fh->write_float32(value.b);
		fh->write_float32(value.a);
	} else {
		fh->write_uint8(value.get_a8());
		fh->write_uint8(value.get_r8());
		fh->write_uint8(value.get_g8());
		fh->write_uint8(value.get_b8());
	}
	return fh->is_valid();
}
bool DataContainer::_decode_godot_variant(const Ref<FormatHelper> &fh, Transform2D &value) {
	return _decode_godot_variant(fh, value[0]) &&
		_decode_godot_variant(fh, value[1]) &&
		_decode_godot_variant(fh, value[2]);
}
bool DataContainer::_encode_godot_variant(const Ref<FormatHelper> &fh, const Transform2D &value) const {
	return _encode_godot_variant(fh, value[0]) &&
		_encode_godot_variant(fh, value[1]) &&
		_encode_godot_variant(fh, value[2]);
}
bool DataContainer::_decode_godot_variant(const Ref<FormatHelper> &fh, Transform3D &value) {
	return _decode_godot_variant(fh, value.basis) &&
		_decode_godot_variant(fh, value.origin);
}
bool DataContainer::_encode_godot_variant(const Ref<FormatHelper> &fh, const Transform3D &value) const {
	return _encode_godot_variant(fh, value.basis) &&
		_encode_godot_variant(fh, value.origin);
}
bool DataContainer::_decode_godot_variant(const Ref<FormatHelper> &fh, Basis &value) {
	return _decode_godot_variant(fh, value[0]) &&
		_decode_godot_variant(fh, value[1]) &&
		_decode_godot_variant(fh, value[2]);
}
bool DataContainer::_encode_godot_variant(const Ref<FormatHelper> &fh, const Basis &value) const {
	return _encode_godot_variant(fh, value[0]) &&
		_encode_godot_variant(fh, value[1]) &&
		_encode_godot_variant(fh, value[2]);
}
bool DataContainer::_decode_godot_variant(const Ref<FormatHelper> &fh, Rect2 &value) {
	return _decode_godot_variant(fh, value.position) &&
		_decode_godot_variant(fh, value.size);
}
bool DataContainer::_encode_godot_variant(const Ref<FormatHelper> &fh, const Rect2 &value) const {
	return _encode_godot_variant(fh, value.position) &&
		_encode_godot_variant(fh, value.size);
}
