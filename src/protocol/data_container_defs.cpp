#include "protocol/data_container.h"

#include "defs/card_filter_and.h"
#include "defs/card_filter_or.h"
#include "defs/card_filter_card.h"
#include "defs/card_filter_rank.h"
#include "defs/card_filter_tribe.h"
#include "defs/card_filter_cost.h"
#include "defs/card_filter_param.h"

bool DataContainer::_decode_audience_def(const Ref<FormatHelper> &fh, const Ref<AudienceDef> &audience_def) {
	DECODE_VARIANT(audience_def, front);
	DECODE_VARIANT(audience_def, front_cheer);
	DECODE_VARIANT(audience_def, back);
	DECODE_VARIANT(audience_def, back_cheer);
	DECODE_VARIANT(audience_def, weight);
	DECODE_VARIANT(audience_def, pixel_size);

	return fh->is_valid();
}
bool DataContainer::_encode_audience_def(const Ref<FormatHelper> &fh, const Ref<AudienceDef> &audience_def) const {
	ENCODE_VARIANT(audience_def, front);
	ENCODE_VARIANT(audience_def, front_cheer);
	ENCODE_VARIANT(audience_def, back);
	ENCODE_VARIANT(audience_def, back_cheer);
	ENCODE_VARIANT(audience_def, weight);
	ENCODE_VARIANT(audience_def, pixel_size);

	return fh->is_valid();
}

bool DataContainer::_decode_card_def(const Ref<FormatHelper> &fh, const Ref<CardDef> &card_def) {
	DECODE_VARIANT(card_def, id);
	DECODE_VARIANT(card_def, rank);

	TypedArray<StatValue> costs;
	costs.resize(fh->read_uvarint());
	for (int64_t i = 0; i < costs.size(); i++) {
		Ref<StatValue> cost;
		cost.instantiate();
		DECODE_VARIANT(cost, stat);
		ERR_FAIL_COND_V(!_decode_amount(fh, cost), false);
		costs[i] = cost;
	}
	card_def->set_costs(costs);

	DECODE_VARIANT(card_def, name);
	DECODE_VARIANT(card_def, portrait);

	TypedArray<enums::TribeDef::Tribe> tribes;
	tribes.resize(fh->read_uvarint());
	for (int64_t i = 0; i < tribes.size(); i++) {
		tribes[i] = fh->read_id<enums::TribeDef::Tribe>();
	}
	card_def->set_tribes(tribes);

	TypedArray<EffectInstance> effects;
	effects.resize(fh->read_uvarint());
	for (int64_t i = 0; i < effects.size(); i++) {
		Ref<EffectInstance> effect;
		effect.instantiate();
		ERR_FAIL_COND_V(!_decode_effect_instance(fh, effect), false);
		effects[i] = effect;
	}
	card_def->set_effects(effects);

	DECODE_VARIANT(card_def, curve_adjust);

	return fh->is_valid();
}
bool DataContainer::_encode_card_def(const Ref<FormatHelper> &fh, const Ref<CardDef> &card_def) const {
	ENCODE_VARIANT(card_def, id);
	ENCODE_VARIANT(card_def, rank);

	TypedArray<StatValue> costs = card_def->get_costs();
	fh->write_uvarint(costs.size());
	for (int64_t i = 0; i < costs.size(); i++) {
		Ref<StatValue> cost = costs[i];
		ENCODE_VARIANT(cost, stat);
		ERR_FAIL_COND_V(!_encode_amount(fh, cost), false);
	}

	ENCODE_VARIANT(card_def, name);
	ENCODE_VARIANT(card_def, portrait);

	TypedArray<enums::TribeDef::Tribe> tribes = card_def->get_tribes();
	fh->write_uvarint(tribes.size());
	for (int64_t i = 0; i < tribes.size(); i++) {
		int64_t tribe = tribes[i];
		fh->write_id(static_cast<enums::TribeDef::Tribe>(tribe));
	}

	TypedArray<EffectInstance> effects = card_def->get_effects();
	fh->write_uvarint(effects.size());
	for (int64_t i = 0; i < effects.size(); i++) {
		Ref<EffectInstance> effect = effects[i];
		ERR_FAIL_COND_V(!_encode_effect_instance(fh, effect), false);
	}

	ENCODE_VARIANT(card_def, curve_adjust);

	return fh->is_valid();
}

static LazyGlobal<CardDesign> default_card_design{[]() -> Ref<CardDesign> { return memnew(CardDesign); }};
enum : uint64_t {
	CDD_BASE_LAYOUT = 0x00000001,
	CDD_MAIN_ART    = 0x00000002,
	CDD_NAME_DESC   = 0x00000004,
	CDD_COSTS       = 0x00000008,
	CDD_TRIBES      = 0x00000010,
	CDD_SIMPLE_DESC = 0x00000020,
	CDD_TEXT_BORDER = 0x00000040,

	CDD_MASK        = 0x0000007f,
};
static uint64_t card_design_diff(const Ref<CardDesign> &a, const Ref<CardDesign> &b = default_card_design) {
	ERR_FAIL_COND_V(a.is_null(), 0);
	ERR_FAIL_COND_V(b.is_null(), 0);

	uint64_t bits = 0;
	if (Math::is_equal_approx(a->get_pixel_size(), b->get_pixel_size()) ||
		!a->get_base_transform().is_equal_approx(b->get_base_transform()) ||
		!a->get_smooth_corners().is_equal_approx(b->get_smooth_corners()) ||
		!a->get_portrait_transform().is_equal_approx(b->get_portrait_transform()) ||
		!a->get_rank_decoration_transform().is_equal_approx(b->get_rank_decoration_transform())) {
		bits |= CDD_BASE_LAYOUT;
	}
	if (a->get_card_front() != b->get_card_front() ||
		a->get_card_front_window() != b->get_card_front_window() ||
		a->get_tribe_bubble() != b->get_tribe_bubble() ||
		a->get_tribe_bubble_wide() != b->get_tribe_bubble_wide()) {
		bits |= CDD_MAIN_ART;
	}
	if (!a->get_card_name_pos().is_equal_approx(b->get_card_name_pos()) ||
		!a->get_card_name_scale().is_equal_approx(b->get_card_name_scale()) ||
		!a->get_card_description_pos().is_equal_approx(b->get_card_description_pos()) ||
		!a->get_card_description_scale().is_equal_approx(b->get_card_description_scale())) {
		bits |= CDD_NAME_DESC;
	}
	if (!a->get_card_costs_pos().is_equal_approx(b->get_card_costs_pos()) ||
		a->get_card_costs_grow_mode() != b->get_card_costs_grow_mode() ||
		a->get_card_costs_shrink_name() != b->get_card_costs_shrink_name() ||
		!Math::is_equal_approx(a->get_card_costs_spacing(), b->get_card_costs_spacing()) ||
		!a->get_card_costs_text_scale().is_equal_approx(b->get_card_costs_text_scale())) {
		bits |= CDD_COSTS;
	}
	if (!a->get_card_tribes_pos().is_equal_approx(b->get_card_tribes_pos()) ||
		a->get_card_tribes_grow_mode() != b->get_card_tribes_grow_mode() ||
		a->get_card_tribes_shrink_name() != b->get_card_tribes_shrink_name() ||
		!Math::is_equal_approx(a->get_card_tribes_spacing(), b->get_card_tribes_spacing()) ||
		!a->get_card_tribes_text_scale().is_equal_approx(b->get_card_tribes_text_scale())) {
		bits |= CDD_TRIBES;
	}
	if (!a->get_simple_description_pos().is_equal_approx(b->get_simple_description_pos()) ||
		a->get_simple_description_grow_mode() != b->get_simple_description_grow_mode() ||
		a->get_simple_description_spacing() != b->get_simple_description_spacing() ||
		!Math::is_equal_approx(a->get_simple_description_icon_align(), b->get_simple_description_icon_align()) ||
		!Math::is_equal_approx(a->get_simple_description_text_align(), b->get_simple_description_text_align()) ||
		!a->get_simple_description_text_padding().is_equal_approx(b->get_simple_description_text_padding()) ||
		!a->get_simple_description_text_scale().is_equal_approx(b->get_simple_description_text_scale())) {
		bits |= CDD_SIMPLE_DESC;
	}
	if (a->get_effect_highlight_width() != b->get_effect_highlight_width() ||
		!a->get_effect_highlight_color().is_equal_approx(b->get_effect_highlight_color()) ||
		!a->get_description_background_color().is_equal_approx(b->get_description_background_color()) ||
		a->get_simple_description_border_thickness() != b->get_simple_description_border_thickness() ||
		!a->get_simple_description_border_color().is_equal_approx(b->get_simple_description_border_color())) {
		bits |= CDD_TEXT_BORDER;
	}
	return bits;
}

bool DataContainer::_decode_card_design(const Ref<FormatHelper> &fh, const Ref<CardDesign> &card_design) {
	uint64_t diffs = fh->read_uvarint();
	ERR_FAIL_COND_V(diffs != (diffs & CDD_MASK), false);

	if (diffs & CDD_BASE_LAYOUT) {
		DECODE_VARIANT(card_design, pixel_size);
		DECODE_VARIANT(card_design, base_transform);
		DECODE_VARIANT(card_design, smooth_corners);
		DECODE_VARIANT(card_design, portrait_transform);
		DECODE_VARIANT(card_design, rank_decoration_transform);
	}
	if (diffs & CDD_MAIN_ART) {
		DECODE_VARIANT(card_design, card_front);
		DECODE_VARIANT(card_design, card_front_window);
		DECODE_VARIANT(card_design, tribe_bubble);
		DECODE_VARIANT(card_design, tribe_bubble_wide);
	}
	if (diffs & CDD_NAME_DESC) {
		DECODE_VARIANT(card_design, card_name_pos);
		DECODE_VARIANT(card_design, card_name_scale);
		DECODE_VARIANT(card_design, card_description_pos);
		DECODE_VARIANT(card_design, card_description_scale);
	}
	if (diffs & CDD_COSTS) {
		DECODE_VARIANT(card_design, card_costs_pos);
		card_design->set_card_costs_grow_mode(static_cast<CardDesign::GrowMode>(fh->read_uvarint()));
		DECODE_VARIANT(card_design, card_costs_shrink_name);
		DECODE_VARIANT(card_design, card_costs_spacing);
		DECODE_VARIANT(card_design, card_costs_text_scale);
	}
	if (diffs & CDD_TRIBES) {
		DECODE_VARIANT(card_design, card_tribes_pos);
		card_design->set_card_tribes_grow_mode(static_cast<CardDesign::GrowMode>(fh->read_uvarint()));
		DECODE_VARIANT(card_design, card_tribes_shrink_name);
		DECODE_VARIANT(card_design, card_tribes_spacing);
		DECODE_VARIANT(card_design, card_tribes_text_scale);
	}
	if (diffs & CDD_SIMPLE_DESC) {
		DECODE_VARIANT(card_design, simple_description_pos);
		card_design->set_simple_description_grow_mode(static_cast<CardDesign::GrowMode>(fh->read_uvarint()));
		DECODE_VARIANT(card_design, simple_description_spacing);
		DECODE_VARIANT(card_design, simple_description_icon_align);
		DECODE_VARIANT(card_design, simple_description_text_align);
		DECODE_VARIANT(card_design, simple_description_text_padding);
		DECODE_VARIANT(card_design, simple_description_text_scale);
	}
	if (diffs & CDD_TEXT_BORDER) {
		card_design->set_effect_highlight_width(fh->read_uvarint());
		DECODE_VARIANT(card_design, effect_highlight_color);
		DECODE_VARIANT(card_design, description_background_color);
		card_design->set_simple_description_border_thickness(fh->read_uvarint());
		DECODE_VARIANT(card_design, simple_description_border_color);
	}

	static_assert(CDD_MASK == (CDD_BASE_LAYOUT | CDD_MAIN_ART | CDD_NAME_DESC | CDD_COSTS | CDD_TRIBES | CDD_SIMPLE_DESC | CDD_TEXT_BORDER));

	TypedArray<StickerDef> stickers;
	stickers.resize(fh->read_uvarint());

	for (int64_t i = 0; i < stickers.size(); i++) {
		Ref<StickerDef> sticker;
		sticker.instantiate();

		ERR_FAIL_COND_V(!_decode_sticker_def(fh, sticker), false);

		stickers[i] = sticker;
	}

	card_design->set_stickers(stickers);

	return fh->is_valid();
}
bool DataContainer::_encode_card_design(const Ref<FormatHelper> &fh, const Ref<CardDesign> &card_design) const {
	ERR_FAIL_COND_V(card_design.is_null(), false);

	uint64_t diffs = card_design_diff(card_design);
	fh->write_uvarint(diffs);

	if (diffs & CDD_BASE_LAYOUT) {
		ENCODE_VARIANT(card_design, pixel_size);
		ENCODE_VARIANT(card_design, base_transform);
		ENCODE_VARIANT(card_design, smooth_corners);
		ENCODE_VARIANT(card_design, portrait_transform);
		ENCODE_VARIANT(card_design, rank_decoration_transform);
	}
	if (diffs & CDD_MAIN_ART) {
		ENCODE_VARIANT(card_design, card_front);
		ENCODE_VARIANT(card_design, card_front_window);
		ENCODE_VARIANT(card_design, tribe_bubble);
		ENCODE_VARIANT(card_design, tribe_bubble_wide);
	}
	if (diffs & CDD_NAME_DESC) {
		ENCODE_VARIANT(card_design, card_name_pos);
		ENCODE_VARIANT(card_design, card_name_scale);
		ENCODE_VARIANT(card_design, card_description_pos);
		ENCODE_VARIANT(card_design, card_description_scale);
	}
	if (diffs & CDD_COSTS) {
		ENCODE_VARIANT(card_design, card_costs_pos);
		fh->write_uvarint(card_design->get_card_costs_grow_mode());
		ENCODE_VARIANT(card_design, card_costs_shrink_name);
		ENCODE_VARIANT(card_design, card_costs_spacing);
		ENCODE_VARIANT(card_design, card_costs_text_scale);
	}
	if (diffs & CDD_TRIBES) {
		ENCODE_VARIANT(card_design, card_tribes_pos);
		fh->write_uvarint(card_design->get_card_tribes_grow_mode());
		ENCODE_VARIANT(card_design, card_tribes_shrink_name);
		ENCODE_VARIANT(card_design, card_tribes_spacing);
		ENCODE_VARIANT(card_design, card_tribes_text_scale);
	}
	if (diffs & CDD_SIMPLE_DESC) {
		ENCODE_VARIANT(card_design, simple_description_pos);
		fh->write_uvarint(card_design->get_simple_description_grow_mode());
		ENCODE_VARIANT(card_design, simple_description_spacing);
		ENCODE_VARIANT(card_design, simple_description_icon_align);
		ENCODE_VARIANT(card_design, simple_description_text_align);
		ENCODE_VARIANT(card_design, simple_description_text_padding);
		ENCODE_VARIANT(card_design, simple_description_text_scale);
	}
	if (diffs & CDD_TEXT_BORDER) {
		fh->write_uvarint(card_design->get_effect_highlight_width());
		ENCODE_VARIANT(card_design, effect_highlight_color);
		ENCODE_VARIANT(card_design, description_background_color);
		fh->write_uvarint(card_design->get_simple_description_border_thickness());
		ENCODE_VARIANT(card_design, simple_description_border_color);
	}

	static_assert(CDD_MASK == (CDD_BASE_LAYOUT | CDD_MAIN_ART | CDD_NAME_DESC | CDD_COSTS | CDD_TRIBES | CDD_SIMPLE_DESC | CDD_TEXT_BORDER));

	TypedArray<StickerDef> stickers = card_design->get_stickers();
	fh->write_uvarint(stickers.size());

	for (int64_t i = 0; i < stickers.size(); i++) {
		Ref<StickerDef> sticker = stickers[i];
		ERR_FAIL_COND_V(!_encode_sticker_def(fh, sticker), false);
	}

	return fh->is_valid();
}

bool DataContainer::_decode_card_filter(const Ref<FormatHelper> &fh, Ref<CardFilter> &card_filter) {
	CardFilter::Type type = static_cast<CardFilter::Type>(int64_t(fh->read_uvarint()) - 1);
	if (type == CardFilter::NONE) {
		card_filter = Ref<CardFilter>();
		return fh->is_valid();
	}

	bool negate = fh->read_bool();

	switch (type) {
	case CardFilter::NONE:
		ERR_FAIL_V(false); // unreachable
	case CardFilter::AND:
	{
		Ref<CardFilterAnd> filter;
		filter.instantiate();
		filter->set_negate(negate);

		TypedArray<CardFilter> list;
		list.resize(fh->read_uvarint());
		for (int64_t i = 0; i < list.size(); i++) {
			Ref<CardFilter> sub_filter;
			ERR_FAIL_COND_V(!_decode_card_filter(fh,  sub_filter), false);
			ERR_FAIL_COND_V(sub_filter.is_null(), false);
			list[i] = sub_filter;
		}
		filter->set_list(list);

		card_filter = filter;
		return fh->is_valid();
	}
	case CardFilter::OR:
	{
		Ref<CardFilterOr> filter;
		filter.instantiate();
		filter->set_negate(negate);

		TypedArray<CardFilter> list;
		list.resize(fh->read_uvarint());
		for (int64_t i = 0; i < list.size(); i++) {
			Ref<CardFilter> sub_filter;
			ERR_FAIL_COND_V(!_decode_card_filter(fh,  sub_filter), false);
			ERR_FAIL_COND_V(sub_filter.is_null(), false);
			list[i] = sub_filter;
		}
		filter->set_list(list);

		card_filter = filter;
		return fh->is_valid();
	}
	case CardFilter::CARD:
	{
		Ref<CardFilterCard> filter;
		filter.instantiate();
		filter->set_negate(negate);
		DECODE_VARIANT(filter, card);
		card_filter = filter;
		return fh->is_valid();
	}
	case CardFilter::RANK:
	{
		Ref<CardFilterRank> filter;
		filter.instantiate();
		filter->set_negate(negate);
		DECODE_VARIANT(filter, rank);
		card_filter = filter;
		return fh->is_valid();
	}
	case CardFilter::TRIBE:
	{
		Ref<CardFilterTribe> filter;
		filter.instantiate();
		filter->set_negate(negate);
		DECODE_VARIANT(filter, tribe);
		card_filter = filter;
		return fh->is_valid();
	}
	case CardFilter::COST:
	{
		Ref<CardFilterCost> filter;
		filter.instantiate();
		filter->set_negate(negate);
		DECODE_VARIANT(filter, stat);
		ERR_FAIL_COND_V(!_decode_amount(fh, filter), false);
		card_filter = filter;
		return fh->is_valid();
	}
	case CardFilter::PARAM:
	{
		Ref<CardFilterParam> filter;
		filter.instantiate();
		filter->set_negate(negate);
		Ref<JigsawParameter> param;
		ERR_FAIL_COND_V(!_decode_jigsaw_parameter(fh, param), false);
		ERR_FAIL_COND_V(param.is_null(), false);
		ERR_FAIL_COND_V(param->get_type() != JigsawParameter::VARIABLE, false);
		filter->set_param(param);
		card_filter = filter;
		return fh->is_valid();
	}
	}

	ERR_FAIL_V_MSG(false, vformat("unhandled card filter type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("CardFilter", "Type", type)));
}
bool DataContainer::_encode_card_filter(const Ref<FormatHelper> &fh, const Ref<CardFilter> &card_filter) const {
	if (card_filter.is_null()) {
		fh->write_id(CardFilter::NONE);
		return fh->is_valid();
	}

	fh->write_id(card_filter->get_type());
	fh->write_bool(card_filter->get_negate());

	switch (card_filter->get_type()) {
	case CardFilter::NONE:
		ERR_FAIL_V(false); // unreachable
	case CardFilter::AND:
	{
		Ref<CardFilterAnd> filter = card_filter;
		TypedArray<CardFilter> list = filter->get_list();
		fh->write_uvarint(list.size());
		for (int64_t i = 0; i < list.size(); i++) {
			Ref<CardFilter> sub_filter = list[i];
			ERR_FAIL_COND_V(sub_filter.is_null(), false);
			ERR_FAIL_COND_V(!_encode_card_filter(fh,  sub_filter), false);
		}
		return fh->is_valid();
	}
	case CardFilter::OR:
	{
		Ref<CardFilterOr> filter = card_filter;
		TypedArray<CardFilter> list = filter->get_list();
		fh->write_uvarint(list.size());
		for (int64_t i = 0; i < list.size(); i++) {
			Ref<CardFilter> sub_filter = list[i];
			ERR_FAIL_COND_V(sub_filter.is_null(), false);
			ERR_FAIL_COND_V(!_encode_card_filter(fh,  sub_filter), false);
		}
		return fh->is_valid();
	}
	case CardFilter::CARD:
	{
		Ref<CardFilterCard> filter = card_filter;
		ENCODE_VARIANT(filter, card);
		return fh->is_valid();
	}
	case CardFilter::RANK:
	{
		Ref<CardFilterRank> filter = card_filter;
		ENCODE_VARIANT(filter, rank);
		return fh->is_valid();
	}
	case CardFilter::TRIBE:
	{
		Ref<CardFilterTribe> filter = card_filter;
		ENCODE_VARIANT(filter, tribe);
		return fh->is_valid();
	}
	case CardFilter::COST:
	{
		Ref<CardFilterCost> filter = card_filter;
		ENCODE_VARIANT(filter, stat);
		ERR_FAIL_COND_V(!_encode_amount(fh, filter), false);
		return fh->is_valid();
	}
	case CardFilter::PARAM:
	{
		Ref<CardFilterParam> filter = card_filter;
		ERR_FAIL_COND_V(filter->get_param().is_null(), false);
		ERR_FAIL_COND_V(!_encode_jigsaw_parameter(fh, filter->get_param()), false);
		return fh->is_valid();
	}
	}

	ERR_FAIL_V_MSG(false, vformat("unhandled card filter type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("CardFilter", "Type", card_filter->get_type())));
}

enum CharacterDefFlags {
	CDF_HIDDEN = 0x01,
	CDF_PORTRAIT_CROP_TOP = 0x02,
	CDF_PORTRAIT_CROP_FRONT = 0x04,
};
bool DataContainer::_decode_character_def(const Ref<FormatHelper> &fh, const Ref<CharacterDef> &character_def) {
	DECODE_VARIANT(character_def, id);
	DECODE_VARIANT(character_def, display_name);
	DECODE_VARIANT(character_def, tags);

	DECODE_VARIANT(character_def, idle0);
	DECODE_VARIANT(character_def, idle1);
	DECODE_VARIANT(character_def, angry);

	DECODE_VARIANT(character_def, idle0_center_px);
	DECODE_VARIANT(character_def, idle1_center_px);
	DECODE_VARIANT(character_def, angry_center_px);

	DECODE_VARIANT(character_def, pixel_size);
	DECODE_VARIANT(character_def, portrait_offset);
	DECODE_VARIANT(character_def, portrait_scale);

	BitField<CharacterDefFlags> flags = fh->read_uvarint();
	character_def->set_hidden(flags.has_flag(CDF_HIDDEN));
	character_def->set_portrait_crop_top(flags.has_flag(CDF_PORTRAIT_CROP_TOP));
	character_def->set_portrait_crop_front(flags.has_flag(CDF_PORTRAIT_CROP_FRONT));

	return fh->is_valid();
}
bool DataContainer::_encode_character_def(const Ref<FormatHelper> &fh, const Ref<CharacterDef> &character_def) const {
	ENCODE_VARIANT(character_def, id);
	ENCODE_VARIANT(character_def, display_name);
	ENCODE_VARIANT(character_def, tags);

	ENCODE_VARIANT(character_def, idle0);
	ENCODE_VARIANT(character_def, idle1);
	ENCODE_VARIANT(character_def, angry);

	ENCODE_VARIANT(character_def, idle0_center_px);
	ENCODE_VARIANT(character_def, idle1_center_px);
	ENCODE_VARIANT(character_def, angry_center_px);

	ENCODE_VARIANT(character_def, pixel_size);
	ENCODE_VARIANT(character_def, portrait_offset);
	ENCODE_VARIANT(character_def, portrait_scale);

	BitField<CharacterDefFlags> flags = 0;
	if (character_def->is_hidden()) {
		flags.set_flag(CDF_HIDDEN);
	}
	if (character_def->get_portrait_crop_top()) {
		flags.set_flag(CDF_PORTRAIT_CROP_TOP);
	}
	if (character_def->get_portrait_crop_front()) {
		flags.set_flag(CDF_PORTRAIT_CROP_FRONT);
	}
	fh->write_uvarint(flags);

	return fh->is_valid();
}

bool DataContainer::_decode_choices_def(const Ref<FormatHelper> &fh, const Ref<ChoicesDef> &choices_def) {
	DECODE_VARIANT(choices_def, editor_name);
	DECODE_VARIANT(choices_def, editor_choice_names);

	return fh->is_valid();
}
bool DataContainer::_encode_choices_def(const Ref<FormatHelper> &fh, const Ref<ChoicesDef> &choices_def) const {
	ENCODE_VARIANT(choices_def, editor_name);
	ENCODE_VARIANT(choices_def, editor_choice_names);

	return fh->is_valid();
}

bool DataContainer::_decode_effect_def(const Ref<FormatHelper> &fh, const Ref<EffectDef> &effect_def) {
	DECODE_VARIANT(effect_def, editor_name);
	effect_def->set_default_priority(fh->read_svarint());

	PackedStringArray parameter_names;
	TypedArray<JigsawParameter> default_parameters;
	parameter_names.resize(fh->read_uvarint());
	default_parameters.resize(parameter_names.size());
	for (int64_t i = 0; i < default_parameters.size(); i++) {
		parameter_names[i] = fh->read_stringvar();
		Ref<JigsawParameter> param;
		ERR_FAIL_COND_V(!_decode_jigsaw_parameter(fh, param), false);
		ERR_FAIL_COND_V(param.is_null(), false);
		ERR_FAIL_COND_V(!JigsawParameter::is_concrete_type(param->get_type()), false);
		default_parameters[i] = param;
	}
	effect_def->set_default_parameters(default_parameters);
	effect_def->set_parameter_names(parameter_names);

	TypedArray<JigsawTriggerEffect> triggers;
	triggers.resize(fh->read_uvarint());
	for (int64_t i = 0; i < triggers.size(); i++) {
		Ref<JigsawTriggerEffect> trigger;
		ERR_FAIL_COND_V(!_decode_jigsaw_trigger_effect(fh, trigger), false);
		triggers[i] = trigger;
	}
	effect_def->set_triggers(triggers);

	Ref<JigsawProcedureEffectDescribe> describe;
	if (fh->read_bool()) {
		describe.instantiate();
		ERR_FAIL_COND_V(!_decode_jigsaw_procedure(fh, describe), false);
	}
	effect_def->set_describe(describe);

	Ref<JigsawProcedureEffectSimpleDescribe> simple_describe;
	if (fh->read_bool()) {
		simple_describe.instantiate();
		ERR_FAIL_COND_V(!_decode_jigsaw_procedure(fh, simple_describe), false);
	}
	effect_def->set_simple_describe(simple_describe);

	Ref<JigsawProcedureEffectExtendedDescribe> extended_describe;
	if (fh->read_bool()) {
		extended_describe.instantiate();
		ERR_FAIL_COND_V(!_decode_jigsaw_procedure(fh, extended_describe), false);
	}
	effect_def->set_extended_describe(extended_describe);

	Ref<JigsawProcedureEffectCurve> curve;
	if (fh->read_bool()) {
		curve.instantiate();
		ERR_FAIL_COND_V(!_decode_jigsaw_procedure(fh, curve), false);
	}
	effect_def->set_curve(curve);

	return fh->is_valid();
}
bool DataContainer::_encode_effect_def(const Ref<FormatHelper> &fh, const Ref<EffectDef> &effect_def) const {
	ENCODE_VARIANT(effect_def, editor_name);
	fh->write_svarint(effect_def->get_default_priority());

	PackedStringArray parameter_names = effect_def->get_parameter_names();
	TypedArray<JigsawParameter> default_parameters = effect_def->get_default_parameters();
	ERR_FAIL_COND_V(parameter_names.size() != default_parameters.size(), false);

	fh->write_uvarint(default_parameters.size());

	for (int64_t i = 0; i < default_parameters.size(); i++) {
		fh->write_stringvar(parameter_names[i]);

		Ref<JigsawParameter> param = default_parameters[i];
		ERR_FAIL_COND_V(param.is_null(), false);
		ERR_FAIL_COND_V(!JigsawParameter::is_concrete_type(param->get_type()), false);
		ERR_FAIL_COND_V(!_encode_jigsaw_parameter(fh, param), false);
	}

	TypedArray<JigsawTriggerEffect> triggers = effect_def->get_triggers();
	fh->write_uvarint(triggers.size());
	for (int64_t i = 0; i < triggers.size(); i++) {
		ERR_FAIL_COND_V(!_encode_jigsaw_trigger_effect(fh, triggers[i]), false);
	}

	Ref<JigsawProcedureEffectDescribe> describe = effect_def->get_describe();
	fh->write_bool(describe.is_valid());
	if (describe.is_valid()) {
		ERR_FAIL_COND_V(!_encode_jigsaw_procedure(fh, describe), false);
	}

	Ref<JigsawProcedureEffectSimpleDescribe> simple_describe = effect_def->get_simple_describe();
	fh->write_bool(simple_describe.is_valid());
	if (simple_describe.is_valid()) {
		ERR_FAIL_COND_V(!_encode_jigsaw_procedure(fh, simple_describe), false);
	}

	Ref<JigsawProcedureEffectExtendedDescribe> extended_describe = effect_def->get_extended_describe();
	fh->write_bool(extended_describe.is_valid());
	if (extended_describe.is_valid()) {
		ERR_FAIL_COND_V(!_encode_jigsaw_procedure(fh, extended_describe), false);
	}

	Ref<JigsawProcedureEffectCurve> curve = effect_def->get_curve();
	fh->write_bool(curve.is_valid());
	if (curve.is_valid()) {
		ERR_FAIL_COND_V(!_encode_jigsaw_procedure(fh, curve), false);
	}

	return fh->is_valid();
}

bool DataContainer::_decode_effect_instance(const Ref<FormatHelper> &fh, const Ref<EffectInstance> &effect_instance) {
	DECODE_VARIANT(effect_instance, effect);
	effect_instance->set_priority(fh->read_svarint());

	TypedArray<JigsawParameter> params;
	params.resize(fh->read_uvarint());
	for (int64_t i = 0; i < params.size(); i++) {
		Ref<JigsawParameter> param;
		ERR_FAIL_COND_V(!_decode_jigsaw_parameter(fh, param), false);
		ERR_FAIL_COND_V(param.is_null(), false);
		params[i] = param;
	}
	effect_instance->set_params(params);

	return fh->is_valid();
}
bool DataContainer::_encode_effect_instance(const Ref<FormatHelper> &fh, const Ref<EffectInstance> &effect_instance) const {
	ENCODE_VARIANT(effect_instance, effect);
	fh->write_svarint(effect_instance->get_priority());

	TypedArray<JigsawParameter> params = effect_instance->get_params();
	fh->write_uvarint(params.size());
	for (int64_t i = 0; i < params.size(); i++) {
		Ref<JigsawParameter> param = params[i];
		ERR_FAIL_COND_V(param.is_null(), false);
		ERR_FAIL_COND_V(!_encode_jigsaw_parameter(fh, param), false);
	}

	return fh->is_valid();
}

bool DataContainer::_decode_location_def(const Ref<FormatHelper> &fh, const Ref<LocationDef> &location_def) {
	DECODE_VARIANT(location_def, editor_name);

	return fh->is_valid();
}
bool DataContainer::_encode_location_def(const Ref<FormatHelper> &fh, const Ref<LocationDef> &location_def) const {
	ENCODE_VARIANT(location_def, editor_name);

	return fh->is_valid();
}

bool DataContainer::_decode_modifier_def(const Ref<FormatHelper> &fh, const Ref<ModifierDef> &modifier_def) {
	DECODE_VARIANT(modifier_def, editor_name);

	TypedArray<JigsawTriggerModifier> triggers;
	triggers.resize(fh->read_uvarint());
	for (int64_t i = 0; i < triggers.size(); i++) {
		Ref<JigsawTriggerModifier> trigger;
		ERR_FAIL_COND_V(!_decode_jigsaw_trigger_modifier(fh, trigger), false);
		triggers[i] = trigger;
	}
	modifier_def->set_triggers(triggers);

	Ref<JigsawProcedureModifierDescribe> describe;
	if (fh->read_bool()) {
		describe.instantiate();
		ERR_FAIL_COND_V(!_decode_jigsaw_procedure(fh, describe), false);
	}
	modifier_def->set_describe(describe);

	Ref<JigsawProcedureModifierSimpleDescribe> simple_describe;
	if (fh->read_bool()) {
		simple_describe.instantiate();
		ERR_FAIL_COND_V(!_decode_jigsaw_procedure(fh, simple_describe), false);
	}
	modifier_def->set_simple_describe(simple_describe);

	Ref<JigsawProcedureModifierExtendedDescribe> extended_describe;
	if (fh->read_bool()) {
		extended_describe.instantiate();
		ERR_FAIL_COND_V(!_decode_jigsaw_procedure(fh, extended_describe), false);
	}
	modifier_def->set_extended_describe(extended_describe);

	return fh->is_valid();
}
bool DataContainer::_encode_modifier_def(const Ref<FormatHelper> &fh, const Ref<ModifierDef> &modifier_def) const {
	ENCODE_VARIANT(modifier_def, editor_name);

	TypedArray<JigsawTriggerModifier> triggers = modifier_def->get_triggers();
	fh->write_uvarint(triggers.size());
	for (int64_t i = 0; i < triggers.size(); i++) {
		ERR_FAIL_COND_V(!_encode_jigsaw_trigger_modifier(fh, triggers[i]), false);
	}

	Ref<JigsawProcedureModifierDescribe> describe = modifier_def->get_describe();
	fh->write_bool(describe.is_valid());
	if (describe.is_valid()) {
		ERR_FAIL_COND_V(!_encode_jigsaw_procedure(fh, describe), false);
	}

	Ref<JigsawProcedureModifierSimpleDescribe> simple_describe = modifier_def->get_simple_describe();
	fh->write_bool(simple_describe.is_valid());
	if (simple_describe.is_valid()) {
		ERR_FAIL_COND_V(!_encode_jigsaw_procedure(fh, simple_describe), false);
	}

	Ref<JigsawProcedureModifierExtendedDescribe> extended_describe = modifier_def->get_extended_describe();
	fh->write_bool(extended_describe.is_valid());
	if (extended_describe.is_valid()) {
		ERR_FAIL_COND_V(!_encode_jigsaw_procedure(fh, extended_describe), false);
	}

	return fh->is_valid();
}

bool DataContainer::_decode_npc_def(const Ref<FormatHelper> &fh, const Ref<NPCDef> &npc_def) {
	DECODE_VARIANT(npc_def, display_name);
	DECODE_VARIANT(npc_def, character);

	TypedArray<JigsawTriggerNPC> triggers;
	triggers.resize(fh->read_uvarint());
	for (int64_t i = 0; i < triggers.size(); i++) {
		Ref<JigsawTriggerNPC> trigger;
		ERR_FAIL_COND_V(!_decode_jigsaw_trigger_npc(fh, trigger), false);
		triggers[i] = trigger;
	}
	npc_def->set_triggers(triggers);

	Ref<JigsawProcedureNPCBuildDeck> build_deck;
	if (fh->read_bool()) {
		build_deck.instantiate();
		ERR_FAIL_COND_V(!_decode_jigsaw_procedure(fh, build_deck), false);
	}
	npc_def->set_build_deck(build_deck);

	Ref<JigsawProcedureNPCDecideChoice> decide_choice;
	if (fh->read_bool()) {
		decide_choice.instantiate();
		ERR_FAIL_COND_V(!_decode_jigsaw_procedure(fh, decide_choice), false);
	}
	npc_def->set_decide_choice(decide_choice);

	Ref<JigsawProcedureNPCDecideRealtime> decide_realtime;
	if (fh->read_bool()) {
		decide_realtime.instantiate();
		ERR_FAIL_COND_V(!_decode_jigsaw_procedure(fh, decide_realtime), false);
	}
	npc_def->set_decide_realtime(decide_realtime);

	return fh->is_valid();
}
bool DataContainer::_encode_npc_def(const Ref<FormatHelper> &fh, const Ref<NPCDef> &npc_def) const {
	ENCODE_VARIANT(npc_def, display_name);
	ENCODE_VARIANT(npc_def, character);

	TypedArray<JigsawTriggerNPC> triggers = npc_def->get_triggers();
	fh->write_uvarint(triggers.size());
	for (int64_t i = 0; i < triggers.size(); i++) {
		ERR_FAIL_COND_V(!_encode_jigsaw_trigger_npc(fh, triggers[i]), false);
	}

	Ref<JigsawProcedureNPCBuildDeck> build_deck = npc_def->get_build_deck();
	fh->write_bool(build_deck.is_valid());
	if (build_deck.is_valid()) {
		ERR_FAIL_COND_V(!_encode_jigsaw_procedure(fh, build_deck), false);
	}

	Ref<JigsawProcedureNPCDecideChoice> decide_choice = npc_def->get_decide_choice();
	fh->write_bool(decide_choice.is_valid());
	if (decide_choice.is_valid()) {
		ERR_FAIL_COND_V(!_encode_jigsaw_procedure(fh, decide_choice), false);
	}

	Ref<JigsawProcedureNPCDecideRealtime> decide_realtime = npc_def->get_decide_realtime();
	fh->write_bool(decide_realtime.is_valid());
	if (decide_realtime.is_valid()) {
		ERR_FAIL_COND_V(!_encode_jigsaw_procedure(fh, decide_realtime), false);
	}

	return fh->is_valid();
}

bool DataContainer::_decode_rank_def(const Ref<FormatHelper> &fh, const Ref<RankDef> &rank_def) {
	DECODE_VARIANT(rank_def, name);
	DECODE_VARIANT(rank_def, color);
	DECODE_VARIANT(rank_def, color_alt);
	DECODE_VARIANT(rank_def, front);
	DECODE_VARIANT(rank_def, back);

	Ref<CardDesign> card_design;
	if (fh->read_bool()) {
		card_design.instantiate();
		ERR_FAIL_COND_V(!_decode_card_design(fh, card_design), false);
	}
	rank_def->set_custom_design(card_design);

	return fh->is_valid();
}
bool DataContainer::_encode_rank_def(const Ref<FormatHelper> &fh, const Ref<RankDef> &rank_def) const {
	ENCODE_VARIANT(rank_def, name);
	ENCODE_VARIANT(rank_def, color);
	ENCODE_VARIANT(rank_def, color_alt);
	ENCODE_VARIANT(rank_def, front);
	ENCODE_VARIANT(rank_def, back);

	Ref<CardDesign> card_design = rank_def->get_custom_design();
	fh->write_bool(card_design.is_valid());
	if (card_design.is_valid()) {
		ERR_FAIL_COND_V(!_encode_card_design(fh, card_design), false);
	}

	return fh->is_valid();
}

bool DataContainer::_decode_stat_def(const Ref<FormatHelper> &fh, const Ref<StatDef> &stat_def) {
	DECODE_VARIANT(stat_def, name);
	DECODE_VARIANT(stat_def, icon);

	Ref<JigsawProcedureStatFormatCost> format_cost;
	if (fh->read_bool()) {
		format_cost.instantiate();
		ERR_FAIL_COND_V(!_decode_jigsaw_procedure(fh, format_cost), false);
	}
	stat_def->set_format_cost(format_cost);

	return fh->is_valid();
}
bool DataContainer::_encode_stat_def(const Ref<FormatHelper> &fh, const Ref<StatDef> &stat_def) const {
	ENCODE_VARIANT(stat_def, name);
	ENCODE_VARIANT(stat_def, icon);

	Ref<JigsawProcedureStatFormatCost> format_cost = stat_def->get_format_cost();
	fh->write_bool(format_cost.is_valid());
	if (format_cost.is_valid()) {
		ERR_FAIL_COND_V(!_encode_jigsaw_procedure(fh, format_cost), false);
	}

	return fh->is_valid();
}

bool DataContainer::_decode_sticker_def(const Ref<FormatHelper> &fh, const Ref<StickerDef> &sticker_def) {
	DECODE_VARIANT(sticker_def, icon);
	DECODE_VARIANT(sticker_def, position);

	Ref<JigsawProcedureStickerShouldShow> should_show;
	if (fh->read_bool()) {
		should_show.instantiate();
		ERR_FAIL_COND_V(!_decode_jigsaw_procedure(fh, should_show), false);
	}
	sticker_def->set_should_show(should_show);

	return fh->is_valid();
}
bool DataContainer::_encode_sticker_def(const Ref<FormatHelper> &fh, const Ref<StickerDef> &sticker_def) const {
	ENCODE_VARIANT(sticker_def, icon);
	ENCODE_VARIANT(sticker_def, position);

	Ref<JigsawProcedureStickerShouldShow> should_show = sticker_def->get_should_show();
	fh->write_bool(should_show.is_valid());
	if (should_show.is_valid()) {
		ERR_FAIL_COND_V(!_encode_jigsaw_procedure(fh, should_show), false);
	}

	return fh->is_valid();
}

bool DataContainer::_decode_tribe_def(const Ref<FormatHelper> &fh, const Ref<TribeDef> &tribe_def) {
	DECODE_VARIANT(tribe_def, name);
	DECODE_VARIANT(tribe_def, color);
	tribe_def->set_display(static_cast<TribeDef::DisplayMode>(fh->read_uvarint()));

	return fh->is_valid();
}
bool DataContainer::_encode_tribe_def(const Ref<FormatHelper> &fh, const Ref<TribeDef> &tribe_def) const {
	ENCODE_VARIANT(tribe_def, name);
	ENCODE_VARIANT(tribe_def, color);
	fh->write_uvarint(tribe_def->get_display());

	return fh->is_valid();
}

bool DataContainer::_decode_variable_def(const Ref<FormatHelper> &fh, const Ref<VariableDef> &variable_def) {
	DECODE_VARIANT(variable_def, editor_name);
	variable_def->set_uniqueness(static_cast<VariableDef::Uniqueness>(fh->read_uvarint()));

	Ref<JigsawParameter> default_value;
	ERR_FAIL_COND_V(!_decode_jigsaw_parameter(fh, default_value), false);
	variable_def->set_default_value(default_value);

	return fh->is_valid();
}
bool DataContainer::_encode_variable_def(const Ref<FormatHelper> &fh, const Ref<VariableDef> &variable_def) const {
	ENCODE_VARIANT(variable_def, editor_name);
	fh->write_uvarint(variable_def->get_uniqueness());

	Ref<JigsawParameter> default_value = variable_def->get_default_value();
	ERR_FAIL_COND_V(!_encode_jigsaw_parameter(fh, default_value), false);

	return fh->is_valid();
}

bool DataContainer::_decode_variant_def(const Ref<FormatHelper> &fh, const Ref<VariantDef> &variant_def) {
	DECODE_VARIANT(variant_def, title);
	DECODE_VARIANT(variant_def, content_warning);
	variant_def->set_flags(fh->read_uvarint());
	variant_def->set_player_count(fh->read_uvarint());

	TypedArray<enums::NPCDef::NPC> npcs;
	npcs.resize(fh->read_uvarint());
	for (int64_t i = 0; i < npcs.size(); i++) {
		npcs[i] = fh->read_id<enums::NPCDef::NPC>();
	}
	variant_def->set_npcs(npcs);

	TypedArray<JigsawTriggerVariant> triggers;
	triggers.resize(fh->read_uvarint());
	for (int64_t i = 0; i < triggers.size(); i++) {
		Ref<JigsawTriggerVariant> trigger;
		ERR_FAIL_COND_V(!_decode_jigsaw_trigger_variant(fh, trigger), false);
		triggers[i] = trigger;
	}
	variant_def->set_triggers(triggers);

	return fh->is_valid();
}
bool DataContainer::_encode_variant_def(const Ref<FormatHelper> &fh, const Ref<VariantDef> &variant_def) const {
	ERR_FAIL_COND_V(variant_def.is_null(), false);

	ENCODE_VARIANT(variant_def, title);
	ENCODE_VARIANT(variant_def, content_warning);
	fh->write_uvarint(variant_def->get_flags());
	fh->write_uvarint(variant_def->get_player_count());

	TypedArray<enums::NPCDef::NPC> npcs = variant_def->get_npcs();
	fh->write_uvarint(npcs.size());
	for (int64_t i = 0; i < npcs.size(); i++) {
		int64_t npc = npcs[i];
		fh->write_id(static_cast<enums::NPCDef::NPC>(npc));
	}

	TypedArray<JigsawTriggerVariant> triggers = variant_def->get_triggers();
	fh->write_uvarint(triggers.size());
	for (int64_t i = 0; i < triggers.size(); i++) {
		ERR_FAIL_COND_V(!_encode_jigsaw_trigger_variant(fh, triggers[i]), false);
	}

	return fh->is_valid();
}
