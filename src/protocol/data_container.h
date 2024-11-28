#ifndef DATA_CONTAINER_H
#define DATA_CONTAINER_H

#include "dry.h"

class DataContainer;

#include "defs/game_mode_summary.h"
#include "defs/game_mode.h"
#include "protocol/embedded_data_file.h"
#include "protocol/recording_player_data.h"
#include "protocol/recording_round_data.h"
#include "util/format_helper.h"

class DataContainer : public Resource {
	GDCLASS(DataContainer, Resource);

protected:
	static void _bind_methods();

public:
	// first 8 bytes of sha256("Spy Cards Online")
	static constexpr uint64_t MAGIC = 0x300e7eb7871ca461LLU;

	enum FormatRevision {
		FORMAT_0 = 0,
	};

	enum ContainerType {
		GAME_MODE_SUMMARY = 0,
		GAME_MODE = 1,
		RECORDING = 2,
		ERROR_REPORT = 3,
	};

	// header
	DECLARE_PROPERTY(FormatRevision, format_revision, = FORMAT_0);
	DECLARE_PROPERTY(ContainerType, container_type, = GAME_MODE);
	DECLARE_PROPERTY(Vector3i, game_version);

	// shared
	DECLARE_PROPERTY(uint64_t, timestamp, = 0);
	DECLARE_PROPERTY(TypedArray<EmbeddedDataFile>, data_files);

	// game mode
	DECLARE_PROPERTY(Ref<GameModeSummary>, mode_summary);
	DECLARE_PROPERTY(Ref<GameMode>, mode);

	// recording
	DECLARE_PROPERTY(String, mode_public_name);
	DECLARE_PROPERTY(int64_t, mode_public_revision, = 0);
	DECLARE_PROPERTY(int64_t, selected_variant, = 0);
	DECLARE_PROPERTY(int64_t, rematches, = 0);
	DECLARE_PROPERTY(TypedArray<RecordingPlayerData>, player_data);
	DECLARE_PROPERTY(PackedByteArray, shared_seed);
	DECLARE_PROPERTY(TypedArray<RecordingRoundData>, rounds);
	DECLARE_PROPERTY(PackedByteArray, final_checksum);

private:
	bool _decode_game_mode_summary(const Ref<FormatHelper> &fh, const Ref<GameModeSummary> &summary);
	bool _encode_game_mode_summary(const Ref<FormatHelper> &fh, const Ref<GameModeSummary> &summary) const;
	bool _decode_game_mode(const Ref<FormatHelper> &fh);
	bool _encode_game_mode(const Ref<FormatHelper> &fh) const;
	bool _decode_recording(const Ref<FormatHelper> &fh);
	bool _encode_recording(const Ref<FormatHelper> &fh) const;
	bool _decode_error_report(const Ref<FormatHelper> &fh);
	bool _encode_error_report(const Ref<FormatHelper> &fh) const;

public:
	static Ref<DataContainer> from_byte_array(const PackedByteArray &buf, const String &debug_name = "data container");
	PackedByteArray to_byte_array(const String &debug_name = "data container") const;
};
DECLARE_ENUM(DataContainer::FormatRevision);
DECLARE_ENUM(DataContainer::ContainerType);

#endif // DATA_CONTAINER_H
