#include "legacy/legacy_parse.h"

#include "jigsaw/parameter/jigsaw_parameter_amount.h"
#include "jigsaw/parameter/jigsaw_parameter_stat.h"

enums::RankDef::Rank LegacyParse::card_rank(enums::CardDef::Card id) {
	if (id >= enums::CardDef::FIRST_CUSTOM) {
		return static_cast<enums::RankDef::Rank>((id >> 5) & 3);
	}

	switch (id) {
	case enums::CardDef::ZOMBIANT:
	case enums::CardDef::JELLYSHROOM:
	case enums::CardDef::PSICORP:
	case enums::CardDef::THIEF:
	case enums::CardDef::BANDIT:
	case enums::CardDef::SEEDLING:
	case enums::CardDef::CHOMPER:
	case enums::CardDef::WASP_TROOPER:
	case enums::CardDef::WASP_SCOUT:
	case enums::CardDef::UNDERLING:
	case enums::CardDef::GOLDEN_SEEDLING:
	case enums::CardDef::ARROW_WORM:
	case enums::CardDef::BURGLAR:
	case enums::CardDef::SECURITY_TURRET:
	case enums::CardDef::ABOMIHONEY:
	case enums::CardDef::ZOMBEE:
	case enums::CardDef::ZOMBEETLE:
	case enums::CardDef::BLOATSHROOM:
	case enums::CardDef::KRAWLER:
	case enums::CardDef::HAUNTED_CLOTH:
	case enums::CardDef::WARDEN:
	case enums::CardDef::JUMPING_SPIDER:
	case enums::CardDef::MIMIC_SPIDER:
	case enums::CardDef::CHOMPER_BRUTE:
	case enums::CardDef::MANTIDFLY:
	case enums::CardDef::MOTHFLY:
	case enums::CardDef::MOTHFLY_CLUSTER:
	case enums::CardDef::BELOSTOSS:
	case enums::CardDef::RUFFIAN:
	case enums::CardDef::WATER_STRIDER:
	case enums::CardDef::DIVING_SPIDER:
		return enums::RankDef::ATTACKER;
	case enums::CardDef::CACTILING:
	case enums::CardDef::INICHAS:
	case enums::CardDef::NUMBNAIL:
	case enums::CardDef::ACORNLING:
	case enums::CardDef::WEEVIL:
	case enums::CardDef::VENUS_BUD:
	case enums::CardDef::WASP_BOMBER:
	case enums::CardDef::WASP_DRILLER:
	case enums::CardDef::MIDGE: // attacker in modern Spy Cards, but this isn't modern Spy Cards
	case enums::CardDef::PLUMPLING:
	case enums::CardDef::FLOWERLING:
	case enums::CardDef::BEE_BOOP:
	case enums::CardDef::DENMUKI:
	case enums::CardDef::MENDER:
	case enums::CardDef::LEAFBUG_NINJA:
	case enums::CardDef::LEAFBUG_ARCHER:
	case enums::CardDef::LEAFBUG_CLUBBER:
	case enums::CardDef::MADESPHY:
	case enums::CardDef::WILD_CHOMPER:
	case enums::CardDef::IRONNAIL:
		return enums::RankDef::EFFECT;
	case enums::CardDef::ZASP:
	case enums::CardDef::MOTHIVA:
	case enums::CardDef::ACOLYTE_ARIA:
	case enums::CardDef::KABBU:
	case enums::CardDef::MONSIEUR_SCARLET:
	case enums::CardDef::CARMINA:
	case enums::CardDef::ASTOTHELES:
	case enums::CardDef::DUNE_SCORPION:
	case enums::CardDef::KALI:
	case enums::CardDef::GENERAL_ULTIMAX:
	case enums::CardDef::CROSS:
	case enums::CardDef::POI:
	case enums::CardDef::PRIMAL_WEEVIL:
	case enums::CardDef::CENN:
	case enums::CardDef::PISCI:
	case enums::CardDef::DEAD_LANDER_ALPHA:
	case enums::CardDef::DEAD_LANDER_BETA:
	case enums::CardDef::DEAD_LANDER_GAMMA:
	case enums::CardDef::KINA:
	case enums::CardDef::YIN:
	case enums::CardDef::RIZ:
	case enums::CardDef::STRATOS:
	case enums::CardDef::DELILAH:
		return enums::RankDef::MINI_BOSS;
	case enums::CardDef::SPIDER:
	case enums::CardDef::VENUS_GUARDIAN:
	case enums::CardDef::SEEDLING_KING:
	case enums::CardDef::BROODMOTHER:
	case enums::CardDef::MOTHER_CHOMPER:
	case enums::CardDef::AHONEYNATION:
	case enums::CardDef::HEAVY_DRONE_B33:
	case enums::CardDef::TIDAL_WYRM:
	case enums::CardDef::THE_WATCHER:
	case enums::CardDef::PEACOCK_SPIDER:
	case enums::CardDef::THE_BEAST:
	case enums::CardDef::FALSE_MONARCH:
	case enums::CardDef::WASP_KING:
	case enums::CardDef::THE_EVERLASTING_KING:
	case enums::CardDef::MAKI:
	case enums::CardDef::ULTIMAX_TANK:
	case enums::CardDef::ZOMMOTH:
	case enums::CardDef::DEVOURER:
		return enums::RankDef::BOSS;
	default:
		return enums::RankDef::NONE;
	}
}
String LegacyParse::card_name(enums::CardDef::Card id) {
	if (id >= enums::CardDef::FIRST_CUSTOM) {
		int64_t n = (((id >> 2) & ~31) | (id & 31)) - 31;
		switch (card_rank(id)) {
		case enums::RankDef::ATTACKER:
			return vformat("Custom Attacker #%d", n);
		case enums::RankDef::EFFECT:
			return vformat("Custom Effect #%d", n);
		case enums::RankDef::MINI_BOSS:
			return vformat("Custom Mini-Boss #%d", n);
		case enums::RankDef::BOSS:
			return vformat("Custom Boss #%d", n);
		}
	}

	Ref<CardDef> vanilla_card = VANILLA_1_2_1->get_card(id);
	if (vanilla_card.is_valid()) {
		return vanilla_card->get_name();
	}

	return vformat("MissingCard?%d?", id);
}

bool LegacyParse::card(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh) {
	int64_t format_version = fh->read_uvarint();
	switch (format_version) {
	case 0:
	case 1:
		return card_v0_v1(container, fh, format_version);
	case 2:
	case 4:
		return card_v2_v4(container, fh, format_version);
	case 5:
		return card_v5(container, fh, format_version);
	default:
		ERR_FAIL_V_MSG(false, vformat("invalid legacy card format version %d", format_version));
	}
}
bool LegacyParse::card_v0_v1(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, int64_t format_version) {
	Ref<CardDef> card;
	card.instantiate();

	card->set_id(static_cast<enums::CardDef::Card>(fh->read_uint8()));
	card->set_rank(card_rank(card->get_id()));
	ERR_FAIL_COND_V(card->get_rank() == enums::RankDef::NONE, false);

	uint8_t tribes = fh->read_uint8();
	ERR_FAIL_COND_V((tribes >> 4) == enums::TribeDef::LEGACY_NONE, false);
	if ((tribes & 15) == enums::TribeDef::LEGACY_NONE) {
		card->set_tribes(Array::make(tribes >> 4));
	} else {
		card->set_tribes(Array::make(tribes >> 4, tribes & 15));
	}

	uint8_t rank_tp = fh->read_uint8();
	uint8_t tp = rank_tp & 15;
	ERR_FAIL_COND_V(tp > 10 && tp != 15, false);
	if (likely(tp <= 10)) {
		card->set_costs(Array::make(StatValue::make(enums::StatDef::TP, rank_tp & 15)));
	} else {
		card->set_costs(Array::make(StatValue::make(enums::StatDef::TP, 1)));
		card->set_meta("legacy_unpickable", true);
	}

	ERR_FAIL_COND_V(((rank_tp >> 4) & 3) != card->get_rank(), false);

	bool external_portrait = (rank_tp & 0x40) != 0;

	enums::IconDef::Icon portrait = static_cast<enums::IconDef::Icon>(fh->read_uint8());
	ERR_FAIL_COND_V(portrait > 235 && portrait != enums::IconDef::LEGACY_PORTRAIT_EXTERNAL, false);
	ERR_FAIL_COND_V(portrait != enums::IconDef::LEGACY_PORTRAIT_EXTERNAL && external_portrait, false);

	if (portrait != enums::IconDef::LEGACY_PORTRAIT_EXTERNAL && portrait >= 228) {
		portrait = static_cast<enums::IconDef::Icon>(portrait + 19);
	}

	card->set_portrait(portrait);
	card->set_name(fh->read_string1());
	if (card->get_name().is_empty()) {
		card->set_name(card_name(card->get_id()));
	}

	if (format_version == 0 && card->get_rank() == enums::RankDef::ATTACKER && (fh->is_eof() || fh->peek_byte(0) == 137)) {
		Ref<EffectInstance> atk;
		atk.instantiate();
		atk->set_effect(enums::EffectDef::STAT);
		atk->set_priority(65);
		atk->set_params(Array::make(
			JigsawParameterStat::make(enums::StatDef::ATK),
			tp != 15 ? JigsawParameterAmount::make(tp) : JigsawParameterAmount::make(0, 1)
		));
		card->set_effects(Array::make(atk));
	} else {
		TypedArray<EffectInstance> effects;
		effects.resize(fh->read_uint8());
		for (int64_t i = 0; i < effects.size(); i++) {
			Ref<EffectInstance> effect;
			effect.instantiate();
			ERR_FAIL_COND_V_MSG(!effect_v0_v1(container, effect, fh, format_version), false, vformat("failed to parse effect at index %d of card %s", i, card->get_name()));
			effects[i] = effect;
		}
		card->set_effects(effects);
	}

	PackedArray<enums::TribeDef::Tribe> tribe_ids = card->get_tribes();
	for (int64_t i = 0; i < tribe_ids.size(); i++) {
		int64_t tribe_id = tribe_ids[i];
		if (tribe_id != enums::TribeDef::LEGACY_CUSTOM) {
			continue;
		}

		Color tribe_color = fh->read_color_rgb888();
		String tribe_name = fh->read_string1();

		tribe_ids[i] = custom_tribe(container, tribe_color, tribe_name);
		card->set_tribes(tribe_ids);
	}

	if (portrait == enums::IconDef::LEGACY_PORTRAIT_EXTERNAL) {
		card->set_portrait(custom_portrait(container, fh->read_bytes(fh->remaining_len()), external_portrait));
	}

	TypedArray<CardDef> cards = container->get_mode()->get_card_defs();
	cards.append(card);
	container->get_mode()->set_card_defs(cards);

	return true;
}
bool LegacyParse::card_v2_v4(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, int64_t format_version) {
	Ref<CardDef> card;
	card.instantiate();

	card->set_id(static_cast<enums::CardDef::Card>(fh->read_uvarint()));
	card->set_rank(card_rank(card->get_id()));

	uint8_t legacy_tribes = fh->read_uint8();

	ERR_FAIL_COND_V((legacy_tribes >> 4) == enums::TribeDef::LEGACY_NONE, false);
	PackedArray<enums::TribeDef::Tribe> tribes;
	tribes.append(legacy_tribes >> 4);

	if ((legacy_tribes & 15) != enums::TribeDef::LEGACY_NONE) {
		tribes.append(legacy_tribes & 15);
	}

	uint8_t special = fh->read_uint8();
	while ((special >> 4) != 0) {
		ERR_FAIL_COND_V((special >> 4) != 1, false);
		ERR_FAIL_COND_V(tribes.size() < 2, false);
		ERR_FAIL_COND_V((special & 15) == enums::TribeDef::LEGACY_NONE, false);

		if ((special & 15) == enums::TribeDef::LEGACY_CUSTOM) {
			Color tribe_color = fh->read_color_rgb888();
			String tribe_name = fh->read_stringvar();
			tribes.append(custom_tribe(container, tribe_color, tribe_name));
		}

		special = fh->read_uint8();
	}

	ERR_FAIL_COND_V(special > 10 && special != 15, false);
	Ref<StatValue> tp_cost;
	if (likely(special <= 10)) {
		tp_cost = StatValue::make(enums::StatDef::TP, special);
	} else {
		tp_cost = StatValue::make(enums::StatDef::TP, 1);
		card->set_meta("legacy_unpickable", true);
	}
	card->set_costs(Array::make(tp_cost));

	card->set_portrait(static_cast<enums::IconDef::Icon>(fh->read_uint8()));

	ERR_FAIL_COND_V(card->get_portrait() > 235 && card->get_portrait() < 254, false);
	if (card->get_portrait() >= 228 && card->get_portrait() <= 235) {
		card->set_portrait(static_cast<enums::IconDef::Icon>(card->get_portrait() + 19));
	}

	card->set_name(fh->read_stringvar());
	if (card->get_name().is_empty()) {
		card->set_name(card_name(card->get_id()));
	}

	TypedArray<EffectInstance> effects;
	effects.resize(fh->read_uvarint());
	int64_t j = 0;
	for (int64_t i = 0; i < effects.size(); i++) {
		Ref<EffectInstance> effect;
		effect.instantiate();
		ERR_FAIL_COND_V_MSG(!effect_v2_v4(container, effect, fh, format_version), false, vformat("failed to parse effect at index %d of card %s", i, card->get_name()));

		// collapse TP effect into just being part of the cost
		if (effect->get_effect() == enums::EffectDef::TP) {
			Ref<JigsawParameterAmount> amount = effect->get_params()[0];
			tp_cost->set_amount(tp_cost->get_amount() - amount->get_amount());
			continue;
		}

		effects[j] = effect;
		j++;
	}
	effects.resize(j);
	card->set_effects(effects);

	for (int64_t i = 0; i < tribes.size() && i < 2; i++) {
		int64_t tribe_id = tribes[i];
		if (tribe_id != enums::TribeDef::LEGACY_CUSTOM) {
			continue;
		}

		Color tribe_color = fh->read_color_rgb888();
		String tribe_name = fh->read_stringvar();

		tribes[i] = custom_tribe(container, tribe_color, tribe_name);
	}
	card->set_tribes(tribes);

	if (card->get_portrait() == enums::IconDef::LEGACY_PORTRAIT_EXTERNAL) {
		card->set_portrait(custom_portrait(container, fh->read_bytesvar(), true));
	} else if (card->get_portrait() == enums::IconDef::LEGACY_PORTRAIT_EMBEDDED) {
		card->set_portrait(custom_portrait(container, fh->read_bytesvar(), false));
	}

	TypedArray<CardDef> cards = container->get_mode()->get_card_defs();
	cards.append(card);
	container->get_mode()->set_card_defs(cards);

	return true;
}
bool LegacyParse::card_v5(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, int64_t format_version) {
	Ref<CardDef> card;
	card.instantiate();

	card->set_id(static_cast<enums::CardDef::Card>(fh->read_uvarint()));

	card->set_name(fh->read_stringvar());
	if (card->get_name().is_empty()) {
		card->set_name(card_name(card->get_id()));
	}

	card->set_costs(Array::make(StatValue::make(enums::StatDef::TP, fh->read_svarint())));

	card->set_portrait(static_cast<enums::IconDef::Icon>(fh->read_uint8()));
	ERR_FAIL_COND_V(card->get_portrait() >= 233 && card->get_portrait() < 247, false);

	uint8_t rank_tribe = fh->read_uint8();
	card->set_rank(static_cast<enums::RankDef::Rank>(rank_tribe >> 4));
	ERR_FAIL_COND_V(card->get_rank() > enums::RankDef::TOKEN, false);

	PackedArray<enums::TribeDef::Tribe> tribes;
	ERR_FAIL_COND_V((rank_tribe & 15) == enums::TribeDef::LEGACY_NONE, false);
	if ((rank_tribe & 15) == enums::TribeDef::LEGACY_CUSTOM) {
		Color tribe_color = fh->read_color_rgb888();
		String tribe_name = fh->read_stringvar();

		tribes.append(custom_tribe(container, tribe_color, tribe_name));
	} else {
		tribes.append(rank_tribe & 15);
	}

	int64_t additional_tribes = fh->read_uvarint();
	tribes.resize(1 + additional_tribes);

	for (int64_t i = 0; i < additional_tribes; i += 2) {
		uint8_t packed_tribes = fh->read_uint8();

		ERR_FAIL_COND_V((packed_tribes >> 4) == enums::TribeDef::LEGACY_NONE, false);
		if ((packed_tribes >> 4) == enums::TribeDef::LEGACY_CUSTOM) {
			Color tribe_color = fh->read_color_rgb888();
			String tribe_name = fh->read_stringvar();

			tribes.append(custom_tribe(container, tribe_color, tribe_name));
		} else {
			tribes.append(packed_tribes >> 4);
		}

		if (i + 1 == additional_tribes) {
			ERR_FAIL_COND_V((packed_tribes & 15) != enums::TribeDef::LEGACY_NONE, false);

			break;
		}

		ERR_FAIL_COND_V((packed_tribes & 15) == enums::TribeDef::LEGACY_NONE, false);
		if ((packed_tribes & 15) == enums::TribeDef::LEGACY_CUSTOM) {
			Color tribe_color = fh->read_color_rgb888();
			String tribe_name = fh->read_stringvar();

			tribes.append(custom_tribe(container, tribe_color, tribe_name));
		} else {
			tribes.append(packed_tribes & 15);
		}
	}

	card->set_tribes(tribes);

	TypedArray<EffectInstance> effects;
	effects.resize(fh->read_uvarint());

	for (int64_t i = 0; i < effects.size(); i++) {
		Ref<EffectInstance> effect;
		effect.instantiate();
		ERR_FAIL_COND_V_MSG(!effect_v5(container, effect, fh, format_version), false, vformat("failed to parse effect at index %d of card %s", i, card->get_name()));
		effects[i] = effect;
	}

	if (card->get_portrait() == enums::IconDef::LEGACY_PORTRAIT_EXTERNAL) {
		card->set_portrait(custom_portrait(container, fh->read_bytesvar(), true));
	} else if (card->get_portrait() == enums::IconDef::LEGACY_PORTRAIT_EMBEDDED) {
		card->set_portrait(custom_portrait(container, fh->read_bytesvar(), false));
	}

	TypedArray<EffectInstance> extensions;
	extensions.resize(fh->read_uvarint());

	for (int64_t i = 0; i < extensions.size(); i++) {
		Ref<EffectInstance> extension;
		extension.instantiate();
		ERR_FAIL_COND_V_MSG(!extension_v5(container, extension, fh, format_version), false, vformat("failed to parse extension at index %d of card %s", i, card->get_name()));
		extensions[i] = extension;
	}

	effects.append_array(extensions);
	card->set_effects(effects);

	TypedArray<CardDef> cards = container->get_mode()->get_card_defs();
	cards.append(card);
	container->get_mode()->set_card_defs(cards);

	return true;
}
