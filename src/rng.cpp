#include "rng.h"

void RNG::_bind_methods() {
	BIND_PROPERTY(Variant::PACKED_BYTE_ARRAY, bug_separate_update_seed);
	BIND_PROPERTY(Variant::BOOL, bug_reverse_float);
	BIND_PROPERTY(Variant::BOOL, bug_max_value_fencepost);
	BIND_PROPERTY(Variant::BOOL, bug_max_value_shift);

	ClassDB::bind_method(D_METHOD("duplicate"), &RNG::duplicate);
	ClassDB::bind_method(D_METHOD("next_cheap_float"), &RNG::next_cheap_float);
	ClassDB::bind_method(D_METHOD("next_float"), &RNG::next_float);
	ClassDB::bind_method(D_METHOD("next_range_int", "min", "max"), &RNG::next_range_int);
	ClassDB::bind_method(D_METHOD("next_range_int_float", "min", "max"), &RNG::next_range_int_float);
	ClassDB::bind_method(D_METHOD("next_range_float", "min", "max"), &RNG::next_range_float);
	ClassDB::bind_method(D_METHOD("fintn", "n"), &RNG::fintn);

	ClassDB::bind_static_method("RNG", D_METHOD("with_seed", "seed"), &RNG::with_seed);
}

RNG::RNG() : RNG(PackedByteArray()) {}
RNG::RNG(PackedByteArray seed) {
	_seed = seed;
	_hash.instantiate();
	Error error = _hash->start(_hash_type);
	ERR_FAIL_COND(error != OK);
	if (!_seed.is_empty()) {
		error = _hash->update(_seed);
		ERR_FAIL_COND(error != OK);
	}
	_buf = _hash->finish();
}
RNG::~RNG() {}

IMPLEMENT_PROPERTY_SIMPLE(RNG, PackedByteArray, bug_separate_update_seed);
IMPLEMENT_PROPERTY_SIMPLE(RNG, bool, bug_reverse_float);
IMPLEMENT_PROPERTY_SIMPLE(RNG, bool, bug_max_value_fencepost);
IMPLEMENT_PROPERTY_SIMPLE(RNG, bool, bug_max_value_shift);

uint64_t RNG::_next_multi(int64_t num_bytes) {
	uint64_t x = 0;

	for (int64_t i = 0; i < num_bytes; i++) {
		x |= uint64_t(_next()) << (i << 3);
	}

	return x;
}
uint8_t RNG::_next() {
	if (_index >= _buf.size()) {
		Error error = _hash->start(_hash_type);
		ERR_FAIL_COND_V(error != OK, 0);
		if (!bug_separate_update_seed.is_empty()) {
			error = _hash->update(bug_separate_update_seed);
			ERR_FAIL_COND_V(error != OK, 0);
			error = _hash->update(_buf);
			ERR_FAIL_COND_V(error != OK, 0);
		} else {
			error = _hash->update(_buf);
			ERR_FAIL_COND_V(error != OK, 0);
			if (!_seed.is_empty()) {
				error = _hash->update(_seed);
				ERR_FAIL_COND_V(error != OK, 0);
			}
		}
		_buf = _hash->finish();
		_index = 0;
	}

	uint8_t n = _buf[_index];
	_index++;

	return n;
}

RNG *RNG::duplicate() const {
	RNG *dupe = memnew(RNG(_seed));
	dupe->_buf = _buf;
	dupe->_index = _index;

	dupe->bug_separate_update_seed = bug_separate_update_seed;
	dupe->bug_reverse_float = bug_reverse_float;
	dupe->bug_max_value_fencepost = bug_max_value_fencepost;
	dupe->bug_max_value_shift = bug_max_value_shift;

	return dupe;
}
double RNG::next_cheap_float() {
	return double(_next()) / 256.0;
}
double RNG::next_float() {
	if (bug_reverse_float) {
		double a = double(_next());
		double b = double(_next());
		double c = double(_next());
		double d = double(_next());

		return (((a * 256.0 + b) * 256.0 + c) * 256.0 + d) / 4294967296.0;
	}

	return double(_next_multi(4)) / 4294967296.0;
}
int64_t RNG::next_range_int(int64_t min, int64_t max) {
	int64_t diff = max - min;
	ERR_FAIL_COND_V_MSG(diff <= 0, 0, "invalid random range");

	int64_t num_bytes = 0;
	for (int64_t i = diff; i > 0; i >>= 8) {
		num_bytes++;
	}

	int64_t max_max_value = num_bytes << 3;
	if (likely(!bug_max_value_shift)) {
		max_max_value = 1 << max_max_value;
	}

	int64_t max_value = diff;
	while (max_value + diff <= max_max_value) {
		max_value += diff;
	}

	int64_t value = _next_multi(num_bytes);
	while (value >= max_value && (likely(!bug_max_value_fencepost) || value != max_max_value)) {
		value = _next_multi(num_bytes);
	}

	return (value % diff) + min;
}
double RNG::next_range_int_float(double min, double max) {
	ERR_FAIL_COND_V_MSG(!Math::is_finite(min), 0.0, "invalid random range minimum");
	ERR_FAIL_COND_V_MSG(!Math::is_finite(max), 0.0, "invalid random range maximum");
	double diff = max - min;
	ERR_FAIL_COND_V_MSG(diff <= 0.0, 0.0, "invalid random range");

	int64_t num_bytes = 0;
	for (double i = diff; i >= 1; i /= 256.0) {
		num_bytes++;
	}
	ERR_FAIL_COND_V_MSG(num_bytes > 8, 0.0, "invalid random range");

	double max_max_value = Math::pow(256.0, num_bytes);
	double max_value = diff;
	while (max_value + diff <= max_max_value) {
		max_value += diff;
	}

	double value = double(_next_multi(num_bytes));
	while (value >= max_value) {
		value = double(_next_multi(num_bytes));
	}

	return Math::fmod(value, diff) + min;
}
double RNG::next_range_float(double min, double max) {
	ERR_FAIL_COND_V_MSG(!Math::is_finite(min), 0.0, "invalid random range minimum");
	ERR_FAIL_COND_V_MSG(!Math::is_finite(max), 0.0, "invalid random range maximum");
	double diff = max - min;
	ERR_FAIL_COND_V_MSG(diff <= 0.0, 0.0, "invalid random range");

	return diff * next_float() + min;
}
int64_t RNG::fintn(double n) {
	return int64_t(Math::floor(next_float() * n));
}

Ref<RNG> RNG::with_seed(PackedByteArray seed) {
	return memnew(RNG(seed));
}
