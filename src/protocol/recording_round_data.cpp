#include "recording_round_data.h"

void RecordingRoundData::_bind_methods() {
	BIND_ENUM_CONSTANT(CHOICE);
	BIND_ENUM_CONSTANT(ACTION);
	BIND_ENUM_CONSTANT(REALTIME);

	BIND_PROPERTY_ENUM(RecordingRoundData::RoundType, type);
	BIND_PROPERTY(Variant::PACKED_BYTE_ARRAY, initial_checksum);
	BIND_PROPERTY_RESOURCE_ARRAY(RecordingRoundPlayerData, player_data);
	BIND_PROPERTY(Variant::PACKED_BYTE_ARRAY, final_checksum);
}

IMPLEMENT_PROPERTY(RecordingRoundData, RecordingRoundData::RoundType, type);
IMPLEMENT_PROPERTY(RecordingRoundData, PackedByteArray, initial_checksum);
IMPLEMENT_PROPERTY(RecordingRoundData, TypedArray<RecordingRoundPlayerData>, player_data);
IMPLEMENT_PROPERTY(RecordingRoundData, PackedByteArray, final_checksum);
