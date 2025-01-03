#include "legacy/legacy_parse.h"

#include "defs/card_filter_and.h"
#include "defs/card_filter_or.h"
#include "defs/card_filter_tribe.h"
#include "jigsaw/parameter/jigsaw_parameter_card_filter.h"
#include "jigsaw/parameter/jigsaw_parameter_effect_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_location.h"

#include <godot_cpp/classes/marshalls.hpp>

LazyGlobal<GameMode> LegacyParse::VANILLA_1_0_5{[]() -> Ref<GameMode> { return get_predefined_defs()->get("VANILLA_1_0_5"); }};
LazyGlobal<GameMode> LegacyParse::VANILLA_1_1{[]() -> Ref<GameMode> { return get_predefined_defs()->get("VANILLA_1_1"); }};
LazyGlobal<GameMode> LegacyParse::VANILLA_1_1_1{[]() -> Ref<GameMode> { return get_predefined_defs()->get("VANILLA_1_1_1"); }};
LazyGlobal<GameMode> LegacyParse::VANILLA_1_2_1{[]() -> Ref<GameMode> { return get_predefined_defs()->get("VANILLA_1_2_1"); }};

void LegacyParse::_bind_methods() {
	ClassDB::bind_static_method("LegacyParse", D_METHOD("card_set", "buf", "vanilla_default", "p1_spoiler_guard", "p2_spoiler_guard"), &LegacyParse::card_set, DEFVAL(Ref<GameMode>()), DEFVAL(PackedByteArray()), DEFVAL(PackedByteArray()));
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
			// TODO!!!
			ERR_FAIL_V_MSG(Ref<DataContainer>(), vformat("TODO: legacy_unpickable (#%d: %s)", card->get_id(), card->get_name())); // TODO: legacy_unpickable
			card->remove_meta("legacy_unpickable");
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

	TypedArray<enums::TribeDef::Tribe> mode_tribes = container->get_mode()->get_tribes();
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
		return legacy_npc_card_master(container, "carmina", "01H00000000013HSMR");
	} else if (code == "carmina2") {
		return legacy_npc_card_master(container, "carmina", "3P7T52H8MA5273HG842YF7KR");
	} else if (code == "chuck") {
		return legacy_npc_card_master(container, "chuck", "4HH0000007VXYZFG84210GG8");
	} else if (code == "arie") {
		return legacy_npc_card_master(container, "arie", "310J10G84212NANCPAD6K7VW");
	} else if (code == "shay") {
		return legacy_npc_card_master(container, "shay", "511KHRWE631GRD6K9MMA5840");
	} else if (code == "crow") {
		return legacy_npc_card_master(container, "crow", "101AXEPH8MCJ8G845G");
	} else if (code == "genow") {
		return legacy_npc_card_master(container, "genow", "6N5Q3HRWE6VDQ6K9G8");
	} else if (code == "mender-spam") {
		return legacy_npc_mender_spam(container, enums::CardDef::NONE);
	} else if (code == "mender-spam-mothiva") {
		return legacy_npc_mender_spam(container, enums::CardDef::MOTHIVA);
	} else if (code == "mender-spam-kali") {
		return legacy_npc_mender_spam(container, enums::CardDef::KALI);
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
		return legacy_npc_card_master(container, code.get_slice("-", 2), code.get_slice("-", 1));
	} else {
		ERR_FAIL_V_MSG(legacy_npc_generic(container, code), vformat("unhandled custom NPC code: %s", code));
	}
}

enums::NPCDef::NPC LegacyParse::legacy_npc_generic(const Ref<DataContainer> &container, const String &code) {
	ERR_FAIL_V(enums::NPCDef::NONE); // TODO
}
enums::NPCDef::NPC LegacyParse::legacy_npc_tourney_player(const Ref<DataContainer> &container, const String &name) {
	ERR_FAIL_V(enums::NPCDef::NONE); // TODO
}
enums::NPCDef::NPC LegacyParse::legacy_npc_janet(const Ref<DataContainer> &container) {
	ERR_FAIL_V(enums::NPCDef::NONE); // TODO
}
enums::NPCDef::NPC LegacyParse::legacy_npc_card_master(const Ref<DataContainer> &container, const String &name, const String &deck) {
	ERR_FAIL_V(enums::NPCDef::NONE); // TODO
}
enums::NPCDef::NPC LegacyParse::legacy_npc_mender_spam(const Ref<DataContainer> &container, enums::CardDef::Card mini_boss) {
	ERR_FAIL_V(enums::NPCDef::NONE); // TODO
}
enums::NPCDef::NPC LegacyParse::legacy_npc_tp2_generic(const Ref<DataContainer> &container) {
	ERR_FAIL_V(enums::NPCDef::NONE); // TODO
}
enums::NPCDef::NPC LegacyParse::legacy_npc_tp2_janet(const Ref<DataContainer> &container) {
	ERR_FAIL_V(enums::NPCDef::NONE); // TODO
}
enums::NPCDef::NPC LegacyParse::legacy_npc_tp2_bu_gi(const Ref<DataContainer> &container) {
	ERR_FAIL_V(enums::NPCDef::NONE); // TODO
}
enums::NPCDef::NPC LegacyParse::legacy_npc_tp2_johnny(const Ref<DataContainer> &container) {
	ERR_FAIL_V(enums::NPCDef::NONE); // TODO
}
enums::NPCDef::NPC LegacyParse::legacy_npc_tp2_kage(const Ref<DataContainer> &container) {
	ERR_FAIL_V(enums::NPCDef::NONE); // TODO
}
enums::NPCDef::NPC LegacyParse::legacy_npc_tp2_ritchee(const Ref<DataContainer> &container) {
	ERR_FAIL_V(enums::NPCDef::NONE); // TODO
}
enums::NPCDef::NPC LegacyParse::legacy_npc_tp2_serene(const Ref<DataContainer> &container) {
	ERR_FAIL_V(enums::NPCDef::NONE); // TODO
}
enums::NPCDef::NPC LegacyParse::legacy_npc_tp2_carmina(const Ref<DataContainer> &container) {
	ERR_FAIL_V(enums::NPCDef::NONE); // TODO
}
enums::NPCDef::NPC LegacyParse::legacy_npc_tp2_chuck(const Ref<DataContainer> &container) {
	ERR_FAIL_V(enums::NPCDef::NONE); // TODO
}
enums::NPCDef::NPC LegacyParse::legacy_npc_tp2_arie(const Ref<DataContainer> &container) {
	ERR_FAIL_V(enums::NPCDef::NONE); // TODO
}
enums::NPCDef::NPC LegacyParse::legacy_npc_tp2_shay(const Ref<DataContainer> &container) {
	ERR_FAIL_V(enums::NPCDef::NONE); // TODO
}
enums::NPCDef::NPC LegacyParse::legacy_npc_tp2_crow(const Ref<DataContainer> &container) {
	ERR_FAIL_V(enums::NPCDef::NONE); // TODO
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
	}
}
