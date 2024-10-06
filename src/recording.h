#ifndef RECORDING_H
#define RECORDING_H

#include "dry.h"

#include "game_mode.h"
#include "deck.h"
#include "recording_round.h"

class Recording : public Resource {
	GDCLASS(Recording, Resource);

public:
	enum FormatVersion {
		RECORDING_0 = 0,
		RECORDING_1 = 1,
		RECORDING_2 = 2,
		RECORDING_3 = 3,
	};

protected:
	static void _bind_methods();

public:
	Recording();
	~Recording();

	DECLARE_PROPERTY(FormatVersion, format_version, = FormatVersion::RECORDING_3);
	DECLARE_PROPERTY(Vector3i, game_version);
	DECLARE_PROPERTY(String, mode_name);
	DECLARE_PROPERTY(int64_t, perspective, = 0);
	DECLARE_PROPERTY(PackedStringArray, player_name);
	DECLARE_PROPERTY(PackedInt64Array, player_character);
	DECLARE_PROPERTY(int64_t, rematch_count, = 0);
	DECLARE_PROPERTY(PackedInt64Array, won_matches);
	DECLARE_PROPERTY(int64_t, match_start_time_ms, = 0);
	DECLARE_PROPERTY(PackedByteArray, spoiler_guard_p1);
	DECLARE_PROPERTY(PackedByteArray, spoiler_guard_p2);
	DECLARE_PROPERTY(PackedByteArray, shared_seed);
	DECLARE_PROPERTY(TypedArray<PackedByteArray>, private_seeds);
	DECLARE_PROPERTY(Ref<GameMode>, mode);
	DECLARE_PROPERTY(int64_t, variant, = -1);
	DECLARE_PROPERTY(TypedArray<Deck>, initial_decks);
	DECLARE_PROPERTY(TypedArray<RecordingRound>, rounds);
};
DECLARE_ENUM(Recording::FormatVersion);

#endif // RECORDING_H
