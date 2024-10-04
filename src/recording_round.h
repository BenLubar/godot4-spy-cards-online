#ifndef RECORDING_ROUND_H
#define RECORDING_ROUND_H

#include "dry.h"

class RecordingRound : public Resource {
	GDCLASS(RecordingRound, Resource);

protected:
	static void _bind_methods();

public:
	RecordingRound();
	~RecordingRound();

	DECLARE_PROPERTY(PackedByteArray, turn_seed);
	DECLARE_PROPERTY(PackedByteArray, turn_seed2);
	DECLARE_PROPERTY(TypedArray<PackedInt64Array>, ready);
};

#endif // RECORDING_ROUND_H
