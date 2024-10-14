#include "recording.h"

void Recording::_bind_methods() {
	BIND_ENUM_CONSTANT(RECORDING_0);
	BIND_ENUM_CONSTANT(RECORDING_1);
	BIND_ENUM_CONSTANT(RECORDING_2);
	BIND_ENUM_CONSTANT(RECORDING_3);

	BIND_PROPERTY_ENUM(Recording::FormatVersion, format_version);
	BIND_PROPERTY(Variant::VECTOR3I, game_version);
	BIND_PROPERTY(Variant::STRING, mode_name);
	BIND_PROPERTY(Variant::INT, perspective);
	BIND_PROPERTY(Variant::PACKED_STRING_ARRAY, player_name);
	BIND_PROPERTY(Variant::PACKED_INT64_ARRAY, player_character);
	BIND_PROPERTY(Variant::INT, rematch_count);
	BIND_PROPERTY(Variant::PACKED_INT64_ARRAY, won_matches);
	BIND_PROPERTY(Variant::INT, match_start_time_ms);
	BIND_PROPERTY(Variant::PACKED_BYTE_ARRAY, spoiler_guard_p1);
	BIND_PROPERTY(Variant::PACKED_BYTE_ARRAY, spoiler_guard_p2);
	BIND_PROPERTY(Variant::PACKED_BYTE_ARRAY, shared_seed);
	BIND_PROPERTY_VARIANT_ARRAY(Variant::PACKED_BYTE_ARRAY, private_seeds);
	BIND_PROPERTY_RESOURCE(GameMode, mode);
	BIND_PROPERTY(Variant::INT, variant);
	BIND_PROPERTY_VARIANT_ARRAY(Variant::PACKED_INT64_ARRAY, initial_decks);
	BIND_PROPERTY_RESOURCE_ARRAY(RecordingRound, rounds);
}

IMPLEMENT_PROPERTY(Recording, Recording::FormatVersion, format_version);
IMPLEMENT_PROPERTY(Recording, Vector3i, game_version);
IMPLEMENT_PROPERTY(Recording, String, mode_name);
IMPLEMENT_PROPERTY(Recording, int64_t, perspective);
IMPLEMENT_PROPERTY(Recording, PackedStringArray, player_name);
IMPLEMENT_PROPERTY(Recording, PackedInt64Array, player_character);
IMPLEMENT_PROPERTY(Recording, int64_t, rematch_count);
IMPLEMENT_PROPERTY(Recording, PackedInt64Array, won_matches);
IMPLEMENT_PROPERTY(Recording, int64_t, match_start_time_ms);
IMPLEMENT_PROPERTY(Recording, PackedByteArray, spoiler_guard_p1);
IMPLEMENT_PROPERTY(Recording, PackedByteArray, spoiler_guard_p2);
IMPLEMENT_PROPERTY(Recording, PackedByteArray, shared_seed);
IMPLEMENT_PROPERTY(Recording, TypedArray<PackedByteArray>, private_seeds);
IMPLEMENT_PROPERTY(Recording, Ref<GameMode>, mode);
IMPLEMENT_PROPERTY(Recording, int64_t, variant);
IMPLEMENT_PROPERTY(Recording, TypedArray<PackedInt64Array>, initial_decks);
IMPLEMENT_PROPERTY(Recording, TypedArray<RecordingRound>, rounds);
