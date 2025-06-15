#pragma once

#include "dry.h"

#include <godot_cpp/classes/hashing_context.hpp>

class RNG : public RefCounted {
	GDCLASS(RNG, RefCounted);

protected:
	static void _bind_methods();

private:
	PackedByteArray _seed;
	PackedByteArray _buf;
	Ref<HashingContext> _hash;
	constexpr static HashingContext::HashType _hash_type = HashingContext::HASH_SHA256;
	int64_t _index = 0;

public:
	RNG();
	explicit RNG(const PackedByteArray &p_seed);
	~RNG();

	DECLARE_PROPERTY(PackedByteArray, bug_separate_update_seed);
	DECLARE_PROPERTY(bool, bug_reverse_float, = false);
	DECLARE_PROPERTY(bool, bug_max_value_fencepost, = false);
	DECLARE_PROPERTY(bool, bug_max_value_shift, = false);

private:
	uint64_t _next_multi(int64_t p_num_bytes);
	uint8_t _next();

public:

	Ref<RNG> duplicate() const;
	double next_cheap_float();
	double next_float();
	int64_t next_range_int(int64_t p_min, int64_t p_max);
	double next_range_int_float(double p_min, double p_max);
	double next_range_float(double p_min, double p_max);
	int64_t fintn(double n);

	static Ref<RNG> with_seed(PackedByteArray p_seed);
};
