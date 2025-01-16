#include "legacy/legacy_parse.h"

#include "defs/card_filter_and.h"
#include "defs/card_filter_or.h"
#include "defs/card_filter_tribe.h"
#include "protocol/deck.h"
#include "util/base32.h"

#include "jigsaw/command/jigsaw_command_set_variable.h"
#include "jigsaw/parameter/jigsaw_parameter_card.h"
#include "jigsaw/parameter/jigsaw_parameter_card_filter.h"
#include "jigsaw/parameter/jigsaw_parameter_effect_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_location.h"
#include "jigsaw/parameter/jigsaw_parameter_ordered_list.h"

#include <godot_cpp/classes/marshalls.hpp>

LazyPredefined<GameMode> LegacyParse::VANILLA_1_0_5{ "VANILLA_1_0_5" };
LazyPredefined<GameMode> LegacyParse::VANILLA_1_1{ "VANILLA_1_1" };
LazyPredefined<GameMode> LegacyParse::VANILLA_1_1_1{ "VANILLA_1_1_1" };
LazyPredefined<GameMode> LegacyParse::VANILLA_1_2_1{ "VANILLA_1_2_1" };

void LegacyParse::_bind_methods() {
	ClassDB::bind_static_method("LegacyParse", D_METHOD("card_set", "buf", "vanilla_default", "p1_spoiler_guard", "p2_spoiler_guard"), &LegacyParse::card_set, DEFVAL(PackedByteArray()), DEFVAL(PackedByteArray()));
	ClassDB::bind_static_method("LegacyParse", D_METHOD("card_recording", "buf"), &LegacyParse::card_recording);
	ClassDB::bind_static_method("LegacyParse", D_METHOD("arcade_recording", "buf"), &LegacyParse::arcade_recording);
}

Ref<DataContainer> LegacyParse::card_set(const PackedStringArray &buf, const Ref<GameMode> &vanilla_default, const PackedByteArray &p1_spoiler_guard, const PackedByteArray &p2_spoiler_guard) {
	Ref<DataContainer> container;
	container.instantiate();

	Ref<GameMode> mode;
	mode.instantiate();

	Ref<GameMode> vanilla = vanilla_default;

	container->set_format_revision(DataContainer::FORMAT_0);
	container->set_container_type(DataContainer::GAME_MODE);
	container->set_game_version(Vector3i(0, 4, 0));

	container->set_timestamp(0);

	container->set_mode_summary(mode);
	container->set_mode(mode);

	if (likely(buf.size() > 0)) {
		PackedByteArray first_data = Marshalls::get_singleton()->base64_to_raw(buf[0]);
		int64_t first_card_index = 0;
		if (likely(first_data.size() > 0 && first_data[0] == 3)) {
			first_card_index = 1;

			Ref<FormatHelper> fh = FormatHelper::read("legacy game mode", first_data);
			ERR_FAIL_COND_V(!game_mode(container, fh, vanilla), Ref<DataContainer>());
			ERR_FAIL_COND_V(!fh->is_valid_eof(), Ref<DataContainer>());
		}

		for (int64_t i = first_card_index; i < buf.size(); i++) {
			PackedByteArray card_data = Marshalls::get_singleton()->base64_to_raw(buf[i]);
			Ref<FormatHelper> fh = FormatHelper::read("legacy card", card_data);
			ERR_FAIL_COND_V(!card(container, fh), Ref<DataContainer>());
			ERR_FAIL_COND_V(!fh->is_valid_eof(), Ref<DataContainer>());
		}
	}

	mode->set_base_triggers(vanilla->get_base_triggers().duplicate(true));
	if (mode->get_variants().is_empty()) {
		mode->set_variants(vanilla->get_variants().duplicate(true));
	}

	mode->set_default_card_design(vanilla->get_default_card_design()->duplicate(true));
	mode->set_audience(vanilla->get_audience().duplicate(true));
	mode->set_characters(vanilla->get_characters().duplicate(true));

	mode->set_ranks(vanilla->get_ranks() + mode->get_ranks());
	mode->set_tribes(vanilla->get_tribes() + mode->get_tribes());
	mode->set_stats(vanilla->get_stats() + mode->get_stats());
	mode->set_modifiers(vanilla->get_modifiers() + mode->get_modifiers());
	mode->set_effects(vanilla->get_effects() + mode->get_effects());
	mode->set_npcs(vanilla->get_npcs() + mode->get_npcs());

	mode->set_custom_npcs(vanilla->get_custom_npcs().duplicate(true));
	mode->set_custom_variables(vanilla->get_custom_variables().duplicate(true));

	TypedArray<CardDef> cards = mode->get_card_defs();
	for (int64_t i = 0; i < cards.size(); i++) {
		Ref<CardDef> card = cards[i];
		if (card->get_meta("legacy_unpickable", false)) {
			card->remove_meta("legacy_unpickable");

			Ref<FormatHelper> legacy_unpickable_enc = FormatHelper::write("legacy unpickable");
			legacy_unpickable_enc->write_uint8(0);
			legacy_unpickable_enc->write_uvarint(0x01);
			legacy_unpickable_enc->write_uvarint(1);
			legacy_unpickable_enc->write_uvarint(card->get_id());

			Ref<FormatHelper> legacy_unpickable_dec = FormatHelper::read("legacy unpickable", legacy_unpickable_enc->get_buffer());
			field_banned_cards(container, legacy_unpickable_dec, Ref<VariantDef>());
		}

		if (i == 0 && mode->get_mode_thumbnail() == enums::IconDef::NONE) {
			mode->set_mode_thumbnail(card->get_portrait());
		}

		TypedArray<EffectInstance> effects = card->get_effects();
		for (int64_t j = 0; j < effects.size(); j++) {
			Ref<EffectInstance> effect = effects[j];
			TypedArray<JigsawParameter> params = effect->get_params();
			for (int64_t k = 0; k < params.size(); k++) {
				Ref<JigsawParameter> param = params[k];
				finalize_effect_param(container, param);
			}
		}
	}

	TypedArray<CardDef> vanilla_cards = vanilla->get_card_defs();
	for (int64_t i = vanilla_cards.size() - 1; i >= 0; i--) {
		Ref<CardDef> vanilla_card = vanilla_cards[i];
		bool found = false;
		for (int64_t j = 0; j < cards.size(); j++) {
			Ref<CardDef> card = cards[j];
			if (card->get_id() == vanilla_card->get_id()) {
				found = true;
				break;
			}
		}

		if (!found) {
			cards.insert(0, vanilla_card->duplicate(true));
		}
	}
	mode->set_card_defs(cards);

	TypedArray<VariantDef> variants = mode->get_variants();
	for (int64_t i = 0; i < variants.size(); i++) {
		Ref<VariantDef> variant = variants[i];
		String legacy_npc = variant->get_meta("legacy_npc", "");
		if (!legacy_npc.is_empty()) {
			variant->set_npcs(Array::make(custom_npc(container, legacy_npc)));
			variant->remove_meta("legacy_npc");
		}
	}

	ERR_FAIL_COND_V_MSG(!p1_spoiler_guard.is_empty() || !p2_spoiler_guard.is_empty(), Ref<DataContainer>(), "TODO: spoiler guard"); // TODO: spoiler guard

	TypedArray<NPCDef> npcs = mode->get_custom_npcs();
	for (int64_t i = 0; i < npcs.size(); i++) {
		Ref<NPCDef> npc = npcs[i];
		npc->remove_meta("legacy_npc");
	}

	return container;
}
enums::IconDef::Icon LegacyParse::custom_portrait(const Ref<DataContainer> &container, const PackedByteArray &data, bool external) {
	TypedArray<IconDef> icons = container->get_mode()->get_custom_icons();
	PackedByteArray portrait_id = data;

	if (!external) {
		TypedArray<EmbeddedDataFile> files = container->get_data_files();

		bool found = false;
		for (int64_t i = 0; i < files.size(); i++) {
			Ref<EmbeddedDataFile> file = files[i];
			if (file->get_data() == data) {
				portrait_id = file->get_file_id();
				found = true;
				break;
			}
		}

		if (!found) {
			Ref<HashingContext> hash;
			hash.instantiate();
			hash->start(HashingContext::HASH_SHA256);
			hash->update(data);

			Ref<FormatHelper> portrait_id_fh = FormatHelper::write("legacy embedded portrait file ID");
			portrait_id_fh->write_bytes(hash->finish().slice(0, 8));
			portrait_id_fh->write_uvarint(files.size() + 1); // start opaque IDs at 1
			portrait_id_fh->write_uvarint(4); // embedded icon

			portrait_id = portrait_id_fh->get_buffer();

			Ref<EmbeddedDataFile> file;
			file.instantiate();
			file->set_file_id(portrait_id);
			file->set_data(data);
			files.append(file);
			container->set_data_files(files);
		}
	}

	for (int64_t i = 0; i < icons.size(); i++) {
		Ref<IconDef> icon = icons[i];
		if (icon->get_file_id() == portrait_id) {
			return static_cast<enums::IconDef::Icon>(enums::IconDef::FIRST_CUSTOM + i);
		}
	}

	Ref<IconDef> icon;
	icon.instantiate();
	icon->set_file_id(portrait_id);
	icons.append(icon);
	container->get_mode()->set_custom_icons(icons);

	return static_cast<enums::IconDef::Icon>(enums::IconDef::FIRST_CUSTOM + icons.size() - 1);
}

enums::TribeDef::Tribe LegacyParse::custom_tribe(const Ref<DataContainer> &container, const Color &color, const String &name) {
	TypedArray<TribeDef> custom_tribes = container->get_mode()->get_custom_tribes();
	for (int64_t i = 0; i < custom_tribes.size(); i++) {
		Ref<TribeDef> custom_tribe = custom_tribes[i];
		if (custom_tribe->get_name() == name) {
			return static_cast<enums::TribeDef::Tribe>(enums::TribeDef::FIRST_CUSTOM + i);
		}
	}

	Ref<TribeDef> custom_tribe;
	custom_tribe.instantiate();

	custom_tribe->set_name(name);
	custom_tribe->set_color(color);
	custom_tribe->set_display(name.begins_with("_") ? TribeDef::HIDDEN : TribeDef::NORMAL);

	enums::TribeDef::Tribe tribe = static_cast<enums::TribeDef::Tribe>(enums::TribeDef::FIRST_CUSTOM + custom_tribes.size());
	custom_tribes.append(custom_tribe);

	container->get_mode()->set_custom_tribes(custom_tribes);

	PackedArray<enums::TribeDef::Tribe> mode_tribes = container->get_mode()->get_tribes();
	mode_tribes.append(tribe);
	container->get_mode()->set_tribes(mode_tribes);

	return tribe;
}

enums::NPCDef::NPC LegacyParse::custom_npc(const Ref<DataContainer> &container, const String &code) {
	TypedArray<NPCDef> npcs = container->get_mode()->get_custom_npcs();
	for (int64_t i = 0; i < npcs.size(); i++) {
		Ref<NPCDef> npc = npcs[i];
		if (npc->get_meta("legacy_npc", "") == code) {
			return static_cast<enums::NPCDef::NPC>(i + enums::NPCDef::FIRST_CUSTOM);
		}
	}

	if (code == "janet") {
		return legacy_npc_janet(container);
	} else if (code == "bu-gi" || code == "johnny" || code == "kage" || code == "ritchee" || code == "serene" || code == "carmina") {
		return legacy_npc_tourney_player(container, code);
	} else if (code == "saved-decks") {
		// no longer supported
		return legacy_npc_generic(container, code);
	} else if (code == "tutorial") {
		return legacy_npc_card_master(container, code, "carmina", "01H00000000013HSMR");
	} else if (code == "carmina2") {
		return legacy_npc_card_master(container, code, "carmina", "3P7T52H8MA5273HG842YF7KR");
	} else if (code == "chuck") {
		return legacy_npc_card_master(container, code, "chuck", "4HH0000007VXYZFG84210GG8");
	} else if (code == "arie") {
		return legacy_npc_card_master(container, code, "arie", "310J10G84212NANCPAD6K7VW");
	} else if (code == "shay") {
		return legacy_npc_card_master(container, code, "shay", "511KHRWE631GRD6K9MMA5840");
	} else if (code == "crow") {
		return legacy_npc_card_master(container, code, "crow", "101AXEPH8MCJ8G845G");
	} else if (code == "genow") {
		return legacy_npc_card_master(container, code, "genow", "6N5Q3HRWE6VDQ6K9G8");
	} else if (code == "mender-spam" || code == "mender-spam-mothiva" || code == "mender-spam-kali") {
		return legacy_npc_mender_spam(container, code);
	} else if (code == "tp2-generic") {
		return legacy_npc_tp2_generic(container);
	} else if (code == "tp2-janet") {
		return legacy_npc_tp2_janet(container);
	} else if (code == "tp2-bu-gi") {
		return legacy_npc_tp2_bu_gi(container);
	} else if (code == "tp2-johnny") {
		return legacy_npc_tp2_johnny(container);
	} else if (code == "tp2-kage") {
		return legacy_npc_tp2_kage(container);
	} else if (code == "tp2-ritchee") {
		return legacy_npc_tp2_ritchee(container);
	} else if (code == "tp2-serene") {
		return legacy_npc_tp2_serene(container);
	} else if (code == "tp2-carmina") {
		return legacy_npc_tp2_carmina(container);
	} else if (code == "tp2-chuck") {
		return legacy_npc_tp2_chuck(container);
	} else if (code == "tp2-arie") {
		return legacy_npc_tp2_arie(container);
	} else if (code == "tp2-shay") {
		return legacy_npc_tp2_shay(container);
	} else if (code == "tp2-crow") {
		return legacy_npc_tp2_crow(container);
	} else if (code == "generic") {
		return legacy_npc_generic(container, code);
	} else if (code.begins_with("cm-")) {
		PackedStringArray params = code.split("-", true, 3);
		ERR_FAIL_COND_V_MSG(params.size() == 3 && params[2].is_empty(), legacy_npc_generic(container, code), vformat("unhandled custom NPC code: %s", code));
		return legacy_npc_card_master(container, code, params.size() == 3 ? params[2] : String(), params[1]);
	} else {
		ERR_FAIL_V_MSG(legacy_npc_generic(container, code), vformat("unhandled custom NPC code: %s", code));
	}
}

enums::NPCDef::NPC LegacyParse::add_legacy_npc(const Ref<DataContainer> &container, const String &code, const String &display_name, const String &character, const Ref<JigsawProcedureNPCBuildDeck> &build_deck, const Ref<JigsawProcedureNPCDecideChoice> &decide_choice) {
	Ref<NPCDef> npc;
	npc.instantiate();
	npc->set_meta("legacy_npc", code);
	npc->set_display_name(display_name);
	if (!character.is_empty()) {
		TypedArray<CharacterDef> characters = container->get_mode()->get_characters();
		for (int64_t i = 0; i < characters.size(); i++) {
			Ref<CharacterDef> character = characters[i];
			if (character->get_id() == character) {
				npc->set_character(static_cast<enums::CharacterDef::Character>(enums::CharacterDef::FIRST_CUSTOM + i));
				break;
			}
		}
	}
	npc->set_build_deck(build_deck);
	npc->set_decide_choice(decide_choice);

	TypedArray<NPCDef> npcs = container->get_mode()->get_custom_npcs();
	enums::NPCDef::NPC npc_id = static_cast<enums::NPCDef::NPC>(enums::NPCDef::FIRST_CUSTOM + npcs.size());
	npcs.append(npc);
	container->get_mode()->set_custom_npcs(npcs);

	return npc_id;
}

LazyPredefined<JigsawProcedureNPCBuildDeck> LegacyParse::LEGACY_NPC_BUILD_DECK_GENERIC{ "LEGACY_NPC_BUILD_DECK_GENERIC" };
LazyPredefined<JigsawProcedureNPCBuildDeck> LegacyParse::LEGACY_NPC_BUILD_DECK_TOURNEY_PLAYER{ "LEGACY_NPC_BUILD_DECK_TOURNEY_PLAYER" };
LazyPredefined<JigsawProcedureNPCBuildDeck> LegacyParse::LEGACY_NPC_BUILD_DECK_JANET{ "LEGACY_NPC_BUILD_DECK_JANET" };
LazyPredefined<JigsawProcedureNPCBuildDeck> LegacyParse::LEGACY_NPC_BUILD_DECK_MENDER_SPAM_MOTHIVA{ "LEGACY_NPC_BUILD_DECK_MENDER_SPAM_MOTHIVA" };
LazyPredefined<JigsawProcedureNPCBuildDeck> LegacyParse::LEGACY_NPC_BUILD_DECK_MENDER_SPAM_KALI{ "LEGACY_NPC_BUILD_DECK_MENDER_SPAM_KALI" };
LazyPredefined<JigsawProcedureNPCBuildDeck> LegacyParse::LEGACY_NPC_BUILD_DECK_MENDER_SPAM_RANDOM{ "LEGACY_NPC_BUILD_DECK_MENDER_SPAM_RANDOM" };
LazyPredefined<JigsawProcedureNPCBuildDeck> LegacyParse::LEGACY_NPC_BUILD_DECK_TP2_GENERIC{ "LEGACY_NPC_BUILD_DECK_TP2_GENERIC" };
LazyPredefined<JigsawProcedureNPCBuildDeck> LegacyParse::LEGACY_NPC_BUILD_DECK_TP2_JANET{ "LEGACY_NPC_BUILD_DECK_TP2_JANET" };
LazyPredefined<JigsawProcedureNPCBuildDeck> LegacyParse::LEGACY_NPC_BUILD_DECK_TP2_BU_GI{ "LEGACY_NPC_BUILD_DECK_TP2_BU_GI" };
LazyPredefined<JigsawProcedureNPCBuildDeck> LegacyParse::LEGACY_NPC_BUILD_DECK_TP2_JOHNNY{ "LEGACY_NPC_BUILD_DECK_TP2_JOHNNY" };
LazyPredefined<JigsawProcedureNPCBuildDeck> LegacyParse::LEGACY_NPC_BUILD_DECK_TP2_KAGE{ "LEGACY_NPC_BUILD_DECK_TP2_KAGE" };
LazyPredefined<JigsawProcedureNPCBuildDeck> LegacyParse::LEGACY_NPC_BUILD_DECK_TP2_RITCHEE{ "LEGACY_NPC_BUILD_DECK_TP2_RITCHEE" };
LazyPredefined<JigsawProcedureNPCBuildDeck> LegacyParse::LEGACY_NPC_BUILD_DECK_TP2_SERENE{ "LEGACY_NPC_BUILD_DECK_TP2_SERENE" };
LazyPredefined<JigsawProcedureNPCBuildDeck> LegacyParse::LEGACY_NPC_BUILD_DECK_TP2_CARMINA{ "LEGACY_NPC_BUILD_DECK_TP2_CARMINA" };
LazyPredefined<JigsawProcedureNPCBuildDeck> LegacyParse::LEGACY_NPC_BUILD_DECK_TP2_CHUCK{ "LEGACY_NPC_BUILD_DECK_TP2_CHUCK" };
LazyPredefined<JigsawProcedureNPCBuildDeck> LegacyParse::LEGACY_NPC_BUILD_DECK_TP2_ARIE{ "LEGACY_NPC_BUILD_DECK_TP2_ARIE" };
LazyPredefined<JigsawProcedureNPCBuildDeck> LegacyParse::LEGACY_NPC_BUILD_DECK_TP2_SHAY{ "LEGACY_NPC_BUILD_DECK_TP2_SHAY" };
LazyPredefined<JigsawProcedureNPCBuildDeck> LegacyParse::LEGACY_NPC_BUILD_DECK_TP2_CROW{ "LEGACY_NPC_BUILD_DECK_TP2_CROW" };
LazyPredefined<JigsawProcedureNPCDecideChoice> LegacyParse::LEGACY_NPC_DECIDE_CHOICE_GENERIC{ "LEGACY_NPC_DECIDE_CHOICE_GENERIC" };
LazyPredefined<JigsawProcedureNPCDecideChoice> LegacyParse::LEGACY_NPC_DECIDE_CHOICE_MENDER_SPAM{ "LEGACY_NPC_DECIDE_CHOICE_MENDER_SPAM" };
LazyPredefined<JigsawProcedureNPCDecideChoice> LegacyParse::LEGACY_NPC_DECIDE_CHOICE_TP2{ "LEGACY_NPC_DECIDE_CHOICE_TP2" };

enums::NPCDef::NPC LegacyParse::legacy_npc_generic(const Ref<DataContainer> &container, const String &code) {
	return add_legacy_npc(container, code, vformat("Legacy Generic (%s)", code), String(), LEGACY_NPC_BUILD_DECK_GENERIC->duplicate(true), LEGACY_NPC_DECIDE_CHOICE_GENERIC->duplicate(true));
}
enums::NPCDef::NPC LegacyParse::legacy_npc_tourney_player(const Ref<DataContainer> &container, const String &name) {
	return add_legacy_npc(container, name, vformat("Legacy Tourney Player (%s)", name), name, LEGACY_NPC_BUILD_DECK_TOURNEY_PLAYER->duplicate(true), LEGACY_NPC_DECIDE_CHOICE_GENERIC->duplicate(true));
}
enums::NPCDef::NPC LegacyParse::legacy_npc_janet(const Ref<DataContainer> &container) {
	return add_legacy_npc(container, "janet", "Legacy Tourney Player (janet)", "janet", LEGACY_NPC_BUILD_DECK_JANET->duplicate(true), LEGACY_NPC_DECIDE_CHOICE_GENERIC->duplicate(true));
}
enums::NPCDef::NPC LegacyParse::legacy_npc_card_master(const Ref<DataContainer> &container, const String &code, const String &name, const String &deck) {
	PackedArray<enums::CardDef::Card> card_ids = Deck::decode(Base32::decode_crockford(deck));
	TypedArray<JigsawParameter> card_params = Array(card_ids).map(callable_mp_static(&JigsawParameterCard::make));

	Ref<JigsawCommandSetVariable> build_deck_command;
	build_deck_command.instantiate();
	build_deck_command->set_persistent(false);
	build_deck_command->set_variable(JigsawParameterLocalVariable::make(JigsawParameterLocalVariable::FRAME_RESULTS, 0));
	build_deck_command->set_value(JigsawParameterOrderedList::make(card_params));

	Ref<JigsawCommandList> build_deck_list;
	build_deck_list.instantiate();
	build_deck_list->set_list(Array::make(build_deck_command));

	Ref<JigsawProcedureNPCBuildDeck> build_deck;
	build_deck.instantiate();
	build_deck->set_commands(build_deck_list);

	return add_legacy_npc(container, code, vformat("Legacy Card Master (%s)", code), name, build_deck, LEGACY_NPC_DECIDE_CHOICE_GENERIC->duplicate(true));
}
enums::NPCDef::NPC LegacyParse::legacy_npc_mender_spam(const Ref<DataContainer> &container, const String &code) {
	Ref<JigsawProcedureNPCBuildDeck> build_deck;
	if (code == "mender-spam-mothiva") {
		build_deck = LEGACY_NPC_BUILD_DECK_MENDER_SPAM_MOTHIVA->duplicate(true);
	} else if (code == "mender-spam-kali") {
		build_deck = LEGACY_NPC_BUILD_DECK_MENDER_SPAM_KALI->duplicate(true);
	} else {
		build_deck = LEGACY_NPC_BUILD_DECK_MENDER_SPAM_RANDOM->duplicate(true);
	}

	return add_legacy_npc(container, code, vformat("Legacy Mender Spam (%s)", code), code.get_slice("-", 2), build_deck, LEGACY_NPC_DECIDE_CHOICE_MENDER_SPAM->duplicate(true));
}
enums::NPCDef::NPC LegacyParse::legacy_npc_tp2_generic(const Ref<DataContainer> &container) {
	return add_legacy_npc(container, "tp2-generic", "Legacy TP2 Generic", String(), LEGACY_NPC_BUILD_DECK_TP2_GENERIC->duplicate(true), LEGACY_NPC_DECIDE_CHOICE_TP2->duplicate(true));
}
enums::NPCDef::NPC LegacyParse::legacy_npc_tp2_janet(const Ref<DataContainer> &container) {
	return add_legacy_npc(container, "tp2-janet", "Legacy TP2 Janet", "janet", LEGACY_NPC_BUILD_DECK_TP2_JANET->duplicate(true), LEGACY_NPC_DECIDE_CHOICE_TP2->duplicate(true));
}
enums::NPCDef::NPC LegacyParse::legacy_npc_tp2_bu_gi(const Ref<DataContainer> &container) {
	return add_legacy_npc(container, "tp2-bu-gi", "Legacy TP2 Bu-Gi", "bu-gi", LEGACY_NPC_BUILD_DECK_TP2_BU_GI->duplicate(true), LEGACY_NPC_DECIDE_CHOICE_TP2->duplicate(true));
}
enums::NPCDef::NPC LegacyParse::legacy_npc_tp2_johnny(const Ref<DataContainer> &container) {
	return add_legacy_npc(container, "tp2-johnny", "Legacy TP2 Johnny", "johnny", LEGACY_NPC_BUILD_DECK_TP2_JOHNNY->duplicate(true), LEGACY_NPC_DECIDE_CHOICE_TP2->duplicate(true));
}
enums::NPCDef::NPC LegacyParse::legacy_npc_tp2_kage(const Ref<DataContainer> &container) {
	return add_legacy_npc(container, "tp2-kage", "Legacy TP2 Kage", "kage", LEGACY_NPC_BUILD_DECK_TP2_KAGE->duplicate(true), LEGACY_NPC_DECIDE_CHOICE_TP2->duplicate(true));
}
enums::NPCDef::NPC LegacyParse::legacy_npc_tp2_ritchee(const Ref<DataContainer> &container) {
	return add_legacy_npc(container, "tp2-ritchee", "Legacy TP2 Ritchee", "ritchee", LEGACY_NPC_BUILD_DECK_TP2_RITCHEE->duplicate(true), LEGACY_NPC_DECIDE_CHOICE_TP2->duplicate(true));
}
enums::NPCDef::NPC LegacyParse::legacy_npc_tp2_serene(const Ref<DataContainer> &container) {
	return add_legacy_npc(container, "tp2-serene", "Legacy TP2 Serene", "serene", LEGACY_NPC_BUILD_DECK_TP2_SERENE->duplicate(true), LEGACY_NPC_DECIDE_CHOICE_TP2->duplicate(true));
}
enums::NPCDef::NPC LegacyParse::legacy_npc_tp2_carmina(const Ref<DataContainer> &container) {
	return add_legacy_npc(container, "tp2-carmina", "Legacy TP2 Carmina", "carmina", LEGACY_NPC_BUILD_DECK_TP2_CARMINA->duplicate(true), LEGACY_NPC_DECIDE_CHOICE_TP2->duplicate(true));
}
enums::NPCDef::NPC LegacyParse::legacy_npc_tp2_chuck(const Ref<DataContainer> &container) {
	return add_legacy_npc(container, "tp2-chuck", "Legacy TP2 Chuck", "chuck", LEGACY_NPC_BUILD_DECK_TP2_CHUCK->duplicate(true), LEGACY_NPC_DECIDE_CHOICE_TP2->duplicate(true));
}
enums::NPCDef::NPC LegacyParse::legacy_npc_tp2_arie(const Ref<DataContainer> &container) {
	return add_legacy_npc(container, "tp2-arie", "Legacy TP2 Arie", "arie", LEGACY_NPC_BUILD_DECK_TP2_ARIE->duplicate(true), LEGACY_NPC_DECIDE_CHOICE_TP2->duplicate(true));
}
enums::NPCDef::NPC LegacyParse::legacy_npc_tp2_shay(const Ref<DataContainer> &container) {
	return add_legacy_npc(container, "tp2-shay", "Legacy TP2 Shay", "shay", LEGACY_NPC_BUILD_DECK_TP2_SHAY->duplicate(true), LEGACY_NPC_DECIDE_CHOICE_TP2->duplicate(true));
}
enums::NPCDef::NPC LegacyParse::legacy_npc_tp2_crow(const Ref<DataContainer> &container) {
	return add_legacy_npc(container, "tp2-crow", "Legacy TP2 Crow", "crow", LEGACY_NPC_BUILD_DECK_TP2_CROW->duplicate(true), LEGACY_NPC_DECIDE_CHOICE_TP2->duplicate(true));
}

void LegacyParse::finalize_effect_param(const Ref<DataContainer> &container, const Ref<JigsawParameter> &param) {
	if (param.is_null()) {
		return;
	}

	switch (param->get_type()) {
	case JigsawParameter::EFFECT_INSTANCE:
	{
		Ref<JigsawParameterEffectInstance> param_inst = param;
		Ref<EffectInstance> inst = param_inst->get_instance();
		if (inst.is_valid()) {
			TypedArray<JigsawParameter> params = inst->get_params();
			for (int64_t i = 0; i < params.size(); i++) {
				finalize_effect_param(container, params[i]);
			}
		}
		return;
	}
	case JigsawParameter::CARD_FILTER:
	{
		Ref<JigsawParameterCardFilter> param_filter = param;
		finalize_card_filter(container, param_filter->get_filter());
		return;
	}
	case JigsawParameter::LOCATION:
	{
		Ref<JigsawParameterLocation> param_location = param;
		if (param_location->get_location() == enums::LocationDef::FIRST_CUSTOM) {
			String exile = vformat("Exile: %s", param_location->get_meta("legacy_exile", ""));
			param_location->remove_meta("legacy_exile");

			TypedArray<LocationDef> locations = container->get_mode()->get_custom_locations();
			for (int64_t i = 0; i < locations.size(); i++) {
				Ref<LocationDef> location = locations[i];
				if (location->get_editor_name() == exile) {
					param_location->set_location(static_cast<enums::LocationDef::Location>(enums::LocationDef::FIRST_CUSTOM + i));
					return;
				}
			}

			param_location->set_location(static_cast<enums::LocationDef::Location>(enums::LocationDef::FIRST_CUSTOM + locations.size()));

			Ref<LocationDef> new_location;
			new_location.instantiate();
			new_location->set_editor_name(exile);
			locations.append(new_location);
			container->get_mode()->set_custom_locations(locations);
		}
		return;
	}
	default:
		return;
	}
}

void LegacyParse::finalize_card_filter(const Ref<DataContainer> &container, const Ref<CardFilter> &filter) {
	if (filter.is_null()) {
		return;
	}

	switch (filter->get_type()) {
	case CardFilter::AND:
	{
		Ref<CardFilterAnd> filter_and = filter;
		TypedArray<CardFilter> list = filter_and->get_list();
		for (int64_t i = 0; i < list.size(); i++) {
			finalize_card_filter(container, list[i]);
		}
		return;
	}
	case CardFilter::OR:
	{
		Ref<CardFilterOr> filter_or = filter;
		TypedArray<CardFilter> list = filter_or->get_list();
		for (int64_t i = 0; i < list.size(); i++) {
			finalize_card_filter(container, list[i]);
		}
		return;
	}
	case CardFilter::TRIBE:
	{
		Ref<CardFilterTribe> filter_tribe = filter;
		if (filter_tribe->get_tribe() == enums::TribeDef::LEGACY_CUSTOM) {
			filter_tribe->set_tribe(custom_tribe(container, Color(1.0f, 0.0f, 1.0f), filter_tribe->get_meta("legacy_tribe", "")));
			filter_tribe->remove_meta("legacy_tribe");
		}
		return;
	}
	default:
		return;
	}
}
