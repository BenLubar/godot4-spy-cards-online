#include "protocol/data_container.h"

#include "util/why_isnt_this_in_godot.h"
#include "protocol/button_input_history.h"
#include "protocol/deck.h"

#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/hashing_context.hpp>
#include <godot_cpp/templates/local_vector.hpp>

void DataContainer::_bind_methods() {
	BIND_CONSTANT(MAGIC);

	BIND_ENUM_CONSTANT(FORMAT_0);

	BIND_ENUM_CONSTANT(GAME_MODE_SUMMARY);
	BIND_ENUM_CONSTANT(GAME_MODE);
	BIND_ENUM_CONSTANT(RECORDING);
	BIND_ENUM_CONSTANT(ERROR_REPORT);

	BIND_PROPERTY_ENUM(DataContainer::FormatRevision, format_revision);
	BIND_PROPERTY_ENUM(DataContainer::ContainerType, container_type);
	BIND_PROPERTY(Variant::VECTOR3I, game_version);

	BIND_PROPERTY(Variant::INT, timestamp);
	BIND_PROPERTY_RESOURCE_ARRAY(EmbeddedDataFile, data_files);

	BIND_PROPERTY_RESOURCE(GameModeSummary, mode_summary);
	BIND_PROPERTY_RESOURCE(GameMode, mode);

	BIND_PROPERTY(Variant::STRING, mode_public_name);
	BIND_PROPERTY(Variant::INT, mode_public_revision);
	BIND_PROPERTY(Variant::INT, selected_variant);
	BIND_PROPERTY(Variant::INT, rematches);
	BIND_PROPERTY(Variant::PACKED_INT64_ARRAY, previous_wins);
	BIND_PROPERTY(Variant::PACKED_BYTE_ARRAY, resumed_from_recording);
	BIND_PROPERTY(Variant::INT, resumed_from_round);
	BIND_PROPERTY_PACKED_ENUM_ARRAY(enums::NPCDef::NPC, player_npc);
	BIND_PROPERTY_RESOURCE_ARRAY(RecordingPlayerData, player_data);
	BIND_PROPERTY(Variant::PACKED_BYTE_ARRAY, shared_seed);
	BIND_PROPERTY_RESOURCE_ARRAY(RecordingRoundData, rounds);
	BIND_PROPERTY(Variant::PACKED_BYTE_ARRAY, final_checksum);

	ClassDB::bind_static_method("DataContainer", D_METHOD("from_byte_array", "buf", "debug_name"), &DataContainer::from_byte_array, DEFVAL("data container"));
	ClassDB::bind_method(D_METHOD("to_byte_array", "debug_name"), &DataContainer::to_byte_array, DEFVAL("data container"));
	ClassDB::bind_method(D_METHOD("validate"), &DataContainer::validate);
}

IMPLEMENT_PROPERTY(DataContainer, DataContainer::FormatRevision, format_revision);
IMPLEMENT_PROPERTY(DataContainer, DataContainer::ContainerType, container_type);
IMPLEMENT_PROPERTY(DataContainer, Vector3i, game_version);

IMPLEMENT_PROPERTY(DataContainer, uint64_t, timestamp);
IMPLEMENT_PROPERTY(DataContainer, TypedArray<EmbeddedDataFile>, data_files);

IMPLEMENT_PROPERTY(DataContainer, Ref<GameModeSummary>, mode_summary);
IMPLEMENT_PROPERTY(DataContainer, Ref<GameMode>, mode);

IMPLEMENT_PROPERTY(DataContainer, String, mode_public_name);
IMPLEMENT_PROPERTY(DataContainer, int64_t, mode_public_revision);
IMPLEMENT_PROPERTY(DataContainer, int64_t, selected_variant);
IMPLEMENT_PROPERTY(DataContainer, int64_t, rematches);
IMPLEMENT_PROPERTY(DataContainer, PackedInt64Array, previous_wins);
IMPLEMENT_PROPERTY(DataContainer, PackedByteArray, resumed_from_recording);
IMPLEMENT_PROPERTY(DataContainer, int64_t, resumed_from_round);
IMPLEMENT_PROPERTY(DataContainer, PackedArray<enums::NPCDef::NPC>, player_npc);
IMPLEMENT_PROPERTY(DataContainer, TypedArray<RecordingPlayerData>, player_data);
IMPLEMENT_PROPERTY(DataContainer, PackedByteArray, shared_seed);
IMPLEMENT_PROPERTY(DataContainer, TypedArray<RecordingRoundData>, rounds);
IMPLEMENT_PROPERTY(DataContainer, PackedByteArray, final_checksum);

bool DataContainer::_validate_file_id(const PackedByteArray &file_id, uint64_t remote_type, uint64_t local_type) const {
	Ref<FormatHelper> fh = FormatHelper::read("file id", file_id);
	(void)fh->read_bytes(8); // hash
	(void)fh->read_uvarint(); // opaque id

	uint64_t type = 0;
	if (!fh->is_eof()) {
		type = fh->read_uvarint();
		ERR_FAIL_COND_V(type == 0, false);
	}

	ERR_FAIL_COND_V(!fh->is_valid_eof(), false);

	if (likely(type == remote_type)) {
		// assume remote objects are always good so we don't have to do a network request or have validation depend on the cache.
		return true;
	}

	// redundant condition because it makes the error message nicer.
	ERR_FAIL_COND_V(type != remote_type && type != local_type, false);

	// if the file ID is a local file (which it is at this point in the function), it must be included in the embedded files list.
	for (int64_t i = 0; i < _data_files.size(); i++) {
		Ref<EmbeddedDataFile> file = _data_files[i];
		if (file->get_file_id() == file_id) {
			return true;
		}
	}

	ERR_FAIL_V_MSG(false, "local file ID not found in data container");
}
bool DataContainer::_decode_game_mode_summary(const Ref<FormatHelper> &fh, const Ref<GameModeSummary> &summary) {
	summary->set_title(fh->read_stringvar());
	summary->set_author(fh->read_stringvar());
	summary->set_mode_thumbnail(fh->read_id<enums::IconDef::Icon>());
	summary->set_description(fh->read_stringvar());
	summary->set_latest_changes(fh->read_stringvar());

	TypedArray<IconDef> custom_icons;
	custom_icons.resize(fh->read_uvarint());
	for (int64_t i = 0; i < custom_icons.size(); i++) {
		Ref<IconDef> custom_icon;
		custom_icon.instantiate();

		custom_icon->set_file_id(fh->read_bytesvar());
		ERR_FAIL_COND_V(!_validate_file_id(custom_icon->get_file_id(), 0, 4), false);

		custom_icons[i] = custom_icon;
	}
	summary->set_custom_icons(custom_icons);

	Ref<CardDesign> card_design;
	if (fh->read_bool()) {
		card_design.instantiate();
		ERR_FAIL_COND_V(!_decode_card_design(fh, card_design), false);
	}
	summary->set_mode_list_card_design(card_design);

	return true;
}
bool DataContainer::_encode_game_mode_summary(const Ref<FormatHelper> &fh, const Ref<GameModeSummary> &summary) const {
	fh->write_stringvar(summary->get_title());
	fh->write_stringvar(summary->get_author());
	fh->write_id(summary->get_mode_thumbnail());
	fh->write_stringvar(summary->get_description());
	fh->write_stringvar(summary->get_latest_changes());

	TypedArray<IconDef> custom_icons = summary->get_custom_icons();
	fh->write_uvarint(custom_icons.size());
	for (int64_t i = 0; i < custom_icons.size(); i++) {
		Ref<IconDef> custom_icon = custom_icons[i];
		ERR_FAIL_COND_V(!_validate_file_id(custom_icon->get_file_id(), 0, 4), false);
		fh->write_bytesvar(custom_icon->get_file_id());
	}

	Ref<CardDesign> card_design = summary->get_mode_list_card_design();
	fh->write_bool(card_design.is_valid());
	if (card_design.is_valid()) {
		ERR_FAIL_COND_V(!_encode_card_design(fh, card_design), false);
	}

	return true;
}
template<typename E>
static bool _decode_sorted_id_list(const Ref<FormatHelper> &fh, PackedArray<E> &list) {
	list.resize(fh->read_uvarint());

	E value = E::NONE;
	for (int64_t i = 0; i < list.size(); i++) {
		value = static_cast<E>(value + 1 + fh->read_uvarint());
		list[i] = value;
	}

	return fh->is_valid();
}
bool DataContainer::_decode_game_mode(const Ref<FormatHelper> &fh) {
	_mode.instantiate();
	_mode_summary = _mode;
	ERR_FAIL_COND_V(!_decode_game_mode_summary(fh, _mode), false);

	TypedArray<AudienceDef> audience;
	audience.resize(fh->read_uvarint());
	for (int64_t i = 0; i < audience.size(); i++) {
		Ref<AudienceDef> member;
		member.instantiate();
		ERR_FAIL_COND_V(!_decode_audience_def(fh, member), false);
		audience[i] = member;
	}
	_mode->set_audience(audience);
	TypedArray<CharacterDef> characters;
	characters.resize(fh->read_uvarint());
	for (int64_t i = 0; i < characters.size(); i++) {
		Ref<CharacterDef> character;
		character.instantiate();
		ERR_FAIL_COND_V(!_decode_character_def(fh, character), false);
		characters[i] = character;
	}
	_mode->set_characters(characters);

	Ref<CardDesign> design;
	design.instantiate();
	ERR_FAIL_COND_V(!_decode_card_design(fh, design), false);
	_mode->set_default_card_design(design);

	TypedArray<JigsawTriggerVariant> base_triggers;
	base_triggers.resize(fh->read_uvarint());
	for (int64_t i = 0; i < base_triggers.size(); i++) {
		Ref<JigsawTriggerVariant> trigger;
		ERR_FAIL_COND_V(!_decode_jigsaw_trigger_variant(fh, trigger), false);
		ERR_FAIL_COND_V(trigger.is_null(), false);
		base_triggers[i] = trigger;
	}
	_mode->set_base_triggers(base_triggers);

	TypedArray<VariantDef> variants;
	variants.resize(fh->read_uvarint());
	for (int64_t i = 0; i < variants.size(); i++) {
		Ref<VariantDef> variant;
		variant.instantiate();
		ERR_FAIL_COND_V(!_decode_variant_def(fh, variant), false);
		variants[i] = variant;
	}
	_mode->set_variants(variants);

	PackedArray<enums::RankDef::Rank> ranks;
	ERR_FAIL_COND_V(!_decode_sorted_id_list(fh, ranks), false);
	_mode->set_ranks(ranks);
	PackedArray<enums::TribeDef::Tribe> tribes;
	ERR_FAIL_COND_V(!_decode_sorted_id_list(fh, tribes), false);
	_mode->set_tribes(tribes);
	PackedArray<enums::StatDef::Stat> stats;
	ERR_FAIL_COND_V(!_decode_sorted_id_list(fh, stats), false);
	_mode->set_stats(stats);
	PackedArray<enums::ModifierDef::Modifier> modifiers;
	ERR_FAIL_COND_V(!_decode_sorted_id_list(fh, modifiers), false);
	_mode->set_modifiers(modifiers);
	PackedArray<enums::EffectDef::Effect> effects;
	ERR_FAIL_COND_V(!_decode_sorted_id_list(fh, effects), false);
	_mode->set_effects(effects);
	PackedArray<enums::NPCDef::NPC> npcs;
	ERR_FAIL_COND_V(!_decode_sorted_id_list(fh, npcs), false);
	_mode->set_npcs(npcs);

	TypedArray<RankDef> custom_ranks;
	custom_ranks.resize(fh->read_uvarint());
	for (int64_t i = 0; i < custom_ranks.size(); i++) {
		Ref<RankDef> custom_rank;
		custom_rank.instantiate();
		ERR_FAIL_COND_V(!_decode_rank_def(fh, custom_rank), false);
		custom_ranks[i] = custom_rank;
	}
	_mode->set_custom_ranks(custom_ranks);
	TypedArray<TribeDef> custom_tribes;
	custom_tribes.resize(fh->read_uvarint());
	for (int64_t i = 0; i < custom_tribes.size(); i++) {
		Ref<TribeDef> custom_tribe;
		custom_tribe.instantiate();
		ERR_FAIL_COND_V(!_decode_tribe_def(fh, custom_tribe), false);
		custom_tribes[i] = custom_tribe;
	}
	_mode->set_custom_tribes(custom_tribes);
	TypedArray<StatDef> custom_stats;
	custom_stats.resize(fh->read_uvarint());
	for (int64_t i = 0; i < custom_stats.size(); i++) {
		Ref<StatDef> custom_stat;
		custom_stat.instantiate();
		ERR_FAIL_COND_V(!_decode_stat_def(fh, custom_stat), false);
		custom_stats[i] = custom_stat;
	}
	_mode->set_custom_stats(custom_stats);
	TypedArray<ModifierDef> custom_modifiers;
	custom_modifiers.resize(fh->read_uvarint());
	for (int64_t i = 0; i < custom_modifiers.size(); i++) {
		Ref<ModifierDef> custom_modifier;
		custom_modifier.instantiate();
		ERR_FAIL_COND_V(!_decode_modifier_def(fh, custom_modifier), false);
		custom_modifiers[i] = custom_modifier;
	}
	_mode->set_custom_modifiers(custom_modifiers);
	TypedArray<EffectDef> custom_effects;
	custom_effects.resize(fh->read_uvarint());
	for (int64_t i = 0; i < custom_effects.size(); i++) {
		Ref<EffectDef> custom_effect;
		custom_effect.instantiate();
		ERR_FAIL_COND_V(!_decode_effect_def(fh, custom_effect), false);
		custom_effects[i] = custom_effect;
	}
	_mode->set_custom_effects(custom_effects);
	TypedArray<NPCDef> custom_npcs;
	custom_npcs.resize(fh->read_uvarint());
	for (int64_t i = 0; i < custom_npcs.size(); i++) {
		Ref<NPCDef> custom_npc;
		custom_npc.instantiate();
		ERR_FAIL_COND_V(!_decode_npc_def(fh, custom_npc), false);
		custom_npcs[i] = custom_npc;
	}
	_mode->set_custom_npcs(custom_npcs);

	TypedArray<LocationDef> custom_locations;
	custom_locations.resize(fh->read_uvarint());
	for (int64_t i = 0; i < custom_locations.size(); i++) {
		Ref<LocationDef> custom_location;
		custom_location.instantiate();
		ERR_FAIL_COND_V(!_decode_location_def(fh, custom_location), false);
		custom_locations[i] = custom_location;
	}
	_mode->set_custom_locations(custom_locations);
	TypedArray<VariableDef> custom_variables;
	custom_variables.resize(fh->read_uvarint());
	for (int64_t i = 0; i < custom_variables.size(); i++) {
		Ref<VariableDef> custom_variable;
		custom_variable.instantiate();
		ERR_FAIL_COND_V(!_decode_variable_def(fh, custom_variable), false);
		custom_variables[i] = custom_variable;
	}
	_mode->set_custom_variables(custom_variables);
	TypedArray<ChoicesDef> custom_choices;
	custom_choices.resize(fh->read_uvarint());
	for (int64_t i = 0; i < custom_choices.size(); i++) {
		Ref<ChoicesDef> choices;
		choices.instantiate();
		ERR_FAIL_COND_V(!_decode_choices_def(fh, choices), false);
		custom_choices[i] = choices;
	}
	_mode->set_custom_choices(custom_choices);
	TypedArray<JigsawFunction> custom_functions;
	custom_functions.resize(fh->read_uvarint());
	for (int64_t i = 0; i < custom_functions.size(); i++) {
		Ref<JigsawFunction> custom_function;
		custom_function.instantiate();
		ERR_FAIL_COND_V(!_decode_jigsaw_function(fh, custom_function), false);
		custom_functions[i] = custom_function;
	}
	_mode->set_custom_functions(custom_functions);

	TypedArray<CardDef> card_defs;
	card_defs.resize(fh->read_uvarint());
	for (int64_t i = 0; i < card_defs.size(); i++) {
		Ref<CardDef> card_def;
		card_def.instantiate();
		ERR_FAIL_COND_V(!_decode_card_def(fh, card_def), false);
		card_defs[i] = card_def;
	}
	_mode->set_card_defs(card_defs);

	return fh->is_valid();
}
template<typename E>
static bool _encode_sorted_id_list(const Ref<FormatHelper> &fh, const PackedArray<E> &list) {
	// re-sort the list just in case
	PackedArray<E> list_sorted = list;
	list_sorted.sort();

	fh->write_uvarint(list_sorted.size());

	E prev = E::NONE;
	for (int64_t i = 0; i < list_sorted.size(); i++) {
		fh->write_uvarint(list_sorted[i] - prev - 1);
		prev = list_sorted[i];
	}

	return fh->is_valid();
}
bool DataContainer::_encode_game_mode(const Ref<FormatHelper> &fh) const {
	ERR_FAIL_COND_V(!_encode_game_mode_summary(fh, _mode), false);

	TypedArray<AudienceDef> audience = _mode->get_audience();
	fh->write_uvarint(audience.size());
	for (int64_t i = 0; i < audience.size(); i++) {
		ERR_FAIL_COND_V(!_encode_audience_def(fh, audience[i]), false);
	}
	TypedArray<CharacterDef> characters = _mode->get_characters();
	fh->write_uvarint(characters.size());
	for (int64_t i = 0; i < characters.size(); i++) {
		ERR_FAIL_COND_V(!_encode_character_def(fh, characters[i]), false);
	}

	Ref<CardDesign> design = _mode->get_default_card_design();
	if (unlikely(design.is_null())) {
		design.instantiate();
	}
	ERR_FAIL_COND_V(!_encode_card_design(fh, design), false);

	TypedArray<JigsawTriggerVariant> triggers = _mode->get_base_triggers();
	fh->write_uvarint(triggers.size());
	for (int64_t i = 0; i < triggers.size(); i++) {
		ERR_FAIL_COND_V(!_encode_jigsaw_trigger_variant(fh, triggers[i]), false);
	}

	TypedArray<VariantDef> variants = _mode->get_variants();
	fh->write_uvarint(variants.size());
	for (int64_t i = 0; i < variants.size(); i++) {
		ERR_FAIL_COND_V(!_encode_variant_def(fh, variants[i]), false);
	}

	ERR_FAIL_COND_V(!_encode_sorted_id_list(fh, _mode->get_ranks()), false);
	ERR_FAIL_COND_V(!_encode_sorted_id_list(fh, _mode->get_tribes()), false);
	ERR_FAIL_COND_V(!_encode_sorted_id_list(fh, _mode->get_stats()), false);
	ERR_FAIL_COND_V(!_encode_sorted_id_list(fh, _mode->get_modifiers()), false);
	ERR_FAIL_COND_V(!_encode_sorted_id_list(fh, _mode->get_effects()), false);
	ERR_FAIL_COND_V(!_encode_sorted_id_list(fh, _mode->get_npcs()), false);

	TypedArray<RankDef> custom_ranks = _mode->get_custom_ranks();
	fh->write_uvarint(custom_ranks.size());
	for (int64_t i = 0; i < custom_ranks.size(); i++) {
		ERR_FAIL_COND_V(!_encode_rank_def(fh, custom_ranks[i]), false);
	}
	TypedArray<TribeDef> custom_tribes = _mode->get_custom_tribes();
	fh->write_uvarint(custom_tribes.size());
	for (int64_t i = 0; i < custom_tribes.size(); i++) {
		ERR_FAIL_COND_V(!_encode_tribe_def(fh, custom_tribes[i]), false);
	}
	TypedArray<StatDef> custom_stats = _mode->get_custom_stats();
	fh->write_uvarint(custom_stats.size());
	for (int64_t i = 0; i < custom_stats.size(); i++) {
		ERR_FAIL_COND_V(!_encode_stat_def(fh, custom_stats[i]), false);
	}
	TypedArray<ModifierDef> custom_modifiers = _mode->get_custom_modifiers();
	fh->write_uvarint(custom_modifiers.size());
	for (int64_t i = 0; i < custom_modifiers.size(); i++) {
		ERR_FAIL_COND_V(!_encode_modifier_def(fh, custom_modifiers[i]), false);
	}
	TypedArray<EffectDef> custom_effects = _mode->get_custom_effects();
	fh->write_uvarint(custom_effects.size());
	for (int64_t i = 0; i < custom_effects.size(); i++) {
		ERR_FAIL_COND_V(!_encode_effect_def(fh, custom_effects[i]), false);
	}
	TypedArray<NPCDef> custom_npcs = _mode->get_custom_npcs();
	fh->write_uvarint(custom_npcs.size());
	for (int64_t i = 0; i < custom_npcs.size(); i++) {
		ERR_FAIL_COND_V(!_encode_npc_def(fh, custom_npcs[i]), false);
	}

	TypedArray<LocationDef> custom_locations = _mode->get_custom_locations();
	fh->write_uvarint(custom_locations.size());
	for (int64_t i = 0; i < custom_locations.size(); i++) {
		ERR_FAIL_COND_V(!_encode_location_def(fh, custom_locations[i]), false);
	}
	TypedArray<VariableDef> custom_variables = _mode->get_custom_variables();
	fh->write_uvarint(custom_variables.size());
	for (int64_t i = 0; i < custom_variables.size(); i++) {
		ERR_FAIL_COND_V(!_encode_variable_def(fh, custom_variables[i]), false);
	}
	TypedArray<ChoicesDef> custom_choices = _mode->get_custom_choices();
	fh->write_uvarint(custom_choices.size());
	for (int64_t i = 0; i < custom_choices.size(); i++) {
		ERR_FAIL_COND_V(!_encode_choices_def(fh, custom_choices[i]), false);
	}
	TypedArray<JigsawFunction> custom_functions = _mode->get_custom_functions();
	fh->write_uvarint(custom_functions.size());
	for (int64_t i = 0; i < custom_functions.size(); i++) {
		ERR_FAIL_COND_V(!_encode_jigsaw_function(fh, custom_functions[i]), false);
	}

	TypedArray<CardDef> card_defs = _mode->get_card_defs();
	fh->write_uvarint(card_defs.size());
	for (int64_t i = 0; i < card_defs.size(); i++) {
		ERR_FAIL_COND_V(!_encode_card_def(fh, card_defs[i]), false);
	}

	return fh->is_valid();
}
bool DataContainer::_decode_recording(const Ref<FormatHelper> &fh) {
	_mode_public_name = fh->read_stringvar();
	_mode_public_revision = fh->read_uvarint();

	if (_mode_public_name.is_empty() || _mode_public_revision <= 0) {
		ERR_FAIL_COND_V(!_decode_game_mode(fh), false);
	}

	_selected_variant = fh->read_uvarint();
	_rematches = fh->read_uvarint();
	_previous_wins.resize(fh->read_uvarint());
	for (int64_t i = 0; i < _previous_wins.size(); i++) {
		_previous_wins[i] = fh->read_uvarint();
	}
	_resumed_from_recording = fh->read_bytesvar();
	_resumed_from_round = int64_t(fh->read_uvarint()) - 1;

	_player_npc.resize(fh->read_uvarint());
	for (int64_t i = 0; i < _player_npc.size(); i++) {
		_player_npc[i] = static_cast<enums::NPCDef::NPC>(fh->read_uvarint());
	}
	_player_data.resize(fh->read_uvarint());
	for (int64_t i = 0; i < _player_data.size(); i++) {
		Ref<RecordingPlayerData> player;
		player.instantiate();
		player->set_display_name(fh->read_stringvar());
		player->set_character(fh->read_id<enums::CharacterDef::Character>());

		PackedByteArray packed_deck = fh->read_bytesvar();
		PackedArray<enums::CardDef::Card> initial_deck = Deck::decode(packed_deck);
		player->set_initial_deck(initial_deck);
		_player_data[i] = player;
	}

	_shared_seed = fh->read_bytesvar();

	_rounds.resize(fh->read_uvarint());
	for (int64_t i = 0; i < _rounds.size(); i++) {
		Ref<RecordingRoundData> round;
		round.instantiate();
		round->set_type(static_cast<RecordingRoundData::RoundType>(fh->read_uvarint()));
		ERR_FAIL_COND_V(round->get_type() != RecordingRoundData::CHOICE && round->get_type() != RecordingRoundData::REALTIME, false);

		round->set_initial_checksum(fh->read_bytesvar());

		TypedArray<RecordingRoundPlayerData> player_data;
		player_data.resize(_player_data.size());
		for (int64_t j = 0; j < round->get_player_data().size(); j++) {
			Ref<RecordingRoundPlayerData> player;
			player.instantiate();

			player->set_personal_seed(fh->read_bytesvar());
			switch (round->get_type()) {
			case RecordingRoundData::CHOICE:
			{
				PackedInt64Array chosen_cards;
				chosen_cards.resize(fh->read_uvarint());
				for (int64_t k = 0; k < chosen_cards.size(); k++) {
					chosen_cards[k] = fh->read_uvarint();
				}
				player->set_chosen_cards(chosen_cards);
				break;
			}
			case RecordingRoundData::REALTIME:
			{
				PackedByteArray packed_inputs = fh->read_bytesvar();
				PackedInt32Array unpacked_inputs = ButtonInputHistory::unpack_inputs(packed_inputs);
				ERR_FAIL_COND_V(unpacked_inputs.is_empty() && !packed_inputs.is_empty(), false);
				player->set_realtime_inputs(unpacked_inputs);
				player->set_personal_seed_after(fh->read_bytesvar());
				break;
			}
			}

			player_data[j] = player;
		}
		round->set_player_data(player_data);

		switch (round->get_type()) {
		case RecordingRoundData::CHOICE:
			break;
		case RecordingRoundData::REALTIME:
			round->set_final_checksum(fh->read_bytesvar());
			break;
		}

		_rounds[i] = round;
	}

	_final_checksum = fh->read_bytesvar();

	return true;
}
bool DataContainer::_encode_recording(const Ref<FormatHelper> &fh) const {
	fh->write_stringvar(_mode_public_name);
	fh->write_uvarint(_mode_public_revision);

	if (_mode_public_name.is_empty() || _mode_public_revision <= 0) {
		ERR_FAIL_COND_V(!_encode_game_mode(fh), false);
	}

	fh->write_uvarint(_selected_variant);
	fh->write_uvarint(_rematches);
	fh->write_uvarint(_previous_wins.size());
	for (int64_t i = 0; i < _previous_wins.size(); i++) {
		fh->write_uvarint(_previous_wins[i]);
	}
	fh->write_bytesvar(_resumed_from_recording);
	fh->write_uvarint(_resumed_from_round + 1);

	fh->write_uvarint(_player_npc.size());
	for (int64_t i = 0; i < _player_npc.size(); i++) {
		fh->write_uvarint(_player_npc[i]);
	}
	fh->write_uvarint(_player_data.size());
	for (int64_t i = 0; i < _player_data.size(); i++) {
		Ref<RecordingPlayerData> player = _player_data[i];
		fh->write_stringvar(player->get_display_name());
		fh->write_id(player->get_character());
		fh->write_bytesvar(Deck::encode(player->get_initial_deck()));
	}

	fh->write_bytesvar(_shared_seed);

	fh->write_uvarint(_rounds.size());
	for (int64_t i = 0; i < _rounds.size(); i++) {
		Ref<RecordingRoundData> round = _rounds[i];
		fh->write_uvarint(round->get_type());
		fh->write_bytesvar(round->get_initial_checksum());

		ERR_FAIL_COND_V(_player_data.size() != round->get_player_data().size(), false);
		for (int64_t j = 0; j < round->get_player_data().size(); j++) {
			Ref<RecordingRoundPlayerData> player = round->get_player_data()[j];
			fh->write_bytesvar(player->get_personal_seed());
			switch (round->get_type()) {
			case RecordingRoundData::CHOICE:
			{
				fh->write_uvarint(player->get_chosen_cards().size());
				for (int64_t k = 0; k < player->get_chosen_cards().size(); k++) {
					fh->write_uvarint(player->get_chosen_cards()[k]);
				}
				break;
			}
			case RecordingRoundData::REALTIME:
			{
				PackedInt32Array unpacked_inputs = player->get_realtime_inputs();
				PackedByteArray packed_inputs = ButtonInputHistory::pack_inputs(unpacked_inputs);
				ERR_FAIL_COND_V(packed_inputs.is_empty() && !unpacked_inputs.is_empty(), false);
				fh->write_bytesvar(packed_inputs);
				fh->write_bytesvar(player->get_personal_seed_after());
				break;
			}
			}
		}

		switch (round->get_type()) {
		case RecordingRoundData::CHOICE:
			break;
		case RecordingRoundData::REALTIME:
			fh->write_bytesvar(round->get_final_checksum());
			break;
		}
	}

	fh->write_bytesvar(_final_checksum);

	return true;
}
bool DataContainer::_decode_error_report(const Ref<FormatHelper> &fh) {
	ERR_FAIL_V_MSG(false, "DataContainer cannot decode error report: not yet implemented");
}
bool DataContainer::_encode_error_report(const Ref<FormatHelper> &fh) const {
	ERR_FAIL_V_MSG(false, "DataContainer cannot encode error report: not yet implemented");
}

Ref<DataContainer> DataContainer::from_byte_array(const PackedByteArray &buf, const String &debug_name) {
	Ref<FormatHelper> fh = FormatHelper::read(debug_name, buf);

	uint64_t magic = fh->read_uint64();
	ERR_FAIL_COND_V(magic != MAGIC, Ref<DataContainer>());

	Ref<DataContainer> data;
	data.instantiate();

	data->set_format_revision(static_cast<DataContainer::FormatRevision>(fh->read_uvarint()));
	ERR_FAIL_COND_V(data->get_format_revision() > FORMAT_0, Ref<DataContainer>());

	data->set_container_type(static_cast<DataContainer::ContainerType>(fh->read_uvarint()));
	Vector3i game_version;
	game_version.x = fh->read_uvarint();
	game_version.y = fh->read_uvarint();
	game_version.z = fh->read_uvarint();
	data->set_game_version(game_version);

	data->set_timestamp(fh->read_uvarint());

	int64_t decompressed_size = fh->read_uvarint();
	PackedByteArray compressed_data = fh->read_bytesvar();
	ERR_FAIL_COND_V(!fh->is_valid_eof(), Ref<DataContainer>());
	PackedByteArray decompressed_data = compressed_data.decompress(decompressed_size, FileAccess::COMPRESSION_ZSTD);

	fh = FormatHelper::read(debug_name, decompressed_data);

	int64_t data_file_count = fh->read_uvarint();
	TypedArray<EmbeddedDataFile> data_files;
	data_files.resize(data_file_count);
	LocalVector<PackedByteArray> seen_file_ids;
	for (int64_t i = 0; i < data_file_count; i++) {
		Ref<EmbeddedDataFile> data_file;
		data_file.instantiate();
		data_file->set_file_id(fh->read_bytesvar());
		data_file->set_data(fh->read_bytesvar());
		data_files[i] = data_file;

		ERR_FAIL_COND_V_MSG(seen_file_ids.has(data_file->get_file_id()), Ref<DataContainer>(), "duplicate embedded file ID");
		seen_file_ids.push_back(data_file->get_file_id());

		if (unlikely(!data_file->validate_id())) {
			return Ref<DataContainer>();
		}
	}
	data->set_data_files(data_files);

	switch (data->get_container_type()) {
	case GAME_MODE_SUMMARY:
		data->_mode_summary.instantiate();
		ERR_FAIL_COND_V(!data->_decode_game_mode_summary(fh, data->_mode_summary), Ref<DataContainer>());
		break;
	case GAME_MODE:
		ERR_FAIL_COND_V(!data->_decode_game_mode(fh), Ref<DataContainer>());
		break;
	case RECORDING:
		ERR_FAIL_COND_V(!data->_decode_recording(fh), Ref<DataContainer>());
		break;
	case ERROR_REPORT:
		ERR_FAIL_COND_V(!data->_decode_error_report(fh), Ref<DataContainer>());
		break;
	default:
		ERR_FAIL_V_MSG(Ref<DataContainer>(), vformat("DataContainer cannot decode data for unhandled container type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("DataContainer", "ContainerType", data->get_container_type())));
		break;
	}

	ERR_FAIL_COND_V(!fh->is_valid_eof(), Ref<DataContainer>());
	return data;
}

PackedByteArray DataContainer::to_byte_array(const String &debug_name) const {
	Ref<FormatHelper> fh = FormatHelper::write(debug_name);

	fh->write_uint64(MAGIC);
	fh->write_uvarint(_format_revision);
	fh->write_uvarint(_container_type);
	fh->write_uvarint(_game_version.x);
	fh->write_uvarint(_game_version.y);
	fh->write_uvarint(_game_version.z);

	fh->write_uvarint(_timestamp);

	Ref<FormatHelper> sub_fh = FormatHelper::write(debug_name);

	sub_fh->write_uvarint(_data_files.size());
	LocalVector<PackedByteArray> seen_file_ids;
	for (int64_t i = 0; i < _data_files.size(); i++) {
		Ref<EmbeddedDataFile> data_file = _data_files[i];
		sub_fh->write_bytesvar(data_file->get_file_id());
		sub_fh->write_bytesvar(data_file->get_data());

		ERR_FAIL_COND_V_MSG(seen_file_ids.has(data_file->get_file_id()), PackedByteArray(), "duplicate embedded file ID");
		seen_file_ids.push_back(data_file->get_file_id());

		if (unlikely(!data_file->validate_id())) {
			return PackedByteArray();
		}
	}

	switch (_container_type) {
	case GAME_MODE_SUMMARY:
		ERR_FAIL_COND_V(!_encode_game_mode_summary(sub_fh, _mode_summary), PackedByteArray());
		break;
	case GAME_MODE:
		ERR_FAIL_COND_V(!_encode_game_mode(sub_fh), PackedByteArray());
		break;
	case RECORDING:
		ERR_FAIL_COND_V(!_encode_recording(sub_fh), PackedByteArray());
		break;
	case ERROR_REPORT:
		ERR_FAIL_COND_V(!_encode_error_report(sub_fh), PackedByteArray());
		break;
	default:
		ERR_FAIL_V_MSG(PackedByteArray(), vformat("DataContainer cannot encode data for unhandled container type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("DataContainer", "ContainerType", _container_type)));
		break;
	}

	ERR_FAIL_COND_V(!sub_fh->is_valid(), PackedByteArray());
	PackedByteArray uncompressed_data = sub_fh->get_buffer();
	fh->write_uvarint(uncompressed_data.size());
	PackedByteArray compressed_data = uncompressed_data.compress(FileAccess::COMPRESSION_ZSTD);
	fh->write_bytesvar(compressed_data);

	ERR_FAIL_COND_V(!fh->is_valid(), PackedByteArray());
	return fh->get_buffer();
}

TypedArray<JigsawError> DataContainer::validate() const {
	WARN_PRINT_ONCE("TODO: DataContainer::validate"); // TODO: DataContainer::validate
	return TypedArray<JigsawError>();
}
