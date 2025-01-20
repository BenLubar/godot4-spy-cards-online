#include "legacy/legacy_parse.h"

#include "util/base32.h"

#include "jigsaw/parameter/jigsaw_parameter_boolean.h"
#include "jigsaw/parameter/jigsaw_parameter_card.h"
#include "jigsaw/parameter/jigsaw_parameter_card_filter.h"
#include "jigsaw/parameter/jigsaw_parameter_cid_opus.h"
#include "jigsaw/parameter/jigsaw_parameter_color.h"
#include "jigsaw/parameter/jigsaw_parameter_effect_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_icon.h"
#include "jigsaw/parameter/jigsaw_parameter_location.h"
#include "jigsaw/parameter/jigsaw_parameter_stat.h"
#include "jigsaw/parameter/jigsaw_parameter_string.h"

LazyPredefined<JigsawParameterFileIDOpus> LegacyParse::SOUND_ATK_SUCCESS{ "SOUND_ATK_SUCCESS" };
LazyPredefined<JigsawParameterFileIDOpus> LegacyParse::SOUND_ATK_FAIL{ "SOUND_ATK_FAIL" };
LazyPredefined<JigsawParameterFileIDOpus> LegacyParse::SOUND_NULL{ "SOUND_NULL" };

Ref<JigsawParameterAmount> LegacyParse::read_amount_v0_v1(const Ref<FormatHelper> &fh) {
	int64_t amount = fh->read_sint8();
	int64_t infinity = amount == 0 ? 1 : 0;
	return JigsawParameterAmount::make(amount, infinity);
}

bool LegacyParse::effect_v0_v1(const Ref<DataContainer> &container, const Ref<EffectInstance> &effect, const Ref<FormatHelper> &fh, int64_t format_version) {
	uint8_t type = fh->read_uint8();

	switch (type) {
	case 0: // ATK (static)
	{
		Ref<JigsawParameterAmount> amount = read_amount_v0_v1(fh);

		effect->set_effect(enums::EffectDef::STAT);
		effect->set_priority(65);
		effect->set_params(Array::make(
			JigsawParameterStat::make(enums::StatDef::ATK),
			amount
		));

		break;
	}
	case 1: // DEF (static)
	{
		Ref<JigsawParameterAmount> amount = read_amount_v0_v1(fh);

		effect->set_effect(enums::EffectDef::STAT);
		effect->set_priority(65);
		effect->set_params(Array::make(
			JigsawParameterStat::make(enums::StatDef::DEF),
			amount
		));

		break;
	}
	case 2: // ATK (static, once)
	{
		Ref<JigsawParameterAmount> amount = read_amount_v0_v1(fh);

		Ref<EffectInstance> atk;
		atk.instantiate();
		atk->set_effect(enums::EffectDef::STAT);
		atk->set_priority(65);
		atk->set_params(Array::make(
			JigsawParameterStat::make(enums::StatDef::ATK),
			amount
		));

		effect->set_effect(enums::EffectDef::COND_LIMIT);
		effect->set_priority(10);
		effect->set_params(Array::make(
			JigsawParameterAmount::make(1),
			JigsawParameterEffectInstance::make(atk),
			JigsawParameterBoolean::make(false)
		));

		break;
	}
	case 3: // ATK
	{
		Ref<JigsawParameterAmount> amount = read_amount_v0_v1(fh);

		effect->set_effect(enums::EffectDef::STAT);
		effect->set_priority(65);
		effect->set_params(Array::make(
			JigsawParameterStat::make(enums::StatDef::ATK),
			amount
		));

		break;
	}
	case 4: // DEF
	{
		Ref<JigsawParameterAmount> amount = read_amount_v0_v1(fh);

		effect->set_effect(enums::EffectDef::STAT);
		effect->set_priority(65);
		effect->set_params(Array::make(
			JigsawParameterStat::make(enums::StatDef::DEF),
			amount
		));

		break;
	}
	case 5: // empower (card)
	{
		enums::CardDef::Card card_id = static_cast<enums::CardDef::Card>(fh->read_uint8());
		Ref<JigsawParameterAmount> amount = read_amount_v0_v1(fh);

		effect->set_effect(enums::EffectDef::EMPOWER);
		effect->set_priority(80);
		effect->set_params(Array::make(
			JigsawParameterStat::make(enums::StatDef::ATK),
			amount,
			JigsawParameterCardFilter::make(CardFilter::make_card(card_id)),
			JigsawParameterBoolean::make(false)
		));

		break;
	}
	case 6: // empower (tribe)
	{
		enums::TribeDef::Tribe tribe = static_cast<enums::TribeDef::Tribe>(fh->read_uint8());
		Ref<CardFilter> tribe_filter = CardFilter::make_tribe(tribe);
		if (tribe == enums::TribeDef::LEGACY_CUSTOM) {
			tribe_filter->set_meta(meta_legacy_tribe, fh->read_string1());
		}
		Ref<JigsawParameterAmount> amount = read_amount_v0_v1(fh);

		effect->set_effect(enums::EffectDef::EMPOWER);
		effect->set_priority(80);
		effect->set_params(Array::make(
			JigsawParameterStat::make(enums::StatDef::ATK),
			amount,
			JigsawParameterCardFilter::make(tribe_filter),
			JigsawParameterBoolean::make(false)
		));

		break;
	}
	case 7: // heal
	{
		Ref<JigsawParameterAmount> amount = read_amount_v0_v1(fh);

		effect->set_effect(enums::EffectDef::HEAL);
		effect->set_priority(40);
		effect->set_params(Array::make(
			amount,
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(false)
		));

		break;
	}
	case 8: // lifesteal
	{
		Ref<JigsawParameterAmount> amount = read_amount_v0_v1(fh);

		Ref<EffectInstance> heal;
		heal.instantiate();
		heal->set_effect(enums::EffectDef::HEAL);
		heal->set_priority(40);
		heal->set_params(Array::make(
			amount,
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(false)
		));

		effect->set_effect(enums::EffectDef::COND_WINNER);
		effect->set_priority(225);
		effect->set_params(Array::make(
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(false),
			JigsawParameterEffectInstance::make(heal)
		));

		break;
	}
	case 9: // numb
	{
		Ref<JigsawParameterAmount> amount = read_amount_v0_v1(fh);

		effect->set_effect(enums::EffectDef::NUMB);
		effect->set_priority(90);
		effect->set_params(Array::make(
			amount,
			JigsawParameterCardFilter::make(CardFilter::make_rank(enums::RankDef::ATTACKER)),
			JigsawParameterEffectInstance::make(Ref<EffectInstance>()),
			JigsawParameterEffectInstance::make(Ref<EffectInstance>()),
			JigsawParameterBoolean::make(true),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(false)
		));

		break;
	}
	case 10: // pierce
	{
		Ref<JigsawParameterAmount> amount = read_amount_v0_v1(fh);

		effect->set_effect(enums::EffectDef::RAW_STAT);
		effect->set_priority(95);
		effect->set_params(Array::make(
			JigsawParameterStat::make(enums::StatDef::DEF),
			amount,
			JigsawParameterBoolean::make(true)
		));
		
		break;
	}
	case 11: // summon (card)
	{
		enums::CardDef::Card card_id = static_cast<enums::CardDef::Card>(fh->read_uint8());

		effect->set_effect(enums::EffectDef::SUMMON);
		effect->set_priority(25);
		effect->set_params(Array::make(
			JigsawParameterAmount::make(1),
			JigsawParameterCardFilter::make(CardFilter::make_card(card_id)),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(false)
		));

		break;
	}
	case 12: // summon (rank)
	{
		enums::RankDef::Rank rank = format_version == 0 ? enums::RankDef::MINI_BOSS : static_cast<enums::RankDef::Rank>(fh->read_uint8());

		effect->set_effect(enums::EffectDef::SUMMON);
		effect->set_priority(20);
		effect->set_params(Array::make(
			JigsawParameterAmount::make(1),
			JigsawParameterCardFilter::make(CardFilter::make_rank(rank)),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(true),
			JigsawParameterBoolean::make(false)
		));

		break;
	}
	case 13: // summon (tribe)
	{
		uint8_t rank_tribe = fh->read_uint8();
		enums::RankDef::Rank rank = static_cast<enums::RankDef::Rank>((rank_tribe >> 4) - 1);
		if (rank == -1) {
			rank = enums::RankDef::LEGACY_ENEMY;
		}
		Ref<CardFilter> rank_filter = rank == enums::RankDef::LEGACY_NONE ? Ref<CardFilter>() : CardFilter::make_rank(rank);

		enums::TribeDef::Tribe tribe = static_cast<enums::TribeDef::Tribe>(rank_tribe & 15);
		Ref<CardFilter> tribe_filter = CardFilter::make_tribe(tribe);
		if (tribe == enums::TribeDef::LEGACY_CUSTOM) {
			tribe_filter->set_meta(meta_legacy_tribe, fh->read_string1());
		}
		int64_t count = fh->read_uint8();

		effect->set_effect(enums::EffectDef::SUMMON);
		effect->set_priority(25);
		effect->set_params(Array::make(
			JigsawParameterAmount::make(count),
			JigsawParameterCardFilter::make(CardFilter::make_and(Array::make(tribe_filter, rank_filter))),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(false)
		));

		break;
	}
	case 14: // unity
	{
		enums::TribeDef::Tribe tribe = static_cast<enums::TribeDef::Tribe>(fh->read_uint8());
		Ref<CardFilter> tribe_filter = CardFilter::make_tribe(tribe);
		if (tribe == enums::TribeDef::LEGACY_CUSTOM) {
			tribe_filter->set_meta(meta_legacy_tribe, fh->read_string1());
		}
		Ref<JigsawParameterAmount> amount = read_amount_v0_v1(fh);

		Ref<EffectInstance> empower;
		empower.instantiate();
		empower->set_effect(enums::EffectDef::EMPOWER);
		empower->set_priority(80);
		empower->set_params(Array::make(
			JigsawParameterStat::make(enums::StatDef::ATK),
			amount,
			JigsawParameterCardFilter::make(tribe_filter),
			JigsawParameterBoolean::make(false)
		));

		effect->set_effect(enums::EffectDef::COND_LIMIT);
		effect->set_priority(10);
		effect->set_params(Array::make(
			JigsawParameterAmount::make(1),
			JigsawParameterEffectInstance::make(empower),
			JigsawParameterBoolean::make(false)
		));

		break;
	}
	case 15: // TP
	{
		Ref<JigsawParameterAmount> amount = read_amount_v0_v1(fh);

		effect->set_effect(enums::EffectDef::TP);
		effect->set_priority(0);
		effect->set_params(Array::make(amount));

		break;
	}
	case 16: // summon as opponent
	{
		enums::CardDef::Card card_id = static_cast<enums::CardDef::Card>(fh->read_uint8());

		effect->set_effect(enums::EffectDef::SUMMON);
		effect->set_priority(25);
		effect->set_params(Array::make(
			JigsawParameterAmount::make(1),
			JigsawParameterCardFilter::make(CardFilter::make_card(card_id)),
			JigsawParameterBoolean::make(true),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(false)
		));

		break;
	}
	case 17: // multiply healing
	{
		int64_t amount = fh->read_sint8() + 1;

		effect->set_effect(enums::EffectDef::MULTIPLY_HEALING);
		effect->set_priority(40);
		effect->set_params(Array::make(
			JigsawParameterAmount::make(amount),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(true),
			JigsawParameterBoolean::make(true),
			JigsawParameterBoolean::make(true)
		));

		break;
	}
	case 18: // flavor text
	{
		uint8_t flags = fh->read_uint8();
		bool hide_remaining = (flags & 1) != 0;
		String text = fh->read_string1();

		Color color;
		color.set_r8(0x22);
		color.set_g8(0x66);
		color.set_b8(0x88);

		effect->set_effect(enums::EffectDef::FLAVOR_TEXT);
		effect->set_priority(0);
		effect->set_params(Array::make(
			JigsawParameterString::make(text),
			JigsawParameterColor::make(color),
			JigsawParameterBoolean::make(hide_remaining)
		));

		break;
	}
	case 128: // coin
	{
		bool negate = false;
		if (format_version == 1) {
			uint8_t flags = fh->read_uint8();
			negate = (flags & 1) != 0;
		}
		uint8_t count = fh->read_uint8();
		bool has_tails = (count & 0x80) != 0;
		count = (count & 0x7f) + 1;
		Ref<EffectInstance> heads_result, tails_result;
		heads_result.instantiate();
		ERR_FAIL_COND_V(!effect_v0_v1(container, heads_result, fh, format_version), false);
		if (has_tails) {
			tails_result.instantiate();
			ERR_FAIL_COND_V(!effect_v0_v1(container, tails_result, fh, format_version), false);
		}

		effect->set_effect(enums::EffectDef::COND_COIN);
		effect->set_priority(30);
		if (negate) {
			effect->set_params(Array::make(
				JigsawParameterAmount::make(count),
				JigsawParameterAmount::make(1),
				JigsawParameterAmount::make(1),
				JigsawParameterEffectInstance::make(heads_result),
				JigsawParameterEffectInstance::make(tails_result),
				JigsawParameterIcon::make(enums::IconDef::COIN_SAD),
				JigsawParameterIcon::make(enums::IconDef::COIN_HAPPY),
				SOUND_ATK_FAIL->duplicate(true),
				SOUND_ATK_SUCCESS->duplicate(true)
			));
		} else if (has_tails && heads_result->get_effect() == enums::EffectDef::STAT &&
				Object::cast_to<JigsawParameterStat>(heads_result->get_params()[0])->get_stat() == enums::StatDef::ATK &&
				tails_result->get_effect() == enums::EffectDef::STAT &&
				Object::cast_to<JigsawParameterStat>(tails_result->get_params()[0])->get_stat() == enums::StatDef::DEF) {
			effect->set_params(Array::make(
				JigsawParameterAmount::make(count),
				JigsawParameterAmount::make(1),
				JigsawParameterAmount::make(1),
				JigsawParameterEffectInstance::make(heads_result),
				JigsawParameterEffectInstance::make(tails_result),
				JigsawParameterIcon::make(enums::IconDef::STAT_ATK),
				JigsawParameterIcon::make(enums::IconDef::STAT_DEF),
				SOUND_NULL->duplicate(true),
				SOUND_NULL->duplicate(true)
			));
		} else {
			effect->set_params(Array::make(
				JigsawParameterAmount::make(count),
				JigsawParameterAmount::make(1),
				JigsawParameterAmount::make(1),
				JigsawParameterEffectInstance::make(heads_result),
				JigsawParameterEffectInstance::make(tails_result),
				JigsawParameterIcon::make(enums::IconDef::COIN_HAPPY),
				JigsawParameterIcon::make(enums::IconDef::COIN_SAD),
				SOUND_ATK_SUCCESS->duplicate(true),
				SOUND_ATK_FAIL->duplicate(true)
			));
		}

		break;
	}
	case 129: // if card
	{
		bool compare_less = false;
		if (format_version == 1) {
			uint8_t flags = fh->read_uint8();
			compare_less = (flags & 1) != 0;
		}

		enums::CardDef::Card card_id = static_cast<enums::CardDef::Card>(fh->read_uint8());
		int64_t count = 1;
		if (format_version == 1) {
			count = fh->read_uint8();
		}
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v0_v1(container, result, fh, format_version), false);

		effect->set_effect(enums::EffectDef::COND_CARD);
		effect->set_priority(35);
		effect->set_params(Array::make(
			JigsawParameterAmount::make(count),
			JigsawParameterCardFilter::make(CardFilter::make_card(card_id)),
			JigsawParameterEffectInstance::make(result),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(compare_less),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(false),
			JigsawParameterLocation::make(enums::LocationDef::FIELD)
		));

		break;
	}
	case 130: // per card
	{
		if (format_version == 1) {
			(void)fh->read_uint8(); // unused flags field
		}

		enums::CardDef::Card card_id = static_cast<enums::CardDef::Card>(fh->read_uint8());
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v0_v1(container, result, fh, format_version), false);

		effect->set_effect(enums::EffectDef::COND_CARD);
		effect->set_priority(75);
		effect->set_params(Array::make(
			JigsawParameterAmount::make(1),
			JigsawParameterCardFilter::make(CardFilter::make_card(card_id)),
			JigsawParameterEffectInstance::make(result),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(true),
			JigsawParameterBoolean::make(false),
			JigsawParameterLocation::make(enums::LocationDef::FIELD)
		));

		break;
	}
	case 131: // if tribe
	{
		bool compare_less = false;
		if (format_version == 1) {
			uint8_t flags = fh->read_uint8();
			compare_less = (flags & 1) != 0;
		}
		enums::TribeDef::Tribe tribe = static_cast<enums::TribeDef::Tribe>(fh->read_uint8());
		Ref<CardFilter> tribe_filter = CardFilter::make_tribe(tribe);
		if (tribe == enums::TribeDef::LEGACY_CUSTOM) {
			tribe_filter->set_meta(meta_legacy_tribe, fh->read_string1());
		}
		int64_t count = 1;
		if (format_version == 1) {
			count = fh->read_uint8();
		}
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v0_v1(container, result, fh, format_version), false);

		effect->set_effect(enums::EffectDef::COND_CARD);
		effect->set_priority(35);
		effect->set_params(Array::make(
			JigsawParameterAmount::make(count),
			JigsawParameterCardFilter::make(tribe_filter),
			JigsawParameterEffectInstance::make(result),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(compare_less),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(false),
			JigsawParameterLocation::make(enums::LocationDef::FIELD)
		));

		break;
	}
	case 132: // VS tribe
	{
		bool compare_less = false;
		if (format_version == 1) {
			uint8_t flags = fh->read_uint8();
			compare_less = (flags & 1) != 0;
		}
		enums::TribeDef::Tribe tribe = static_cast<enums::TribeDef::Tribe>(fh->read_uint8());
		Ref<CardFilter> tribe_filter = CardFilter::make_tribe(tribe);
		if (tribe == enums::TribeDef::LEGACY_CUSTOM) {
			tribe_filter->set_meta(meta_legacy_tribe, fh->read_string1());
		}
		int64_t count = 1;
		if (format_version == 1) {
			count = fh->read_uint8();
		}
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v0_v1(container, result, fh, format_version), false);

		effect->set_effect(enums::EffectDef::COND_CARD);
		effect->set_priority(70);
		effect->set_params(Array::make(
			JigsawParameterAmount::make(count),
			JigsawParameterCardFilter::make(tribe_filter),
			JigsawParameterEffectInstance::make(result),
			JigsawParameterBoolean::make(true),
			JigsawParameterBoolean::make(compare_less),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(false),
			JigsawParameterLocation::make(enums::LocationDef::FIELD)
		));

		break;
	}
	case 133: // if stat
	{
		bool compare_less = false;
		if (format_version == 1) {
			uint8_t flags = fh->read_uint8();
			compare_less = (flags & 1) != 0;
		}
		int64_t amount = fh->read_uint8();
		enums::StatDef::Stat stat = enums::StatDef::ATK;
		if ((amount & 0x80) != 0) {
			amount &= 0x7f;
			stat = enums::StatDef::DEF;
		}
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v0_v1(container, result, fh, format_version), false);

		effect->set_effect(enums::EffectDef::COND_STAT);
		effect->set_priority(85);
		effect->set_params(Array::make(
			JigsawParameterAmount::make(amount),
			JigsawParameterEffectInstance::make(result),
			JigsawParameterBoolean::make(compare_less),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(false),
			JigsawParameterStat::make(stat)
		));

		break;
	}
	case 134: // setup
	{
		bool show_original_text = false;
		if (format_version == 1) {
			uint8_t flags = fh->read_uint8();
			show_original_text = (flags & 1) != 0;
		}
		bool show_effect_only = !show_original_text;
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v0_v1(container, result, fh, format_version), false);

		effect->set_effect(enums::EffectDef::COND_APPLY);
		effect->set_priority(235);
		effect->set_params(Array::make(
			JigsawParameterEffectInstance::make(result),
			JigsawParameterBoolean::make(true),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(show_effect_only),
			JigsawParameterBoolean::make(show_original_text)
		));

		break;
	}
	case 135: // limit
	{
		bool greater_than = false;
		if (format_version == 1) {
			uint8_t flags = fh->read_uint8();
			greater_than = (flags & 1) != 0;
		}
		int64_t amount = fh->read_uint8();
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v0_v1(container, result, fh, format_version), false);

		effect->set_effect(enums::EffectDef::COND_LIMIT);
		effect->set_priority(10);
		effect->set_params(Array::make(
			JigsawParameterAmount::make(amount),
			JigsawParameterEffectInstance::make(result),
			JigsawParameterBoolean::make(greater_than)
		));

		break;
	}
	case 136: // VS card
	{
		bool compare_less = false;
		if (format_version == 1) {
			uint8_t flags = fh->read_uint8();
			compare_less = (flags & 1) != 0;
		}
		enums::CardDef::Card card_id = static_cast<enums::CardDef::Card>(fh->read_uint8());
		int64_t count = fh->read_uint8();
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v0_v1(container, result, fh, format_version), false);

		effect->set_effect(enums::EffectDef::COND_CARD);
		effect->set_priority(70);
		effect->set_params(Array::make(
			JigsawParameterAmount::make(count),
			JigsawParameterCardFilter::make(CardFilter::make_card(card_id)),
			JigsawParameterEffectInstance::make(result),
			JigsawParameterBoolean::make(true),
			JigsawParameterBoolean::make(compare_less),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(false),
			JigsawParameterLocation::make(enums::LocationDef::FIELD)
		));

		break;
	}
	case 137: // if rank
	{
		bool compare_less = false;
		if (format_version == 1) {
			uint8_t flags = fh->read_uint8();
			compare_less = (flags & 1) != 0;
		}
		enums::RankDef::Rank rank = static_cast<enums::RankDef::Rank>(fh->read_uint8());
		int64_t count = fh->read_uint8();
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v0_v1(container, result, fh, format_version), false);

		effect->set_effect(enums::EffectDef::COND_CARD);
		effect->set_priority(35);
		effect->set_params(Array::make(
			JigsawParameterAmount::make(count),
			JigsawParameterCardFilter::make(CardFilter::make_rank(rank)),
			JigsawParameterEffectInstance::make(result),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(compare_less),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(false),
			JigsawParameterLocation::make(enums::LocationDef::FIELD)
		));

		break;
	}
	case 138: // VS rank
	{
		bool compare_less = false;
		if (format_version == 1) {
			uint8_t flags = fh->read_uint8();
			compare_less = (flags & 1) != 0;
		}
		enums::RankDef::Rank rank = static_cast<enums::RankDef::Rank>(fh->read_uint8());
		int64_t count = fh->read_uint8();
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v0_v1(container, result, fh, format_version), false);

		effect->set_effect(enums::EffectDef::COND_CARD);
		effect->set_priority(70);
		effect->set_params(Array::make(
			JigsawParameterAmount::make(count),
			JigsawParameterCardFilter::make(CardFilter::make_rank(rank)),
			JigsawParameterEffectInstance::make(result),
			JigsawParameterBoolean::make(true),
			JigsawParameterBoolean::make(compare_less),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(false),
			JigsawParameterLocation::make(enums::LocationDef::FIELD)
		));

		break;
	}
	case 139: // if win
	{
		bool loser = false;
		if (format_version == 1) {
			uint8_t flags = fh->read_uint8();
			loser = (flags & 1) != 0;
		}
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v0_v1(container, result, fh, format_version), false);

		effect->set_effect(enums::EffectDef::COND_WINNER);
		effect->set_priority(225);
		effect->set_params(Array::make(
			JigsawParameterEffectInstance::make(result),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(loser)
		));

		break;
	}
	case 140: // if tie
	{
		bool negate = false;
		if (format_version == 1) {
			uint8_t flags = fh->read_uint8();
			negate = (flags & 1) != 0;
		}
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v0_v1(container, result, fh, format_version), false);

		effect->set_effect(enums::EffectDef::COND_TIE);
		effect->set_priority(225);
		effect->set_params(Array::make(
			JigsawParameterEffectInstance::make(result),
			JigsawParameterBoolean::make(negate)
		));

		break;
	}
	case 141: // if stat (after modifiers)
	{
		bool compare_less = false;
		if (format_version == 1) {
			uint8_t flags = fh->read_uint8();
			compare_less = (flags & 1) != 0;
		}
		int64_t amount = fh->read_uint8();
		enums::StatDef::Stat stat = enums::StatDef::ATK;
		if ((amount & 0x80) != 0) {
			amount &= 0x7f;
			stat = enums::StatDef::DEF;
		}
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v0_v1(container, result, fh, format_version), false);

		effect->set_effect(enums::EffectDef::COND_STAT);
		effect->set_priority(100);
		effect->set_params(Array::make(
			JigsawParameterAmount::make(amount),
			JigsawParameterEffectInstance::make(result),
			JigsawParameterBoolean::make(compare_less),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(false),
			JigsawParameterStat::make(stat)
		));

		break;
	}
	case 142: // VS stat
	{
		bool compare_less = false;
		if (format_version == 1) {
			uint8_t flags = fh->read_uint8();
			compare_less = (flags & 1) != 0;
		}
		int64_t amount = fh->read_uint8();
		enums::StatDef::Stat stat = enums::StatDef::ATK;
		if ((amount & 0x80) != 0) {
			amount &= 0x7f;
			stat = enums::StatDef::DEF;
		}
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v0_v1(container, result, fh, format_version), false);

		effect->set_effect(enums::EffectDef::COND_STAT);
		effect->set_priority(85);
		effect->set_params(Array::make(
			JigsawParameterAmount::make(amount),
			JigsawParameterEffectInstance::make(result),
			JigsawParameterBoolean::make(compare_less),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(true),
			JigsawParameterStat::make(stat)
		));

		break;
	}
	case 143: // VS stat (after modifiers)
	{
		bool compare_less = false;
		if (format_version == 1) {
			uint8_t flags = fh->read_uint8();
			compare_less = (flags & 1) != 0;
		}
		int64_t amount = fh->read_uint8();
		enums::StatDef::Stat stat = enums::StatDef::ATK;
		if ((amount & 0x80) != 0) {
			amount &= 0x7f;
			stat = enums::StatDef::DEF;
		}
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v0_v1(container, result, fh, format_version), false);

		effect->set_effect(enums::EffectDef::COND_STAT);
		effect->set_priority(100);
		effect->set_params(Array::make(
			JigsawParameterAmount::make(amount),
			JigsawParameterEffectInstance::make(result),
			JigsawParameterBoolean::make(compare_less),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(true),
			JigsawParameterStat::make(stat)
		));

		break;
	}
	case 144: // if HP
	{
		bool compare_less = false;
		if (format_version == 1) {
			uint8_t flags = fh->read_uint8();
			compare_less = (flags & 1) != 0;
		}
		int64_t amount = fh->read_uint8();
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v0_v1(container, result, fh, format_version), false);

		effect->set_effect(enums::EffectDef::COND_STAT);
		effect->set_priority(15);
		effect->set_params(Array::make(
			JigsawParameterAmount::make(amount),
			JigsawParameterEffectInstance::make(result),
			JigsawParameterBoolean::make(compare_less),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(false),
			JigsawParameterStat::make(enums::StatDef::HP)
		));

		break;
	}
	case 145: // per tribe
	{
		if (format_version == 1) {
			(void)fh->read_uint8(); // unused flags field
		}

		enums::TribeDef::Tribe tribe = static_cast<enums::TribeDef::Tribe>(fh->read_uint8());
		Ref<CardFilter> tribe_filter = CardFilter::make_tribe(tribe);
		if (tribe == enums::TribeDef::LEGACY_CUSTOM) {
			tribe_filter->set_meta(meta_legacy_tribe, fh->read_string1());
		}
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v0_v1(container, result, fh, format_version), false);

		effect->set_effect(enums::EffectDef::COND_CARD);
		effect->set_priority(75);
		effect->set_params(Array::make(
			JigsawParameterAmount::make(1),
			JigsawParameterCardFilter::make(tribe_filter),
			JigsawParameterEffectInstance::make(result),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(true),
			JigsawParameterBoolean::make(false),
			JigsawParameterLocation::make(enums::LocationDef::FIELD)
		));

		break;
	}
	case 146: // per rank
	{
		if (format_version == 1) {
			(void)fh->read_uint8(); // unused flags field
		}

		enums::RankDef::Rank rank = static_cast<enums::RankDef::Rank>(fh->read_uint8());
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v0_v1(container, result, fh, format_version), false);

		effect->set_effect(enums::EffectDef::COND_CARD);
		effect->set_priority(75);
		effect->set_params(Array::make(
			JigsawParameterAmount::make(1),
			JigsawParameterCardFilter::make(CardFilter::make_rank(rank)),
			JigsawParameterEffectInstance::make(result),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(true),
			JigsawParameterBoolean::make(false),
			JigsawParameterLocation::make(enums::LocationDef::FIELD)
		));

		break;
	}
	case 147: // VS HP
	{
		bool compare_less = false;
		if (format_version == 1) {
			uint8_t flags = fh->read_uint8();
			compare_less = (flags & 1) != 0;
		}
		int64_t amount = fh->read_uint8();
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v0_v1(container, result, fh, format_version), false);

		effect->set_effect(enums::EffectDef::COND_STAT);
		effect->set_priority(15);
		effect->set_params(Array::make(
			JigsawParameterAmount::make(amount),
			JigsawParameterEffectInstance::make(result),
			JigsawParameterBoolean::make(compare_less),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(true),
			JigsawParameterStat::make(enums::StatDef::HP)
		));

		break;
	}
	default:
		ERR_FAIL_V_MSG(false, vformat("unhandled v%d effect type %d", format_version, type));
		break;
	}

	return true;
}

Ref<JigsawParameterAmount> LegacyParse::read_amount_v2_v4(const Ref<FormatHelper> &fh, bool negate) {
	int64_t amount = fh->read_uint8();
	int64_t infinity = 0;

	if (amount == 255) {
		amount = 0;
		infinity = 1;
	}

	if (negate) {
		amount = -amount;
		infinity = -infinity;
	}

	return JigsawParameterAmount::make(amount, infinity);
}

Ref<CardFilter> LegacyParse::read_card_filter_v2_v4(const Ref<FormatHelper> &fh, bool generic) {
	if (!generic) {
		enums::CardDef::Card card_id = static_cast<enums::CardDef::Card>(fh->read_uvarint());

		return CardFilter::make_card(card_id);
	}

	uint8_t rank_tribe = fh->read_uint8();

	enums::RankDef::Rank rank = static_cast<enums::RankDef::Rank>(rank_tribe >> 4);
	enums::TribeDef::Tribe tribe = static_cast<enums::TribeDef::Tribe>(rank_tribe & 15);

	Ref<CardFilter> rank_filter = rank != enums::RankDef::LEGACY_NONE ? CardFilter::make_rank(rank) : Ref<CardFilter>();
	Ref<CardFilter> tribe_filter = tribe != enums::TribeDef::LEGACY_NONE ? CardFilter::make_tribe(tribe) : Ref<CardFilter>();

	if (tribe == enums::TribeDef::LEGACY_CUSTOM) {
		tribe_filter->set_meta(meta_legacy_tribe, fh->read_stringvar());
	}

	return CardFilter::make_and(Array::make(rank_filter, tribe_filter));
}

bool LegacyParse::effect_v2_v4(const Ref<DataContainer> &container, const Ref<EffectInstance> &effect, const Ref<FormatHelper> &fh, int64_t format_version) {
	uint8_t type = fh->read_uint8();
	uint8_t flags = fh->read_uint8();
	bool negate = (flags & 0x01) != 0;
	bool opponent = (flags & 0x02) != 0;
	bool each = (flags & 0x04) != 0;
	bool late = (flags & 0x08) != 0;
	bool generic = (flags & 0x10) != 0;
	bool defense = (flags & 0x20) != 0;

	switch (type) {
	case 0: // flavor text
	{
		String text = fh->read_stringvar();
		Color color;
		color.set_r8(0x22);
		color.set_g8(0x66);
		color.set_b8(0x88);
		bool hide_remaining = negate;

		effect->set_effect(enums::EffectDef::FLAVOR_TEXT);
		effect->set_priority(0);
		effect->set_params(Array::make(
			JigsawParameterString::make(text),
			JigsawParameterColor::make(color),
			JigsawParameterBoolean::make(hide_remaining)
		));

		break;
	}
	case 1: // stat
	{
		enums::StatDef::Stat stat = defense ? enums::StatDef::DEF : enums::StatDef::ATK;
		Ref<JigsawParameterAmount> amount = read_amount_v2_v4(fh, negate);

		if (opponent) {
			effect->set_priority(95);
			effect->set_effect(enums::EffectDef::RAW_STAT);
			effect->set_params(Array::make(
				JigsawParameterStat::make(stat),
				amount,
				JigsawParameterBoolean::make(opponent)
			));
		} else {
			effect->set_priority(65);
			effect->set_effect(enums::EffectDef::STAT);
			effect->set_params(Array::make(
				JigsawParameterStat::make(stat),
				amount
			));
		}

		break;
	}
	case 2: // empower
	{
		enums::StatDef::Stat stat = defense ? enums::StatDef::DEF : enums::StatDef::ATK;
		Ref<JigsawParameterAmount> amount = read_amount_v2_v4(fh, negate);
		Ref<CardFilter> filter = read_card_filter_v2_v4(fh, generic);

		effect->set_effect(enums::EffectDef::EMPOWER);
		effect->set_priority(80);
		effect->set_params(Array::make(
			JigsawParameterStat::make(stat),
			amount,
			JigsawParameterCardFilter::make(filter),
			JigsawParameterBoolean::make(opponent)
		));

		break;
	}
	case 3: // summon
	{
		int64_t count = int64_t(fh->read_uint8()) + 1;
		Ref<CardFilter> filter = read_card_filter_v2_v4(fh, generic);
		bool hide_summoned_card = defense;
		bool hide_summoner = negate;

		effect->set_effect(enums::EffectDef::SUMMON);
		effect->set_priority(hide_summoner ? 20 : 25);
		effect->set_params(Array::make(
			JigsawParameterAmount::make(count),
			JigsawParameterCardFilter::make(filter),
			JigsawParameterBoolean::make(opponent),
			JigsawParameterBoolean::make(hide_summoner),
			JigsawParameterBoolean::make(hide_summoned_card)
		));

		break;
	}
	case 4: // heal
	{
		Ref<JigsawParameterAmount> amount = read_amount_v2_v4(fh, negate);

		if (each) {
			if (amount->get_amount_inf() != 0) {
				amount = JigsawParameterAmount::make(amount->get_amount_inf() * 255);
			}

			effect->set_effect(enums::EffectDef::MULTIPLY_HEALING);
			effect->set_priority(40);
			effect->set_params(Array::make(
				amount,
				JigsawParameterBoolean::make(opponent),
				JigsawParameterBoolean::make(true),
				JigsawParameterBoolean::make(generic || defense),
				JigsawParameterBoolean::make(generic || !defense)
			));
		} else {
			effect->set_effect(enums::EffectDef::HEAL);
			effect->set_priority(40);
			effect->set_params(Array::make(
				amount,
				JigsawParameterBoolean::make(opponent),
				JigsawParameterBoolean::make(false)
			));
		}

		break;
	}
	case 5: // TP
	{
		Ref<JigsawParameterAmount> amount = read_amount_v2_v4(fh, negate);

		effect->set_effect(enums::EffectDef::TP);
		effect->set_priority(0);
		effect->set_params(Array::make(
			amount
		));

		break;
	}
	case 6: // numb
	{
		Ref<JigsawParameterAmount> count = read_amount_v2_v4(fh, false);
		bool numb_self = false;
		Ref<CardFilter> filter;
		if (format_version == 4) {
			numb_self = !opponent;
			filter = read_card_filter_v2_v4(fh, generic);
		} else {
			filter = CardFilter::make_rank(enums::RankDef::ATTACKER);
		}

		effect->set_effect(enums::EffectDef::NUMB);
		effect->set_priority(90);
		effect->set_params(Array::make(
			count,
			JigsawParameterCardFilter::make(filter),
			JigsawParameterEffectInstance::make(Ref<EffectInstance>()),
			JigsawParameterEffectInstance::make(Ref<EffectInstance>()),
			JigsawParameterBoolean::make(!numb_self),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(true),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(false)
		));

		break;
	}
	case 128: // card (condition)
	{
		int64_t count = 1;
		if (!each) {
			count += fh->read_uint8();
		}
		Ref<CardFilter> filter = read_card_filter_v2_v4(fh, generic);
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v2_v4(container, result, fh, format_version), false);

		effect->set_effect(enums::EffectDef::COND_CARD);
		effect->set_priority(each ? 75 : (opponent ? 70 : 35));
		effect->set_params(Array::make(
			JigsawParameterAmount::make(count),
			JigsawParameterCardFilter::make(filter),
			JigsawParameterEffectInstance::make(result),
			JigsawParameterBoolean::make(opponent),
			JigsawParameterBoolean::make(negate),
			JigsawParameterBoolean::make(each),
			JigsawParameterBoolean::make(false),
			JigsawParameterLocation::make(enums::LocationDef::FIELD)
		));

		break;
	}
	case 129: // limit (condition)
	{
		int64_t count = int64_t(fh->read_uint8()) + 1;
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v2_v4(container, result, fh, format_version), false);

		effect->set_effect(enums::EffectDef::COND_LIMIT);
		effect->set_priority(10);
		effect->set_params(Array::make(
			JigsawParameterAmount::make(count),
			JigsawParameterEffectInstance::make(result),
			JigsawParameterBoolean::make(negate)
		));

		break;
	}
	case 130: // winner (condition)
	{
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v2_v4(container, result, fh, format_version), false);

		if (negate) {
			effect->set_effect(enums::EffectDef::COND_TIE);
			effect->set_priority(225);
			effect->set_params(Array::make(
				JigsawParameterEffectInstance::make(result),
				JigsawParameterBoolean::make(opponent)
			));
		} else {
			effect->set_effect(enums::EffectDef::COND_WINNER);
			effect->set_priority(225);
			effect->set_params(Array::make(
				JigsawParameterEffectInstance::make(result),
				JigsawParameterBoolean::make(false),
				JigsawParameterBoolean::make(opponent)
			));
		}

		break;
	}
	case 131: // apply (condition)
	{
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v2_v4(container, result, fh, format_version), false);

		if (negate && !opponent && late &&
				result->get_effect() == enums::EffectDef::SUMMON &&
				!Ref<JigsawParameterBoolean>(result->get_params()[2])->get_boolean() &&
				Ref<JigsawParameterBoolean>(result->get_params()[4])->get_boolean() &&
				Ref<JigsawParameterAmount>(result->get_params()[0])->get_amount() == 1 &&
				Ref<JigsawParameterAmount>(result->get_params()[0])->get_amount_inf() == 0 &&
				Ref<JigsawParameterCardFilter>(result->get_params()[1])->get_filter().is_valid() &&
				Ref<JigsawParameterCardFilter>(result->get_params()[1])->get_filter()->get_type() == CardFilter::CARD) {
			result->set_priority(0);
		}

		effect->set_effect(enums::EffectDef::COND_APPLY);
		effect->set_priority(late ? 235 : 5);
		effect->set_params(Array::make(
			JigsawParameterEffectInstance::make(result),
			JigsawParameterBoolean::make(late),
			JigsawParameterBoolean::make(opponent),
			JigsawParameterBoolean::make(!negate),
			JigsawParameterBoolean::make(negate)
		));

		break;
	}
	case 132: // coin (condition)
	{
		int64_t count = int64_t(fh->read_uint8()) + 1;
		enums::IconDef::Icon heads_icon = enums::IconDef::COIN_HAPPY;
		enums::IconDef::Icon tails_icon = enums::IconDef::COIN_SAD;
		Ref<JigsawParameterFileIDOpus> heads_sound = SOUND_ATK_SUCCESS;
		Ref<JigsawParameterFileIDOpus> tails_sound = SOUND_ATK_FAIL;
		if (defense) {
			heads_icon = enums::IconDef::STAT_ATK;
			tails_icon = enums::IconDef::STAT_DEF;
			heads_sound = SOUND_NULL;
			tails_sound = SOUND_NULL;
		}
		if (negate) {
			SWAP(heads_icon, tails_icon);
			SWAP(heads_sound, tails_sound);
		}

		Ref<EffectInstance> heads_result, tails_result;
		heads_result.instantiate();
		ERR_FAIL_COND_V(!effect_v2_v4(container, heads_result, fh, format_version), false);
		if (generic) {
			tails_result.instantiate();
			ERR_FAIL_COND_V(!effect_v2_v4(container, tails_result, fh, format_version), false);
		}

		effect->set_effect(enums::EffectDef::COND_COIN);
		effect->set_priority(30);
		effect->set_params(Array::make(
			JigsawParameterAmount::make(count),
			JigsawParameterAmount::make(1),
			JigsawParameterAmount::make(1),
			JigsawParameterEffectInstance::make(heads_result),
			JigsawParameterEffectInstance::make(tails_result),
			JigsawParameterIcon::make(heads_icon),
			JigsawParameterIcon::make(tails_icon),
			heads_sound,
			tails_sound
		));

		break;
	}
	case 133: // HP (condition)
	{
		int64_t count = int64_t(fh->read_uint8()) + 1;
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v2_v4(container, result, fh, format_version), false);

		effect->set_effect(enums::EffectDef::COND_STAT);
		effect->set_priority(late ? 230 : 15);
		effect->set_params(Array::make(
			JigsawParameterAmount::make(count),
			JigsawParameterEffectInstance::make(result),
			JigsawParameterBoolean::make(negate),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(opponent),
			JigsawParameterStat::make(enums::StatDef::HP)
		));

		break;
	}
	case 134: // stat (condition)
	{
		int64_t count = int64_t(fh->read_uint8()) + 1;
		enums::StatDef::Stat stat = defense ? enums::StatDef::DEF : enums::StatDef::ATK;
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v2_v4(container, result, fh, format_version), false);

		effect->set_effect(enums::EffectDef::COND_STAT);
		effect->set_priority(late ? 100 : 85);
		effect->set_params(Array::make(
			JigsawParameterAmount::make(count),
			JigsawParameterEffectInstance::make(result),
			JigsawParameterBoolean::make(negate),
			JigsawParameterBoolean::make(false),
			JigsawParameterBoolean::make(opponent),
			JigsawParameterStat::make(stat)
		));

		break;
	}
	case 135: // priority (condition)
	{
		uint8_t priority_effect = fh->read_uint8();
		ERR_FAIL_COND_V(!effect_v2_v4(container, effect, fh, format_version), false);

		switch (priority_effect) {
		case 0: // flavor text
			effect->set_priority(0);
			break;
		case 1: // stat
			effect->set_priority(opponent ? 95 : 65);
			break;
		case 2: // empower
			effect->set_priority(80);
			break;
		case 3: // summon
			effect->set_priority(negate ? 20 : 25);
			break;
		case 4: // heal
			effect->set_priority(40);
			break;
		case 5: // TP
			effect->set_priority(0);
			break;
		case 6: // numb
			effect->set_priority(90);
			break;
		case 128: // card (condition)
			if (opponent) {
				effect->set_priority(70);
			} else if (each) {
				effect->set_priority(75);
			} else {
				effect->set_priority(35);
			}
			break;
		case 129: // limit (condition)
			effect->set_priority(10);
			break;
		case 130: // winner (condition)
			effect->set_priority(225);
			break;
		case 131: // apply (condition)
			effect->set_priority(late ? 235 : 5);
			break;
		case 132: // coin (condition)
			effect->set_priority(30);
			break;
		case 133: // HP (condition)
			effect->set_priority(late ? 230 : 15);
			break;
		case 134: // stat (condition)
			effect->set_priority(late ? 100 : 85);
			break;
		case 136: // on numb (condition)
			effect->set_priority(0);
			break;
		}

		break;
	}
	case 136: // on numb (condition)
	{
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v2_v4(container, result, fh, format_version), false);

		if (result->get_effect() == enums::EffectDef::STAT) {
			result->set_effect(enums::EffectDef::RAW_STAT);
			TypedArray<JigsawParameter> params = result->get_params();
			params.append(JigsawParameterBoolean::make(false));
			result->set_params(params);
		}

		effect->set_effect(enums::EffectDef::COND_ON_NUMB);
		effect->set_priority(0);
		effect->set_params(Array::make(
			JigsawParameterEffectInstance::make(result)
		));

		break;
	}
	default:
		ERR_FAIL_V_MSG(false, vformat("unhandled v%d effect type %d", format_version, type));
		break;
	}

	return true;
}

Ref<JigsawParameterAmount> LegacyParse::read_amount_v5(const Ref<FormatHelper> &fh, bool infinite) {
	int64_t amount = fh->read_svarint();
	int64_t infinity = 0;

	if (infinite) {
		infinity = amount;
		amount = 0;
	}

	return JigsawParameterAmount::make(amount, infinity);
}

Ref<CardFilter> LegacyParse::read_card_filter_v5(const Ref<FormatHelper> &fh) {
	bool first = true;
	uint8_t tag = fh->read_uint8();
	TypedArray<CardFilter> tribe_filters;
	TypedArray<CardFilter> negated_tribe_filters;
	TypedArray<CardFilter> rank_filters;
	TypedArray<CardFilter> tp_filters;
	for (;;) {
		switch (tag >> 4) {
		case 0b0000:
			switch (tag & 15) {
			case 0b0000:
			{
				Ref<CardFilter> all_tribes = CardFilter::make_and(tribe_filters + negated_tribe_filters);
				Ref<CardFilter> any_rank = CardFilter::make_or(rank_filters);
				Ref<CardFilter> any_tp = CardFilter::make_or(tp_filters);

				return CardFilter::make_and(Array::make(all_tribes, any_rank, any_tp));
			}
			case 0b0001:
			{
				ERR_FAIL_COND_V_MSG(!first, Ref<CardFilter>(), vformat("unhandled v5 card filter tag %d", tag));

				enums::CardDef::Card card_id = static_cast<enums::CardDef::Card>(fh->read_uvarint());

				return CardFilter::make_card(card_id);
			}
			default:
			{
				ERR_FAIL_V_MSG(Ref<CardFilter>(), vformat("unhandled v5 card filter tag %d", tag));
				break;
			}
			}
			break;
		case 0b0001:
		{
			enums::TribeDef::Tribe tribe = static_cast<enums::TribeDef::Tribe>(tag & 15);
			ERR_FAIL_COND_V(tribe == enums::TribeDef::LEGACY_NONE, Ref<CardFilter>());
			Ref<CardFilter> tribe_filter = CardFilter::make_tribe(tribe);
			if (tribe == enums::TribeDef::LEGACY_CUSTOM) {
				tribe_filter->set_meta(meta_legacy_tribe, fh->read_stringvar());
			}
			tribe_filters.append(tribe_filter);
			break;
		}
		case 0b0010:
		{
			enums::TribeDef::Tribe tribe = static_cast<enums::TribeDef::Tribe>(tag & 15);
			ERR_FAIL_COND_V(tribe == enums::TribeDef::LEGACY_NONE, Ref<CardFilter>());
			Ref<CardFilter> tribe_filter = CardFilter::make_tribe(tribe);
			tribe_filter->set_negate(true);
			if (tribe == enums::TribeDef::LEGACY_CUSTOM) {
				tribe_filter->set_meta(meta_legacy_tribe, fh->read_stringvar());
			}
			negated_tribe_filters.append(tribe_filter);
			break;
		}
		case 0b0011:
		{
			enums::RankDef::Rank rank = static_cast<enums::RankDef::Rank>(tag & 15);
			ERR_FAIL_COND_V(rank == enums::RankDef::LEGACY_NONE || rank == enums::RankDef::LEGACY_ENEMY, Ref<CardFilter>());
			rank_filters.append(CardFilter::make_rank(rank));
			break;
		}
		case 0b0100:
			switch (tag & 15) {
			case 0b0000:
			{
				int64_t tp = fh->read_svarint();
				tp_filters.append(CardFilter::make_cost(enums::StatDef::TP, tp));
				break;
			}
			case 0b0001:
			{
				ERR_FAIL_COND_V_MSG(!first, Ref<CardFilter>(), vformat("unhandled v5 card filter tag %d", tag));

				return CardFilter::make_param(JigsawParameterVariable::make(enums::VariableDef::TARGET_CARD));
			}
			default:
			{
				ERR_FAIL_V_MSG(Ref<CardFilter>(), vformat("unhandled v5 card filter tag %d", tag));
				break;
			}
			}
			break;
		default:
		{
			ERR_FAIL_V_MSG(Ref<CardFilter>(), vformat("unhandled v5 card filter tag %d", tag));
			break;
		}
		}

		first = false;
		tag = fh->read_uint8();
	}
}

bool LegacyParse::effect_v5(const Ref<DataContainer> &container, const Ref<EffectInstance> &effect, const Ref<FormatHelper> &fh, int64_t format_version) {
	effect->set_effect(static_cast<enums::EffectDef::Effect>(fh->read_uint8()));
	effect->set_priority(fh->read_uint8());
	uint64_t flags = fh->read_uvarint();

	switch (effect->get_effect()) {
	case enums::EffectDef::FLAVOR_TEXT:
	{
		Color color;
		if ((flags & 0x02) != 0) {
			color = fh->read_color_rgb888();
		} else {
			color.set_r8(0x22);
			color.set_r8(0x66);
			color.set_r8(0x88);
		}
		String text = fh->read_stringvar();
		bool hide_remaining = (flags & 0x01) != 0;

		effect->set_params(Array::make(
			JigsawParameterString::make(text),
			JigsawParameterColor::make(color),
			JigsawParameterBoolean::make(hide_remaining)
		));

		break;
	}
	case enums::EffectDef::STAT:
	{
		enums::StatDef::Stat stat = (flags & 0x01) != 0 ? enums::StatDef::DEF : enums::StatDef::ATK;
		Ref<JigsawParameterAmount> amount = read_amount_v5(fh, (flags & 0x02) != 0);

		effect->set_params(Array::make(
			JigsawParameterStat::make(stat),
			amount
		));

		break;
	}
	case enums::EffectDef::EMPOWER:
	{
		enums::StatDef::Stat stat = (flags & 0x01) != 0 ? enums::StatDef::DEF : enums::StatDef::ATK;
		Ref<JigsawParameterAmount> amount = read_amount_v5(fh, (flags & 0x02) != 0);
		Ref<CardFilter> filter = read_card_filter_v5(fh);
		bool opponent = (flags & 0x04) != 0;

		effect->set_params(Array::make(
			JigsawParameterStat::make(stat),
			amount,
			JigsawParameterCardFilter::make(filter),
			JigsawParameterBoolean::make(opponent)
		));

		break;
	}
	case enums::EffectDef::SUMMON:
	{
		int64_t amount = fh->read_uvarint();
		Ref<CardFilter> filter = read_card_filter_v5(fh);
		bool opponent = (flags & 0x01) != 0;
		bool hide_summoner = (flags & 0x02) != 0;
		bool hide_summoned_card = (flags & 0x04) != 0;

		effect->set_params(Array::make(
			JigsawParameterAmount::make(amount),
			JigsawParameterCardFilter::make(filter),
			JigsawParameterBoolean::make(opponent),
			JigsawParameterBoolean::make(hide_summoner),
			JigsawParameterBoolean::make(hide_summoned_card)
		));

		break;
	}
	case enums::EffectDef::HEAL:
	{
		Ref<JigsawParameterAmount> amount = read_amount_v5(fh, (flags & 0x01) != 0);
		bool opponent = (flags & 0x02) != 0;
		bool ignore_multipliers = (flags & 0x04) != 0;

		effect->set_params(Array::make(
			amount,
			JigsawParameterBoolean::make(opponent),
			JigsawParameterBoolean::make(ignore_multipliers)
		));

		break;
	}
	case enums::EffectDef::TP:
	{
		Ref<JigsawParameterAmount> amount = read_amount_v5(fh, (flags & 0x01) != 0);

		effect->set_params(Array::make(
			amount
		));

		break;
	}
	case enums::EffectDef::NUMB:
	{
		int64_t count = fh->read_uvarint();
		int64_t infinity = 0;
		if ((flags & 0x01) != 0) {
			infinity = count;
			count = 0;
		}
		Ref<CardFilter> filter = read_card_filter_v5(fh);
		bool opponent = (flags & 0x02) == 0;
		bool descending_order = (flags & 0x04) != 0;
		bool ignore_atk = (flags & 0x08) != 0;
		bool ignore_def = (flags & 0x10) != 0;
		bool sort_zero_last = (flags & 0x40) != 0;
		Ref<EffectInstance> success_result, failure_result;
		if ((flags & 0x40) != 0) {
			success_result.instantiate();
			ERR_FAIL_COND_V(!effect_v5(container, success_result, fh, format_version), false);
		}
		if ((flags & 0x80) != 0) {
			failure_result.instantiate();
			ERR_FAIL_COND_V(!effect_v5(container, failure_result, fh, format_version), false);
		}
		bool summon_to_opposite_field = (flags & 0x100) != 0;
		bool ignore_prevent_numb = (flags & 0x200) != 0;

		effect->set_params(Array::make(
			JigsawParameterAmount::make(count, infinity),
			JigsawParameterCardFilter::make(filter),
			JigsawParameterEffectInstance::make(success_result),
			JigsawParameterEffectInstance::make(failure_result),
			JigsawParameterBoolean::make(opponent),
			JigsawParameterBoolean::make(descending_order),
			JigsawParameterBoolean::make(ignore_atk),
			JigsawParameterBoolean::make(ignore_def),
			JigsawParameterBoolean::make(sort_zero_last),
			JigsawParameterBoolean::make(summon_to_opposite_field),
			JigsawParameterBoolean::make(ignore_prevent_numb)
		));

		break;
	}
	case enums::EffectDef::RAW_STAT:
	{
		enums::StatDef::Stat stat = (flags & 0x01) != 0 ? enums::StatDef::DEF : enums::StatDef::ATK;
		Ref<JigsawParameterAmount> amount = read_amount_v5(fh, (flags & 0x02) != 0);
		bool opponent = (flags & 0x04) != 0;

		effect->set_params(Array::make(
			JigsawParameterStat::make(stat),
			amount,
			JigsawParameterBoolean::make(opponent)
		));

		break;
	}
	case enums::EffectDef::MULTIPLY_HEALING:
	{
		int64_t amount = fh->read_svarint();
		bool opponent = (flags & 0x01) != 0;
		bool retroactive = (flags & 0x02) == 0;
		bool multiply_positive = (flags & 0x0C) != 0x04;
		bool multiply_negative = (flags & 0x0C) != 0x08;

		effect->set_params(Array::make(
			JigsawParameterAmount::make(amount),
			JigsawParameterBoolean::make(opponent),
			JigsawParameterBoolean::make(retroactive),
			JigsawParameterBoolean::make(multiply_positive),
			JigsawParameterBoolean::make(multiply_negative)
		));

		break;
	}
	case enums::EffectDef::PREVENT_NUMB:
	{
		int64_t count = fh->read_uvarint();
		Ref<CardFilter> filter = read_card_filter_v5(fh);
		bool opponent = (flags & 0x01) != 0;

		effect->set_params(Array::make(
			JigsawParameterAmount::make(count),
			JigsawParameterCardFilter::make(filter),
			JigsawParameterBoolean::make(opponent)
		));

		break;
	}
	case enums::EffectDef::MODIFY_AVAILABLE_CARDS_ADD:
	{
		int64_t count = fh->read_uvarint();
		bool opponent = (flags & 0x01) != 0;
		TypedArray<JigsawParameter> replace_card;
		switch (flags & 0x06) {
		case 0x00:
			switch (flags & 0x18) {
			case 0x00:
				// MODIFY_AVAILABLE_CARDS_ADD
				break;
			case 0x08:
				effect->set_effect(enums::EffectDef::MODIFY_AVAILABLE_CARDS_ADD_HAND);
				break;
			case 0x10:
				effect->set_effect(enums::EffectDef::MODIFY_AVAILABLE_CARDS_ADD_DECK);
				break;
			case 0x18:
				effect->set_effect(enums::EffectDef::MODIFY_AVAILABLE_CARDS_ADD_FIELD);
				break;
			}
			break;
		case 0x02:
			switch (flags & 0x18) {
			case 0x00:
				effect->set_effect(enums::EffectDef::MODIFY_AVAILABLE_CARDS_REMOVE);
				break;
			case 0x08:
				effect->set_effect(enums::EffectDef::MODIFY_AVAILABLE_CARDS_REMOVE_HAND);
				break;
			case 0x10:
				effect->set_effect(enums::EffectDef::MODIFY_AVAILABLE_CARDS_REMOVE_DECK);
				break;
			case 0x18:
				effect->set_effect(enums::EffectDef::MODIFY_AVAILABLE_CARDS_REMOVE_FIELD);
				break;
			}
			break;
		case 0x04:
			switch (flags & 0x18) {
			case 0x00:
				effect->set_effect(enums::EffectDef::MODIFY_AVAILABLE_CARDS_MOVE);
				break;
			case 0x08:
				effect->set_effect(enums::EffectDef::MODIFY_AVAILABLE_CARDS_MOVE_HAND);
				break;
			case 0x10:
				effect->set_effect(enums::EffectDef::MODIFY_AVAILABLE_CARDS_MOVE_DECK);
				break;
			case 0x18:
				ERR_FAIL_V_MSG(false, "invalid flags for modify available cards");
				break;
			}
			break;
		case 0x06:
			switch (flags & 0x18) {
			case 0x00:
				effect->set_effect(enums::EffectDef::MODIFY_AVAILABLE_CARDS_REPLACE);
				break;
			case 0x08:
				effect->set_effect(enums::EffectDef::MODIFY_AVAILABLE_CARDS_REPLACE_HAND);
				break;
			case 0x10:
				effect->set_effect(enums::EffectDef::MODIFY_AVAILABLE_CARDS_REPLACE_DECK);
				break;
			case 0x18:
				effect->set_effect(enums::EffectDef::MODIFY_AVAILABLE_CARDS_REPLACE_FIELD);
				break;
			}
			replace_card.append(JigsawParameterCard::make(static_cast<enums::CardDef::Card>(fh->read_uvarint())));
			break;
		}

		Ref<CardFilter> filter = read_card_filter_v5(fh);

		bool require_self = (flags & 0x60) == 0x20;
		bool require_opponent = (flags & 0x60) == 0x40;
		Ref<EffectInstance> success_result, failure_result;
		if ((flags & 0x80) != 0) {
			success_result.instantiate();
			ERR_FAIL_COND_V(!effect_v5(container, success_result, fh, format_version), false);
		}
		if ((flags & 0x100) != 0) {
			failure_result.instantiate();
			ERR_FAIL_COND_V(!effect_v5(container, failure_result, fh, format_version), false);
		}

		Ref<JigsawParameterLocation> exile_group = JigsawParameterLocation::make(enums::LocationDef::FIRST_CUSTOM);
		exile_group->set_meta(meta_legacy_exile, (flags & 0x200) != 0 ? fh->read_stringvar() : "");

		effect->set_params(Array::make(
			JigsawParameterAmount::make(count),
			JigsawParameterCardFilter::make(filter)
		) + replace_card + Array::make(
			JigsawParameterEffectInstance::make(success_result),
			JigsawParameterEffectInstance::make(failure_result),
			JigsawParameterBoolean::make(opponent),
			JigsawParameterBoolean::make(require_self),
			JigsawParameterBoolean::make(require_opponent),
			exile_group
		));

		break;
	}
	case enums::EffectDef::MODIFY_CARD_COST:
	{
		Ref<JigsawParameterAmount> amount = read_amount_v5(fh, (flags & 0x01) != 0);
		Ref<CardFilter> filter = read_card_filter_v5(fh);
		bool relative = (flags & 0x02) == 0;

		effect->set_params(Array::make(
			amount,
			JigsawParameterCardFilter::make(filter),
			JigsawParameterBoolean::make(relative)
		));

		break;
	}
	case enums::EffectDef::DRAW_CARD:
	{
		int64_t amount = fh->read_svarint();
		bool opponent = (flags & 0x01) != 0;
		bool ignore_max_hand_size = (flags & 0x02) != 0;
		Ref<CardFilter> filter;
		if ((flags & 0x04) != 0) {
			filter = read_card_filter_v5(fh);
		}
		Ref<EffectInstance> success_result, failure_result;
		if ((flags & 0x8) != 0) {
			success_result.instantiate();
			ERR_FAIL_COND_V(!effect_v5(container, success_result, fh, format_version), false);
		}
		if ((flags & 0x10) != 0) {
			failure_result.instantiate();
			ERR_FAIL_COND_V(!effect_v5(container, failure_result, fh, format_version), false);
		}

		effect->set_params(Array::make(
			JigsawParameterAmount::make(amount),
			JigsawParameterCardFilter::make(filter),
			JigsawParameterEffectInstance::make(success_result),
			JigsawParameterEffectInstance::make(failure_result),
			JigsawParameterBoolean::make(opponent),
			JigsawParameterBoolean::make(ignore_max_hand_size)
		));

		break;
	}
	case enums::EffectDef::MODIFY_GAME_RULE:
	{
		enums::VariableDef::Variable rule = static_cast<enums::VariableDef::Variable>(fh->read_uvarint() + enums::VariableDef::FIRST_LEGACY_GAME_RULE);
		int64_t amount = fh->read_svarint();
		bool relative = (flags & 0x01) != 0;
		bool affect_self = (flags & 0x06) != 0x04;
		bool affect_opponent = (flags & 0x06) != 0x02;

		effect->set_params(Array::make(
			JigsawParameterVariable::make(rule),
			JigsawParameterAmount::make(amount),
			JigsawParameterBoolean::make(relative),
			JigsawParameterBoolean::make(affect_self),
			JigsawParameterBoolean::make(affect_opponent)
		));

		break;
	}
	case enums::EffectDef::DELAY_SETUP:
	{
		int64_t amount = fh->read_svarint();
		Ref<CardFilter> filter = read_card_filter_v5(fh);
		bool opponent = (flags & 0x01) != 0;
		bool execute_on_zero = (flags & 0x02) == 0;

		effect->set_params(Array::make(
			JigsawParameterAmount::make(amount),
			JigsawParameterCardFilter::make(filter),
			JigsawParameterBoolean::make(opponent),
			JigsawParameterBoolean::make(execute_on_zero)
		));

		break;
	}
	case enums::EffectDef::COND_CARD:
	{
		int64_t count = fh->read_uvarint();
		Ref<CardFilter> filter = read_card_filter_v5(fh);
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v5(container, result, fh, format_version), false);
		bool opponent = (flags & 0x01) != 0;
		bool compare_less = (flags & 0x02) != 0;
		bool compare_each = (flags & 0x04) != 0;
		bool allow_numb = (flags & 0x08) != 0;
		enums::LocationDef::Location location;
		switch (flags & 0x30) {
		case 0x00:
			location = enums::LocationDef::FIELD;
			break;
		case 0x10:
			location = enums::LocationDef::HAND;
			break;
		case 0x20:
			location = enums::LocationDef::DECK;
			break;
		default:
			ERR_FAIL_V_MSG(false, "invalid location flags in COND_CARD");
			break;
		}

		effect->set_params(Array::make(
			JigsawParameterAmount::make(count),
			JigsawParameterCardFilter::make(filter),
			JigsawParameterEffectInstance::make(result),
			JigsawParameterBoolean::make(opponent),
			JigsawParameterBoolean::make(compare_less),
			JigsawParameterBoolean::make(compare_each),
			JigsawParameterBoolean::make(allow_numb),
			JigsawParameterLocation::make(location)
		));

		break;
	}
	case enums::EffectDef::COND_LIMIT:
	{
		int64_t amount = fh->read_uvarint();
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v5(container, result, fh, format_version), false);
		bool greater_than = (flags & 0x01) != 0;

		effect->set_params(Array::make(
			JigsawParameterAmount::make(amount),
			JigsawParameterEffectInstance::make(result),
			JigsawParameterBoolean::make(greater_than)
		));

		break;
	}
	case enums::EffectDef::COND_WINNER:
	{
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v5(container, result, fh, format_version), false);
		bool negate = (flags & 0x07) >= 0x03;
		bool loser = (flags & 0x07) == 0x01 || (flags & 0x07) == 0x05;
		bool tie = (flags & 0x07) == 0x02 || (flags & 0x07) == 0x03;

		if (tie) {
			effect->set_effect(enums::EffectDef::COND_TIE);
			effect->set_params(Array::make(
				JigsawParameterEffectInstance::make(result),
				JigsawParameterBoolean::make(negate)
			));
		} else {
			effect->set_params(Array::make(
				JigsawParameterEffectInstance::make(result),
				JigsawParameterBoolean::make(negate),
				JigsawParameterBoolean::make(loser)
			));
		}

		break;
	}
	case enums::EffectDef::COND_APPLY:
	{
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v5(container, result, fh, format_version), false);
		bool next_round = (flags & 0x01) != 0;
		bool opponent = (flags & 0x02) != 0;
		bool show_effect_only = (flags & 0x0c) == 0x00;
		bool show_original_text = (flags & 0x0c) == 0x04;

		effect->set_params(Array::make(
			JigsawParameterEffectInstance::make(result),
			JigsawParameterBoolean::make(next_round),
			JigsawParameterBoolean::make(opponent),
			JigsawParameterBoolean::make(show_effect_only),
			JigsawParameterBoolean::make(show_original_text)
		));

		break;
	}
	case enums::EffectDef::COND_COIN:
	{
		int64_t count = fh->read_uvarint();
		int64_t heads_weight = 1;
		int64_t tails_weight = 1;

		if ((flags & 0x08) != 0) {
			heads_weight = fh->read_uvarint() + 1;
			tails_weight = fh->read_uvarint();
		}

		Ref<EffectInstance> heads_result, tails_result;
		heads_result.instantiate();
		ERR_FAIL_COND_V(!effect_v5(container, heads_result, fh, format_version), false);
		if ((flags & 0x01) != 0) {
			tails_result.instantiate();
			ERR_FAIL_COND_V(!effect_v5(container, tails_result, fh, format_version), false);
		}

		if ((flags & 0x04) != 0) {
			if ((flags & 0x02) != 0) {
				effect->set_params(Array::make(
					JigsawParameterAmount::make(count),
					JigsawParameterAmount::make(heads_weight),
					JigsawParameterAmount::make(tails_weight),
					JigsawParameterEffectInstance::make(heads_result),
					JigsawParameterEffectInstance::make(tails_result),
					JigsawParameterIcon::make(enums::IconDef::STAT_DEF),
					JigsawParameterIcon::make(enums::IconDef::STAT_ATK),
					SOUND_NULL->duplicate(true),
					SOUND_NULL->duplicate(true)
				));
			} else {
				effect->set_params(Array::make(
					JigsawParameterAmount::make(count),
					JigsawParameterAmount::make(heads_weight),
					JigsawParameterAmount::make(tails_weight),
					JigsawParameterEffectInstance::make(heads_result),
					JigsawParameterEffectInstance::make(tails_result),
					JigsawParameterIcon::make(enums::IconDef::STAT_ATK),
					JigsawParameterIcon::make(enums::IconDef::STAT_DEF),
					SOUND_NULL->duplicate(true),
					SOUND_NULL->duplicate(true)
				));
			}
		} else {
			if ((flags & 0x02) != 0) {
				effect->set_params(Array::make(
					JigsawParameterAmount::make(count),
					JigsawParameterAmount::make(heads_weight),
					JigsawParameterAmount::make(tails_weight),
					JigsawParameterEffectInstance::make(heads_result),
					JigsawParameterEffectInstance::make(tails_result),
					JigsawParameterIcon::make(enums::IconDef::COIN_SAD),
					JigsawParameterIcon::make(enums::IconDef::COIN_HAPPY),
					SOUND_ATK_FAIL->duplicate(true),
					SOUND_ATK_SUCCESS->duplicate(true)
				));
			} else {
				effect->set_params(Array::make(
					JigsawParameterAmount::make(count),
					JigsawParameterAmount::make(heads_weight),
					JigsawParameterAmount::make(tails_weight),
					JigsawParameterEffectInstance::make(heads_result),
					JigsawParameterEffectInstance::make(tails_result),
					JigsawParameterIcon::make(enums::IconDef::COIN_HAPPY),
					JigsawParameterIcon::make(enums::IconDef::COIN_SAD),
					SOUND_ATK_SUCCESS->duplicate(true),
					SOUND_ATK_FAIL->duplicate(true)
				));
			}
		}
		break;
	}
	case enums::EffectDef::COND_STAT:
	{
		int64_t amount = fh->read_svarint();
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v5(container, result, fh, format_version), false);
		bool opponent = (flags & 0x01) != 0;
		bool less_than = (flags & 0x02) != 0;
		enums::StatDef::Stat base_stat = static_cast<enums::StatDef::Stat>((flags >> 2) & 0x03);
		bool multiple = (flags & 0x10) != 0;
		bool math_none = (flags & 0x60) == 0x00;
		bool math_subtract = (flags & 0x60) == 0x40;
		bool math_opponent = (flags & 0x80) != 0;
		enums::StatDef::Stat math_stat = static_cast<enums::StatDef::Stat>((flags >> 8) & 0x03);

		if (math_none) {
			effect->set_params(Array::make(
				JigsawParameterAmount::make(amount),
				JigsawParameterEffectInstance::make(result),
				JigsawParameterBoolean::make(less_than),
				JigsawParameterBoolean::make(multiple),
				JigsawParameterBoolean::make(opponent),
				JigsawParameterStat::make(base_stat)
			));
		} else {
			effect->set_effect(enums::EffectDef::COND_STAT_MATH);
			effect->set_params(Array::make(
				JigsawParameterAmount::make(amount),
				JigsawParameterEffectInstance::make(result),
				JigsawParameterBoolean::make(less_than),
				JigsawParameterBoolean::make(multiple),
				JigsawParameterBoolean::make(opponent),
				JigsawParameterStat::make(base_stat),
				JigsawParameterBoolean::make(math_opponent),
				JigsawParameterStat::make(math_stat),
				JigsawParameterBoolean::make(math_subtract)
			));
		}

		break;
	}
	case enums::EffectDef::COND_IN_HAND:
	{
		int64_t min_turns = fh->read_uvarint();
		int64_t turn_count = fh->read_uvarint();
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v5(container, result, fh, format_version), false);
		bool reveal = (flags & 0x01) == 0;
		bool on_play = (flags & 0x06) != 0x00;
		bool on_not_play = (flags & 0x06) != 0x04;

		effect->set_params(Array::make(
			JigsawParameterAmount::make(min_turns),
			JigsawParameterAmount::make(turn_count != 0 ? min_turns + turn_count : 0, turn_count == 0 ? 1 : 0),
			JigsawParameterEffectInstance::make(result),
			JigsawParameterBoolean::make(reveal),
			JigsawParameterBoolean::make(on_play),
			JigsawParameterBoolean::make(on_not_play)
		));

		break;
	}
	case enums::EffectDef::COND_LAST_EFFECT:
	{
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v5(container, result, fh, format_version), false);

		effect->set_params(Array::make(
			JigsawParameterEffectInstance::make(result)
		));

		break;
	}
	case enums::EffectDef::COND_ON_NUMB:
	{
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v5(container, result, fh, format_version), false);

		effect->set_params(Array::make(
			JigsawParameterEffectInstance::make(result)
		));

		break;
	}
	case enums::EffectDef::COND_MULTIPLE_EFFECTS:
	{
		Ref<EffectInstance> result1, result2;
		result1.instantiate();
		ERR_FAIL_COND_V(!effect_v5(container, result1, fh, format_version), false);
		result2.instantiate();
		ERR_FAIL_COND_V(!effect_v5(container, result2, fh, format_version), false);

		effect->set_params(Array::make(
			JigsawParameterEffectInstance::make(result1),
			JigsawParameterEffectInstance::make(result2)
		));

		break;
	}
	case enums::EffectDef::COND_ON_DISCARD:
	{
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v5(container, result, fh, format_version), false);

		effect->set_params(Array::make(
			JigsawParameterEffectInstance::make(result)
		));

		break;
	}
	case enums::EffectDef::COND_COMPARE_TARGET_CARD:
	{
		Ref<CardFilter> filter = read_card_filter_v5(fh);
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v5(container, result, fh, format_version), false);
		bool negate = (flags & 0x01) != 0;

		effect->set_params(Array::make(
			JigsawParameterCardFilter::make(filter),
			JigsawParameterEffectInstance::make(result),
			JigsawParameterBoolean::make(negate)
		));

		break;
	}
	case enums::EffectDef::COND_ON_EXILE:
	{
		Ref<JigsawParameterLocation> location = JigsawParameterLocation::make(enums::LocationDef::NONE);
		if ((flags & 0x01) != 0) {
			location = JigsawParameterLocation::make(enums::LocationDef::FIRST_CUSTOM);
			location->set_meta(meta_legacy_exile, fh->read_stringvar());
		}
		Ref<EffectInstance> result;
		result.instantiate();
		ERR_FAIL_COND_V(!effect_v5(container, result, fh, format_version), false);

		effect->set_params(Array::make(
			location,
			JigsawParameterEffectInstance::make(result)
		));

		break;
	}
	default:
		ERR_FAIL_V_MSG(false, vformat("unhandled v%d effect type %d", format_version, effect->get_effect()));
		break;
	}

	return true;
}

LazyPredefined<JigsawParameterFileIDGLTF> LegacyParse::METAL_ISLAND_AUDITORIUM{ "METAL_ISLAND_AUDITORIUM" };
LazyPredefined<JigsawParameterFileIDGLTF> LegacyParse::GOLDEN_HILLS{ "GOLDEN_HILLS" };
LazyPredefined<JigsawParameterFileIDGLTF> LegacyParse::FORSAKEN_LANDS{ "FORSAKEN_LANDS" };
LazyPredefined<JigsawParameterFileIDGLTF> LegacyParse::UNDERGROUND_TAVERN{ "UNDERGROUND_TAVERN" };
LazyPredefined<JigsawParameterFileIDGLTF> LegacyParse::BROODMOTHER{ "BROODMOTHER" };
LazyPredefined<JigsawParameterFileIDGLTF> LegacyParse::ABANDONED_TENT{ "ABANDONED_TENT" };
LazyPredefined<JigsawParameterFileIDGLTF> LegacyParse::ANT_KINGDOM_MINE{ "ANT_KINGDOM_MINE" };
LazyPredefined<JigsawParameterFileIDGLTF> LegacyParse::ANT_KINGDOM_PLAZA{ "ANT_KINGDOM_PLAZA" };
constexpr static std::initializer_list<uint8_t> LEGACY_STAGE0001_CID0{ 0x01, 0x70, 0x12, 0x20, 0xee, 0xb4, 0x57, 0xb7, 0x33, 0x4b, 0xa0, 0xea, 0x6e, 0x1e, 0xc3, 0x61, 0x8a, 0xae, 0xea, 0x25, 0x3c, 0xa6, 0xf5, 0xdb, 0x79, 0xda, 0xee, 0x01, 0xfd, 0x9e, 0xb8, 0x1b, 0xe2, 0x2e, 0xb2, 0x1f };
constexpr static std::initializer_list<uint8_t> LEGACY_STAGE0002_CID0{ 0x01, 0x70, 0x12, 0x20, 0x57, 0xfb, 0x38, 0x4d, 0x61, 0x01, 0x58, 0x3f, 0x31, 0xd1, 0xc7, 0x94, 0x2c, 0x69, 0xb3, 0x16, 0x85, 0x36, 0x8b, 0x33, 0x41, 0x08, 0xae, 0x21, 0x6e, 0x55, 0xc7, 0x1d, 0x83, 0x45, 0x09, 0x81 };
constexpr static std::initializer_list<uint8_t> LEGACY_STAGE0003_CID0{ 0x01, 0x70, 0x12, 0x20, 0x27, 0x68, 0x54, 0xff, 0x4b, 0x10, 0xf4, 0xbb, 0xb0, 0xea, 0xf0, 0x8d, 0xac, 0x1b, 0x35, 0x16, 0xf1, 0xd2, 0x13, 0xde, 0xa6, 0xa1, 0x42, 0x7b, 0x2c, 0xfd, 0x70, 0x9e, 0x7e, 0xe2, 0xa7, 0xd0 };
constexpr static std::initializer_list<uint8_t> LEGACY_BROODMOTHER_CID0{ 0x01, 0x70, 0x12, 0x20, 0xc0, 0xfe, 0x0d, 0x55, 0x32, 0x99, 0x6a, 0xd9, 0x08, 0x38, 0x65, 0x26, 0xc5, 0x6c, 0x8e, 0x94, 0x39, 0x8f, 0x6b, 0xe8, 0xc0, 0xf6, 0xae, 0x54, 0x62, 0x7c, 0x7e, 0x00, 0xba, 0x84, 0x05, 0x35 };
constexpr static std::initializer_list<uint8_t> LEGACY_BROODMOTHER_CID1{ 0x01, 0x70, 0x12, 0x20, 0x19, 0x4a, 0x41, 0xc7, 0xa4, 0x58, 0x17, 0x1a, 0x43, 0xea, 0x80, 0x58, 0xa7, 0x0c, 0x09, 0x47, 0xb8, 0x63, 0x18, 0xea, 0x0b, 0xe7, 0x1e, 0xc4, 0x06, 0xb1, 0x84, 0x75, 0xc7, 0xd3, 0x7a, 0x30 };
constexpr static std::initializer_list<uint8_t> LEGACY_ABANDONEDTENT_CID0{ 0x01, 0x70, 0x12, 0x20, 0xff, 0xc1, 0x94, 0x32, 0xfd, 0x47, 0x84, 0xa3, 0x22, 0xaa, 0xa3, 0x40, 0xd3, 0x5f, 0xdd, 0x1c, 0x8f, 0xac, 0x66, 0xab, 0x50, 0x09, 0xe3, 0xed, 0x96, 0x4c, 0xf2, 0x4d, 0xdd, 0xd9, 0x06, 0x85 };
constexpr static std::initializer_list<uint8_t> LEGACY_ABANDONEDTENT_CID1{ 0x01, 0x70, 0x12, 0x20, 0x25, 0x36, 0x35, 0x3a, 0x3f, 0x93, 0xed, 0xeb, 0x30, 0x70, 0xda, 0x67, 0x4f, 0xd1, 0x59, 0xf8, 0x0c, 0x1e, 0xd3, 0x55, 0x93, 0x84, 0xb6, 0xc9, 0x66, 0x0e, 0x8a, 0xbe, 0xac, 0x5e, 0x74, 0xbe };
constexpr static std::initializer_list<uint8_t> LEGACY_MINE_CID0{ 0x01, 0x70, 0x12, 0x20, 0x84, 0x04, 0x70, 0x53, 0xf8, 0x48, 0xf4, 0xb8, 0xf8, 0x5e, 0xaa, 0x6f, 0xd9, 0xe7, 0x63, 0xb5, 0x4a, 0xaf, 0xd1, 0x42, 0xac, 0x00, 0x27, 0xae, 0xe5, 0xcc, 0x97, 0xe5, 0x3f, 0x32, 0x6f, 0xe3 };
constexpr static std::initializer_list<uint8_t> LEGACY_BUGARIAMAIN_CID0{ 0x01, 0x70, 0x12, 0x20, 0x02, 0x72, 0x20, 0x2b, 0x09, 0x81, 0x50, 0x51, 0x9b, 0xb6, 0xdf, 0xe5, 0x43, 0xd4, 0xb5, 0x3d, 0x36, 0xea, 0x05, 0x35, 0x55, 0x41, 0x28, 0x5d, 0xfd, 0x35, 0xb0, 0xc7, 0xf7, 0x18, 0x8d, 0x66 };
bool LegacyParse::extension_v5(const Ref<DataContainer> &container, const Ref<EffectInstance> &extension, const Ref<FormatHelper> &outer_fh, int64_t format_version) {
	int64_t type = outer_fh->read_uvarint();
	Ref<FormatHelper> fh = FormatHelper::read("legacy card extension", outer_fh->read_bytesvar());
	uint64_t flags = fh->read_uvarint();
	Ref<JigsawParameterBoolean> replace_custom = JigsawParameterBoolean::make((flags & 1) != 0);

	switch (type) {
	case 0: // set stage by CID
	{
		extension->set_effect(enums::EffectDef::SET_STAGE);
		extension->set_priority(0);

		PackedByteArray cid = fh->read_bytesvar();
		if (cid == LEGACY_STAGE0001_CID0) {
			extension->set_params(Array::make(METAL_ISLAND_AUDITORIUM->duplicate(true), replace_custom));
		} else if (cid == LEGACY_STAGE0002_CID0) {
			extension->set_params(Array::make(GOLDEN_HILLS->duplicate(true), replace_custom));
		} else if (cid == LEGACY_STAGE0003_CID0) {
			extension->set_params(Array::make(FORSAKEN_LANDS->duplicate(true), replace_custom));
		} else if (cid == LEGACY_BROODMOTHER_CID0 || cid == LEGACY_BROODMOTHER_CID1) {
			extension->set_params(Array::make(BROODMOTHER->duplicate(true), replace_custom));
		} else if (cid == LEGACY_ABANDONEDTENT_CID0 || cid == LEGACY_ABANDONEDTENT_CID1) {
			extension->set_params(Array::make(ABANDONED_TENT->duplicate(true), replace_custom));
		} else if (cid == LEGACY_MINE_CID0) {
			extension->set_params(Array::make(ANT_KINGDOM_MINE->duplicate(true), replace_custom));
		} else if (cid == LEGACY_BUGARIAMAIN_CID0) {
			extension->set_params(Array::make(ANT_KINGDOM_PLAZA->duplicate(true), replace_custom));
		} else {
			ERR_FAIL_V_MSG(false, vformat("unknown stage CID %s", Base32::encode_cid(cid)));
		}

		break;
	}
	case 1: // set music by CID
	{
		extension->set_effect(enums::EffectDef::LEGACY_SET_MUSIC);
		extension->set_priority(0);

		PackedByteArray cid = fh->read_bytesvar();
		float loop_start = fh->read_float32();
		float loop_end = fh->read_float32();
		extension->set_params(Array::make(JigsawParameterCIDOpus::make(cid, loop_start, loop_end), replace_custom));
		break;
	}
	case 2: // play sound by CID
	{
		extension->set_effect(enums::EffectDef::LEGACY_PLAY_SOUND);
		extension->set_priority(0);

		PackedByteArray cid = fh->read_bytesvar();
		extension->set_params(Array::make(JigsawParameterCIDOpus::make(cid, 0.0f, 0.0f)));
		break;
	}
	case 3: // set stage by name
	{
		extension->set_effect(enums::EffectDef::SET_STAGE);
		extension->set_priority(0);

		String name = fh->read_stringvar();
		if (name == "stage0001") {
			extension->set_params(Array::make(METAL_ISLAND_AUDITORIUM->duplicate(true), replace_custom));
		} else if (name == "stage0002") {
			extension->set_params(Array::make(GOLDEN_HILLS->duplicate(true), replace_custom));
		} else if (name == "stage0003") {
			extension->set_params(Array::make(FORSAKEN_LANDS->duplicate(true), replace_custom));
		} else if (name == "stage0004") {
			extension->set_params(Array::make(UNDERGROUND_TAVERN->duplicate(true), replace_custom));
		} else if (name == "broodmother") {
			extension->set_params(Array::make(BROODMOTHER->duplicate(true), replace_custom));
		} else if (name == "abandoned-tent") {
			extension->set_params(Array::make(ABANDONED_TENT->duplicate(true), replace_custom));
		} else if (name == "mine") {
			extension->set_params(Array::make(ANT_KINGDOM_MINE->duplicate(true), replace_custom));
		} else if (name == "bugaria-main") {
			extension->set_params(Array::make(ANT_KINGDOM_PLAZA->duplicate(true), replace_custom));
		} else {
			ERR_FAIL_V_MSG(false, vformat("unknown stage name %s", name));
		}

		break;
	}
	default:
		ERR_FAIL_V_MSG(false, vformat("unhandled legacy extension type %d", type));
		break;
	}

	ERR_FAIL_COND_V(!fh->is_valid_eof(), false);
	return true;
}
