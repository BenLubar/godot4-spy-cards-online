#include "recording_round_player_data.h"

void RecordingRoundPlayerData::_bind_methods() {
	BIND_PROPERTY(Variant::PACKED_BYTE_ARRAY, personal_seed);
	BIND_PROPERTY(Variant::PACKED_INT32_ARRAY, chosen_cards);
	BIND_PROPERTY(Variant::PACKED_INT32_ARRAY, realtime_inputs);
	BIND_PROPERTY(Variant::PACKED_BYTE_ARRAY, personal_seed_after);
}

IMPLEMENT_PROPERTY(RecordingRoundPlayerData, PackedByteArray, personal_seed);
IMPLEMENT_PROPERTY(RecordingRoundPlayerData, PackedInt32Array, chosen_cards);
IMPLEMENT_PROPERTY(RecordingRoundPlayerData, PackedInt32Array, realtime_inputs);
IMPLEMENT_PROPERTY(RecordingRoundPlayerData, PackedByteArray, personal_seed_after);
