#include "recording_round.h"

void RecordingRound::_bind_methods() {
	BIND_PROPERTY(Variant::PACKED_BYTE_ARRAY, turn_seed);
	BIND_PROPERTY(Variant::PACKED_BYTE_ARRAY, turn_seed2);
	BIND_PROPERTY_VARIANT_ARRAY(Variant::PACKED_INT64_ARRAY, ready);
}

IMPLEMENT_PROPERTY(RecordingRound, PackedByteArray, turn_seed);
IMPLEMENT_PROPERTY(RecordingRound, PackedByteArray, turn_seed2);
IMPLEMENT_PROPERTY(RecordingRound, TypedArray<PackedInt64Array>, ready);
