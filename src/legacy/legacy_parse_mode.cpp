#include "legacy/legacy_parse.h"

bool LegacyParse::game_mode(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, Ref<GameMode> &vanilla) {
	int64_t format_version = fh->read_uvarint();
	ERR_FAIL_COND_V_MSG(format_version != 3, false, vformat("invalid legacy game mode format version %d", format_version));

	int64_t num_fields = fh->read_uvarint();
	for (int64_t i = 0; i < num_fields; i++) {
		ERR_FAIL_COND_V(!field(container, fh, Ref<VariantDef>(), vanilla), false);
	}

	return true;
}

bool LegacyParse::field(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, const Ref<VariantDef> &variant, Ref<GameMode> &vanilla) {
	int64_t field_type = fh->read_uvarint();
	Ref<FormatHelper> sub_fh = FormatHelper::read("legacy game mode field", fh->read_bytesvar());
	switch (field_type) {
	case 0:
		ERR_FAIL_COND_V(!field_metadata(container, sub_fh, variant), false);
		break;
	case 1:
		ERR_FAIL_COND_V(!field_banned_cards(container, sub_fh, variant), false);
		break;
	case 2:
		ERR_FAIL_COND_V(!field_game_rules(container, sub_fh, variant), false);
		break;
	case 3:
		ERR_FAIL_COND_V(!field_summon_card(container, sub_fh, variant), false);
		break;
	case 4:
		ERR_FAIL_COND_V_MSG(variant.is_valid(), false, "invalid nested game mode variant");
		ERR_FAIL_COND_V(!field_variant(container, sub_fh, vanilla), false);
		break;
	case 5:
		ERR_FAIL_COND_V(!field_unfilter_card(container, sub_fh, variant), false);
		break;
	case 6:
		ERR_FAIL_COND_V(!field_deck_limit_filter(container, sub_fh, variant), false);
		break;
	case 7:
		ERR_FAIL_COND_V(!field_timer(container, sub_fh, variant), false);
		break;
	case 8:
		ERR_FAIL_COND_V(!field_turn_0_effect(container, sub_fh, variant), false);
		break;
	case 9:
		ERR_FAIL_COND_V_MSG(variant.is_valid(), false, "cannot decode vanilla version inside variant");
		ERR_FAIL_COND_V(!field_vanilla_version(container, sub_fh, vanilla), false);
		break;
	case 10:
		ERR_FAIL_COND_V(!field_card_group(container, sub_fh, variant), false);
		break;
	case 11:
		ERR_FAIL_COND_V(!field_special_flags(container, sub_fh, variant), false);
		break;
	default:
		ERR_FAIL_V_MSG(false, vformat("invalid legacy game mode field type %d", field_type));
		break;
	}

	ERR_FAIL_COND_V(!sub_fh->is_valid_eof(), false);

	return true;
}

bool LegacyParse::field_metadata(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, const Ref<VariantDef> &variant) {
	String title = fh->read_stringvar();
	String author = fh->read_stringvar();
	String description = fh->read_stringvar();
	String latest_changes = fh->read_stringvar();

	enums::IconDef::Icon portrait = enums::IconDef::NONE;
	if (fh->remaining_len() != 0) {
		portrait = static_cast<enums::IconDef::Icon>(fh->read_uint8());
		if (portrait == enums::IconDef::LEGACY_PORTRAIT_EMBEDDED) {
			portrait = custom_portrait(container, fh->read_bytesvar(), false);
		} else if (portrait == enums::IconDef::LEGACY_PORTRAIT_EXTERNAL) {
			portrait = custom_portrait(container, fh->read_bytesvar(), true);
		}
	}

	if (variant.is_null()) {
		Ref<GameMode> mode = container->get_mode();
		if (mode->get_title().is_empty() && mode->get_author().is_empty() && mode->get_description().is_empty() && mode->get_latest_changes().is_empty() && mode->get_mode_thumbnail() == enums::IconDef::NONE) {
			mode->set_title(title);
			mode->set_author(author);
			mode->set_description(description);
			mode->set_latest_changes(latest_changes);
			mode->set_mode_thumbnail(portrait);
		} else {
			WARN_PRINT_ONCE("TODO: field_metadata (additional)");
		}
	} else {
		WARN_PRINT_ONCE("TODO: field_metadata (variant)");
	}

	return true;
}
bool LegacyParse::field_banned_cards(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, const Ref<VariantDef> &variant) {
	bool banned = true;
	bool unpickable = false;
	bool hidden_on_home = false;

	if (fh->remaining_len() > 1 && fh->peek_byte(0) == 0) {
		(void)fh->read_uint8(); // padding

		uint64_t flags = fh->read_uvarint();

		banned = (flags & 0x03) == 0x00;
		unpickable = (flags & 0x03) == 0x01;
		hidden_on_home = (flags & 0x03) == 0x02;

		ERR_FAIL_COND_V(!banned && !unpickable && !hidden_on_home, false);
	}

	PackedArray<enums::CardDef::Card> banned_cards;
	banned_cards.resize(fh->read_uvarint());
	for (int64_t i = 0; i < banned_cards.size(); i++) {
		banned_cards[i] = static_cast<enums::CardDef::Card>(fh->read_uvarint());
	}

	WARN_PRINT_ONCE("TODO: field_banned_cards");

	return true;
}
bool LegacyParse::field_game_rules(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, const Ref<VariantDef> &variant) {
	for (int64_t rule_index = fh->read_uvarint() - 1; rule_index != -1; rule_index = fh->read_uvarint() - 1) {
		enums::VariableDef::Variable rule = static_cast<enums::VariableDef::Variable>(rule_index + enums::VariableDef::FIRST_LEGACY_GAME_RULE);
		ERR_FAIL_COND_V(rule >= enums::VariableDef::FIRST_CUSTOM, false);
		int64_t value = fh->read_uvarint();
		if (rule == enums::VariableDef::HAND_MAX_SIZE) {
			value = MIN(value, 50);
		}
	}

	WARN_PRINT_ONCE("TODO: field_game_rules");

	return true;
}
bool LegacyParse::field_summon_card(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, const Ref<VariantDef> &variant) {
	uint64_t flags = fh->read_uvarint();
	bool both_players = (flags & 0x01) != 0;
	enums::CardDef::Card card_id = static_cast<enums::CardDef::Card>(fh->read_uvarint());

	WARN_PRINT_ONCE("TODO: field_summon_card");

	return true;
}
bool LegacyParse::field_variant(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, Ref<GameMode> &vanilla) {
	Ref<VariantDef> variant;
	variant.instantiate();

	variant->set_title(fh->read_stringvar());
	String npc = fh->read_stringvar();
	if (!npc.is_empty()) {
		variant->set_player_count(1);
		variant->set_meta(meta_legacy_npc, npc);
	}

	TypedArray<VariantDef> variants = container->get_mode()->get_variants();
	variants.append(variant);
	container->get_mode()->set_variants(variants);

	int64_t num_fields = fh->read_uvarint();
	for (int64_t i = 0; i < num_fields; i++) {
		ERR_FAIL_COND_V(!field(container, fh, variant, vanilla), false);
	}

	return true;
}
bool LegacyParse::field_unfilter_card(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, const Ref<VariantDef> &variant) {
	(void)fh->read_uvarint(); // unused flags field
	enums::CardDef::Card card_id = static_cast<enums::CardDef::Card>(fh->read_uvarint());

	WARN_PRINT_ONCE("TODO: field_unfilter_card");

	return true;
}
bool LegacyParse::field_deck_limit_filter(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, const Ref<VariantDef> &variant) {
	int64_t count = fh->read_uvarint();
	Ref<CardFilter> filter;
	int64_t cond_count = 0;
	Ref<CardFilter> condition;

	uint8_t rank_tribe = fh->read_uint8();
	if (rank_tribe == 128) {
		filter = read_card_filter_v5(fh);

		if (fh->remaining_len() != 0) {
			cond_count = fh->read_uvarint();
			condition = read_card_filter_v5(fh);
		}
	} else {
		enums::RankDef::Rank rank = static_cast<enums::RankDef::Rank>(rank_tribe >> 4);
		if (rank == enums::RankDef::TOKEN) {
			rank = enums::RankDef::LEGACY_ENEMY;
		}
		Ref<CardFilter> rank_filter = rank != enums::RankDef::LEGACY_NONE ? CardFilter::make_rank(rank) : Ref<CardFilter>();

		enums::TribeDef::Tribe tribe = static_cast<enums::TribeDef::Tribe>(rank_tribe & 15);
		Ref<CardFilter> tribe_filter = tribe != enums::TribeDef::LEGACY_NONE ? CardFilter::make_tribe(tribe) : Ref<CardFilter>();
		if (tribe == enums::TribeDef::LEGACY_CUSTOM) {
			tribe_filter->set_meta(meta_legacy_tribe, fh->read_stringvar());
		}

		filter = CardFilter::make_and(Array::make(rank_filter, tribe_filter));
	}

	WARN_PRINT_ONCE("TODO: field_deck_limit_filter");

	return true;
}
bool LegacyParse::field_timer(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, const Ref<VariantDef> &variant) {
	int64_t start_time = fh->read_uvarint();
	int64_t max_time = fh->read_uvarint();
	int64_t per_turn = fh->read_uvarint();
	int64_t max_per_turn = fh->read_uvarint();

	WARN_PRINT_ONCE("TODO: field_timer");

	return true;
}
bool LegacyParse::field_turn_0_effect(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, const Ref<VariantDef> &variant) {
	(void)fh->read_uvarint(); // unused flags field
	int64_t effect_format = fh->read_uvarint();
	Ref<EffectInstance> effect;
	effect.instantiate();
	switch (effect_format) {
	case 0:
	case 1:
		effect_v0_v1(container, effect, fh, effect_format);
		break;
	case 2:
	case 4:
		effect_v2_v4(container, effect, fh, effect_format);
		break;
	case 5:
		effect_v5(container, effect, fh, effect_format);
		break;
	default:
		ERR_FAIL_V_MSG(false, vformat("unknown turn 0 effect version %d", effect_format));
		break;
	}

	WARN_PRINT_ONCE("TODO: field_turn_0_effect");

	return true;
}
bool LegacyParse::field_vanilla_version(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, Ref<GameMode> &vanilla) {
	int64_t vanilla_version = fh->read_uvarint();
	switch (vanilla_version) {
	case 0:
		vanilla = VANILLA_1_0_5->get_mode();
		break;
	case 1:
		vanilla = VANILLA_1_1->get_mode();
		break;
	case 2:
		vanilla = VANILLA_1_1_1->get_mode();
		break;
	case 3:
		vanilla = VANILLA_1_2_1->get_mode();
		break;
	default:
		ERR_FAIL_V_MSG(false, vformat("out of range value for vanilla version: %d", vanilla_version));
	}

	return true;
}
bool LegacyParse::field_card_group(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, const Ref<VariantDef> &variant) {
	(void)fh->read_uvarint(); // unused flags field

	String title = fh->read_stringvar();

	PackedArray<enums::CardDef::Card> cards;
	cards.resize(fh->read_uvarint());
	for (int64_t i = 0; i < cards.size(); i++) {
		cards[i] = static_cast<enums::CardDef::Card>(fh->read_uvarint());
	}

	WARN_PRINT_ONCE("TODO: field_card_group");

	return true;
}
bool LegacyParse::field_special_flags(const Ref<DataContainer> &container, const Ref<FormatHelper> &fh, const Ref<VariantDef> &variant) {
	(void)fh->read_uvarint(); // unused flags flags field

	int64_t count = fh->read_uvarint();
	for (int64_t i = 0; i < count; i++) {
		int64_t flag = fh->read_uvarint();
		switch (flag) {
		case 1:
			// TODO: Allow Subtracting Infinities
			WARN_PRINT_ONCE("TODO: field_special_flags (Allow Subtracting Infinities)");
			break;
		case 2:
			// TODO: Use Effect Card Back
			WARN_PRINT_ONCE("TODO: field_special_flags (Use Effect Card Back)");
			break;
		case 3:
			// TODO: No Damage on Win
			WARN_PRINT_ONCE("TODO: field_special_flags (No Damage on Win)");
			break;
		case 4:
			// TODO: Reveal Card Selection
			WARN_PRINT_ONCE("TODO: field_special_flags (Reveal Card Selection)");
			break;
		case 5:
			// TODO: Both Players are Player 1
			WARN_PRINT_ONCE("TODO: field_special_flags (Both Players are Player 1)");
			break;
		case 6:
			// TODO: Auto-Ready if Negative TP
			WARN_PRINT_ONCE("TODO: field_special_flags (Auto-Ready if Negative TP)");
			break;
		case 7:
			// TODO: Hide Game Log
			WARN_PRINT_ONCE("TODO: field_special_flags (Hide Game Log)");
			break;
		default:
			ERR_FAIL_V_MSG(false, vformat("undefined legacy special flag %d", flag));
			break;
		}
	}

	return true;
}
