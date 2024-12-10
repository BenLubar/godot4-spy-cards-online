#include "legacy/legacy_parse.h"

#include "protocol/button_input_history.h"

static LazyGlobal<GameMode> mite_knight{[]() -> Ref<GameMode> { return get_predefined_defs()->get("MITE_KNIGHT"); }};
static LazyGlobal<GameMode> flower_journey{[]() -> Ref<GameMode> { return get_predefined_defs()->get("FLOWER_JOURNEY"); }};

void LegacyParse::_bind_methods() {
	ClassDB::bind_static_method("LegacyParse", D_METHOD("game_mode", "buf", "name", "revision"), &LegacyParse::game_mode, DEFVAL("custom"), DEFVAL(0));
	ClassDB::bind_static_method("LegacyParse", D_METHOD("card_recording", "buf"), &LegacyParse::card_recording);
	ClassDB::bind_static_method("LegacyParse", D_METHOD("arcade_recording", "buf"), &LegacyParse::arcade_recording);
}

Ref<DataContainer> LegacyParse::game_mode(const PackedStringArray &buf, const String &name, int64_t revision) {
	ERR_FAIL_V(Ref<DataContainer>()); // TODO
}
Ref<DataContainer> LegacyParse::card_recording(const PackedByteArray &buf) {
	ERR_FAIL_V(Ref<DataContainer>()); // TODO
}
Ref<DataContainer> LegacyParse::arcade_recording(const PackedByteArray &buf) {
	Ref<FormatHelper> fh = FormatHelper::read("arcade recording", buf);

	int64_t format_version = fh->read_uvarint();
	Vector3i game_version = Vector3i(fh->read_uvarint(), fh->read_uvarint(), fh->read_uvarint());
	int64_t arcade_game = fh->read_uvarint();

	ERR_FAIL_COND_V(arcade_game != 1 && arcade_game != 2, Ref<DataContainer>());

	// we could handle rules, but none of the recorded matches have them, so just deprecate that feature altogether.
	PackedByteArray arcade_rules = fh->read_bytesvar();
	ERR_FAIL_COND_V(!arcade_rules.is_empty(), Ref<DataContainer>());

	PackedByteArray seed = fh->read_bytesvar();
	uint64_t timestamp = fh->read_uvarint();

	Ref<FormatHelper> sub_fh = FormatHelper::read("arcade recording", fh->read_bytesvar());
	PackedInt32Array unpacked_inputs;
	if (format_version == 0 && arcade_game == 2) {
		// legacy flower journey
		unpacked_inputs.resize(261);

		while (!sub_fh->is_eof()) {
			int64_t n = sub_fh->read_uvarint();
			unpacked_inputs.resize(unpacked_inputs.size() + n + 1);
			unpacked_inputs[unpacked_inputs.size() - 1] = ButtonInputHistory::CONFIRM;
		}

		unpacked_inputs[unpacked_inputs.size() - 1] = 0;
		unpacked_inputs.append(0);
	} else {
		while (!sub_fh->is_eof()) {
			int32_t buttons = sub_fh->read_uvarint();
			int64_t count = sub_fh->read_uvarint() + 1;
			for (int64_t i = 0; i < count; i++) {
				unpacked_inputs.append(buttons);
			}
		}
	}

	ERR_FAIL_COND_V(!sub_fh->is_valid_eof(), Ref<DataContainer>());

	int64_t final_rand_count = fh->read_uvarint();
	String player_name = fh->read_stringvar();

	ERR_FAIL_COND_V(!fh->is_valid_eof(), Ref<DataContainer>());

	Ref<DataContainer> recording;
	recording.instantiate();
	recording->set_container_type(DataContainer::RECORDING);
	recording->set_game_version(game_version);
	recording->set_timestamp(timestamp);
	recording->set_mode(arcade_game == 1 ? mite_knight : flower_journey);
	recording->set_mode_summary(recording->get_mode());
	recording->set_mode_public_name(arcade_game == 1 ? "miteknight" : "flowerjourney");
	recording->set_mode_public_revision(1);
	recording->set_selected_variant(0);
	recording->set_rematches(0);
	Ref<RecordingPlayerData> player_data;
	player_data.instantiate();
	player_data->set_display_name(player_name);
	player_data->set_character(enums::CharacterDef::FIRST_CUSTOM);
	recording->set_player_data(Array::make(player_data));
	recording->set_shared_seed(seed);
	Ref<RecordingRoundData> round_data;
	round_data.instantiate();
	round_data->set_type(RecordingRoundData::REALTIME);
	// TODO: compute round initial checksum
	Ref<RecordingRoundPlayerData> round_player_data;
	round_player_data.instantiate();
	round_player_data->set_realtime_inputs(unpacked_inputs);
	round_data->set_player_data(Array::make(round_player_data));
	// TODO: compute round final checksum
	recording->set_rounds(Array::make(round_data));
	// TODO: compute final checksum

	return recording;
}
