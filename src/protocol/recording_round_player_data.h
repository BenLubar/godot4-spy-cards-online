#ifndef RECORDING_ROUND_PLAYER_DATA_H
#define RECORDING_ROUND_PLAYER_DATA_H

#include "dry.h"

class RecordingRoundPlayerData : public Resource {
	GDCLASS(RecordingRoundPlayerData, Resource);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(PackedByteArray, personal_seed);

	// CHOICE only:
	DECLARE_PROPERTY(PackedInt64Array, chosen_cards);

	// REALTIME only:
	DECLARE_PROPERTY(PackedByteArray, realtime_inputs);
	DECLARE_PROPERTY(PackedByteArray, personal_seed_after);
};

#endif // RECORDING_ROUND_PLAYER_DATA_H
