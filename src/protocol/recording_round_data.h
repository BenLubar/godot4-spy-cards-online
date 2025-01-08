#ifndef RECORDING_ROUND_DATA_H
#define RECORDING_ROUND_DATA_H

#include "dry.h"

class RecordingRoundData;

#include "protocol/recording_round_player_data.h"

class RecordingRoundData : public Resource {
	GDCLASS(RecordingRoundData, Resource);

protected:
	static void _bind_methods();

public:
	enum RoundType {
		CHOICE = 0,
		REALTIME = 1,
	};

	DECLARE_PROPERTY(RoundType, type, = CHOICE);
	DECLARE_PROPERTY(PackedByteArray, initial_checksum);
	DECLARE_PROPERTY(TypedArray<RecordingRoundPlayerData>, player_data);
	DECLARE_PROPERTY(PackedByteArray, final_checksum); // REALTIME only
};
DECLARE_ENUM(RecordingRoundData::RoundType);

#endif // RECORDING_ROUND_DATA_H
