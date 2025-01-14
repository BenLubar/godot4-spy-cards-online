#include "legacy/legacy_parse.h"

#include "protocol/button_input_history.h"
#include "protocol/deck.h"

#include <godot_cpp/classes/marshalls.hpp>

Ref<DataContainer> LegacyParse::card_recording(const PackedByteArray &buf) {
	Ref<FormatHelper> fh = FormatHelper::read("card recording", buf);

	int64_t format_version = fh->read_uvarint();
	ERR_FAIL_COND_V(format_version > 2, Ref<DataContainer>());

	Vector3i game_version;
	game_version.x = fh->read_uvarint();
	game_version.y = fh->read_uvarint();
	game_version.z = fh->read_uvarint();
	ERR_FAIL_COND_V(game_version.x > 0, Ref<DataContainer>());
	ERR_FAIL_COND_V(game_version.y > 3, Ref<DataContainer>());
	String mode_name = fh->read_string1();
	(void)fh->read_uint8(); // ignore perspective; all it ever did was flip the screen

	String p1_character_name = fh->read_string1();
	String p2_character_name = fh->read_string1();

	int64_t rematch_count = 0;
	if (format_version >= 1) {
		rematch_count = fh->read_uvarint();
	}

	uint64_t start_time = 0;
	PackedByteArray p1_spoiler_guard;
	PackedByteArray p2_spoiler_guard;

	if (format_version >= 2) {
		int64_t won_matches = fh->read_uvarint();

		start_time = fh->read_uvarint();

		int64_t extra_fields = fh->read_uvarint();

		if ((extra_fields & (1 << 0)) != 0) {
			p1_spoiler_guard = fh->read_bytes(32);
		}

		if ((extra_fields & (1 << 1)) != 0) {
			p2_spoiler_guard = fh->read_bytes(32);
		}
	}

	PackedByteArray shared_seed = fh->read_bytes(32);
	PackedByteArray p1_private_seed = fh->read_bytes(4);
	PackedByteArray p2_private_seed = fh->read_bytes(4);

	PackedStringArray cards_raw;
	cards_raw.resize(fh->read_uvarint());
	for (int64_t i = 0; i < cards_raw.size(); i++) {
		cards_raw[i] = Marshalls::get_singleton()->raw_to_base64(fh->read_bytesvar());
	}

	Ref<DataContainer> mode;
	String mode_public_name;
	int64_t mode_public_revision = 0;
	int64_t selected_variant_index = 0;
	Ref<VariantDef> selected_variant;

	if (mode_name.is_empty()) {
		ERR_FAIL_COND_V(!cards_raw.is_empty(), Ref<DataContainer>());
	} else {
		ERR_FAIL_COND_V(cards_raw.is_empty(), Ref<DataContainer>());
		if (mode_name != "custom") {
			ERR_FAIL_COND_V(mode_name.begins_with("."), Ref<DataContainer>());
			ERR_FAIL_COND_V(mode_name.count(".") != 1, Ref<DataContainer>());
			ERR_FAIL_COND_V(!mode_name.get_slice(".", 1).is_valid_int(), Ref<DataContainer>());

			mode_public_name = mode_name.get_slice(".", 0);
			mode_public_revision = mode_name.get_slice(".", 1).to_int();
			ERR_FAIL_COND_V(mode_public_revision <= 0, Ref<DataContainer>());
		}

		if (cards_raw.size() > 1) {
			PackedByteArray variant_raw = Marshalls::get_singleton()->base64_to_raw(cards_raw[1]);
			Ref<FormatHelper> variant_fh = FormatHelper::read("card recording selected variant", variant_raw);
			selected_variant_index = variant_fh->read_uvarint();
			if (variant_fh->is_valid_eof()) {
				cards_raw.remove_at(1);
			} else {
				selected_variant_index = -1;
			}
		}
	}

	if (game_version.x == 0 && game_version.y <= 2) {
		mode = card_set(cards_raw, VANILLA_1_0_5, p1_spoiler_guard, p2_spoiler_guard);
	} else if (game_version.x == 0 && game_version.y == 3 && game_version.z <= 8) {
		mode = card_set(cards_raw, VANILLA_1_1, p1_spoiler_guard, p2_spoiler_guard);
	} else if (game_version.x == 0 && game_version.y == 3 && game_version.z <= 26) {
		mode = card_set(cards_raw, VANILLA_1_1_1, p1_spoiler_guard, p2_spoiler_guard);
	} else {
		mode = card_set(cards_raw, VANILLA_1_2_1, p1_spoiler_guard, p2_spoiler_guard);
	}

	ERR_FAIL_COND_V(mode.is_null(), Ref<DataContainer>());

	if (selected_variant_index != -1) {
		TypedArray<VariantDef> variants = mode->get_mode()->get_variants();
		ERR_FAIL_INDEX_V(selected_variant_index, variants.size(), Ref<DataContainer>());
		selected_variant = variants[selected_variant_index];
	}
	if (p1_character_name == "lanya") {
		p1_character_name = "layna";
	}
	if (p2_character_name == "lanya") {
		p2_character_name = "layna";
	}

	String p1_display_name = p1_character_name;
	enums::CharacterDef::Character p1_character = enums::CharacterDef::NONE;

	TypedArray<CharacterDef> characters = mode->get_mode()->get_characters();
	for (enums::CharacterDef::Character i = enums::CharacterDef::FIRST_CUSTOM; i < characters.size(); i = static_cast<enums::CharacterDef::Character>(i + 1)) {
		Ref<CharacterDef> character = characters[i];
		if (character->get_id() == p1_character_name) {
			p1_character = i;
			p1_display_name = character->get_display_name();
			break;
		}
	}

	String p2_display_name = p2_character_name;
	enums::CharacterDef::Character p2_character = enums::CharacterDef::NONE;
	for (enums::CharacterDef::Character i = enums::CharacterDef::FIRST_CUSTOM; i < characters.size(); i = static_cast<enums::CharacterDef::Character>(i + 1)) {
		Ref<CharacterDef> character = characters[i];
		if (character->get_id() == p2_character_name) {
			p2_character = i;
			p2_display_name = character->get_display_name();
			break;
		}
	}

	PackedByteArray packed_deck = fh->read_bytesvar();
	PackedArray<enums::CardDef::Card> p1_initial_deck;
	p1_initial_deck = Deck::decode(packed_deck);
	ERR_FAIL_COND_V(p1_initial_deck.is_empty() && !packed_deck.is_empty(), Ref<DataContainer>());

	packed_deck = fh->read_bytesvar();
	PackedArray<enums::CardDef::Card> p2_initial_deck;
	p2_initial_deck = Deck::decode(packed_deck);
	ERR_FAIL_COND_V(p2_initial_deck.is_empty() && !packed_deck.is_empty(), Ref<DataContainer>());

	// undo NPCs (legacy recordings contain their cards played)
	TypedArray<VariantDef> variants = mode->get_mode()->get_variants();
	for (int64_t i = 0; i < variants.size(); i++) {
		Ref<VariantDef> variant = variants[i];
		if (!variant->get_npcs().is_empty()) {
			variant->set_player_count(variant->get_player_count() + variant->get_npcs().size());
			variant->set_npcs(PackedArray<enums::NPCDef::NPC>());
		}
	}

	Ref<DataContainer> data;
	data.instantiate();
	data->set_format_revision(DataContainer::FORMAT_0);
	data->set_container_type(DataContainer::RECORDING);
	data->set_game_version(game_version);
	data->set_timestamp(start_time);
	data->set_data_files(mode->get_data_files());

	data->set_mode_summary(mode->get_mode());
	data->set_mode(mode->get_mode());
	data->set_mode_public_name(mode_public_name);
	data->set_mode_public_revision(mode_public_revision);
	data->set_selected_variant(selected_variant_index);

	data->set_rematches(rematch_count);

	Ref<RecordingPlayerData> player_1_data;
	player_1_data.instantiate();
	player_1_data->set_display_name(p1_display_name);
	player_1_data->set_character(p1_character);
	player_1_data->set_initial_deck(p1_initial_deck);

	Ref<RecordingPlayerData> player_2_data;
	player_2_data.instantiate();
	player_2_data->set_display_name(p2_display_name);
	player_2_data->set_character(p2_character);
	player_2_data->set_initial_deck(p2_initial_deck);

	data->set_player_data(Array::make(player_1_data, player_2_data));

	// this is very incorrect, but the recording player knows how to split this back out.
	PackedByteArray initial_seeds;
	initial_seeds.append_array(shared_seed);
	initial_seeds.append_array(p1_private_seed);
	initial_seeds.append_array(p2_private_seed);
	if (!p1_spoiler_guard.is_empty() || !p2_spoiler_guard.is_empty()) {
		PackedByteArray all_ones;
		all_ones.resize(32);
		all_ones.fill(255);

		if (p1_spoiler_guard.is_empty()) {
			initial_seeds.append_array(all_ones);
		} else {
			initial_seeds.append_array(p1_spoiler_guard);
		}

		if (p2_spoiler_guard.is_empty()) {
			initial_seeds.append_array(all_ones);
		} else {
			initial_seeds.append_array(p2_spoiler_guard);
		}
	}
	data->set_shared_seed(initial_seeds);

	TypedArray<RecordingRoundData> rounds;
	rounds.resize(fh->read_uvarint());
	for (int64_t i = 0; i < rounds.size(); i++) {
		Ref<RecordingRoundData> round;
		round.instantiate();
		round->set_type(RecordingRoundData::CHOICE);

		// we're abusing the per-player seed information fields as what used to be 1 or 2 global seeds; the recording player understands this.
		Ref<RecordingRoundPlayerData> player_1_round_data;
		player_1_round_data.instantiate();
		player_1_round_data->set_personal_seed(fh->read_bytes(8));

		Ref<RecordingRoundPlayerData> player_2_round_data;
		player_2_round_data.instantiate();
		if (format_version >= 1) {
			player_2_round_data->set_personal_seed(fh->read_bytes(8));
		}

		uint64_t player_1_ready = fh->read_uvarint();
		PackedInt64Array player_1_choices;
		for (int64_t j = 0; j < 64; j++) {
			if (player_1_ready & (1 << j)) {
				player_1_choices.append(j);
			}
		}
		player_1_round_data->set_chosen_cards(player_1_choices);

		uint64_t player_2_ready = fh->read_uvarint();
		PackedInt64Array player_2_choices;
		for (int64_t j = 0; j < 64; j++) {
			if (player_2_ready & (1 << j)) {
				player_2_choices.append(j);
			}
		}
		player_2_round_data->set_chosen_cards(player_2_choices);

		round->set_player_data(Array::make(player_1_round_data, player_2_round_data));
	}

	data->set_rounds(rounds);

	// TODO: compute final checksum

	return data;
}
Ref<DataContainer> LegacyParse::arcade_recording(const PackedByteArray &buf) {
	Ref<FormatHelper> fh = FormatHelper::read("arcade recording", buf);

	int64_t format_version = fh->read_uvarint();
	Vector3i game_version;
	game_version.x = fh->read_uvarint();
	game_version.y = fh->read_uvarint();
	game_version.z = fh->read_uvarint();
	ERR_FAIL_COND_V(game_version.x > 0, Ref<DataContainer>());
	ERR_FAIL_COND_V(game_version.y > 3, Ref<DataContainer>());
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
	recording->set_format_revision(DataContainer::FORMAT_0);
	recording->set_container_type(DataContainer::RECORDING);
	recording->set_game_version(game_version);
	recording->set_timestamp(timestamp);
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
