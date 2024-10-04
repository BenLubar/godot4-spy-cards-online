#include "recording_round.h"
#include "dry.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void RecordingRound::_bind_methods() {
	BIND_PROPERTY(Variant::PACKED_BYTE_ARRAY, turn_seed);
	BIND_PROPERTY(Variant::PACKED_BYTE_ARRAY, turn_seed2);
	BIND_PROPERTY(Variant::ARRAY, ready);
}

RecordingRound::RecordingRound() {}
RecordingRound::~RecordingRound() {}

IMPLEMENT_PROPERTY(RecordingRound, PackedByteArray, turn_seed);
IMPLEMENT_PROPERTY(RecordingRound, PackedByteArray, turn_seed2);
IMPLEMENT_PROPERTY(RecordingRound, TypedArray<PackedInt64Array>, ready);
