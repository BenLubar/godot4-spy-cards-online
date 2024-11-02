#include "jigsaw_command_lookup_definition_property.h"

#include "jigsaw_parameter_amount.h"
#include "jigsaw_parameter_boolean.h"
#include "jigsaw_parameter_card.h"
#include "jigsaw_parameter_card_instance.h"
#include "jigsaw_parameter_color.h"
#include "jigsaw_parameter_effect_instance.h"
#include "jigsaw_parameter_icon.h"
#include "jigsaw_parameter_ordered_list.h"
#include "jigsaw_parameter_rank.h"
#include "jigsaw_parameter_stat.h"
#include "jigsaw_parameter_stat_value.h"
#include "jigsaw_parameter_string.h"
#include "jigsaw_parameter_tribe.h"

void JigsawCommandLookupDefinitionProperty::_bind_methods() {
	BIND_ENUM_CONSTANT(CARD_DEF);
	BIND_ENUM_CONSTANT(CARD_NAME);
	BIND_ENUM_CONSTANT(CARD_RANK);
	BIND_ENUM_CONSTANT(CARD_COSTS);
	BIND_ENUM_CONSTANT(CARD_PORTRAIT);
	BIND_ENUM_CONSTANT(CARD_TRIBES);
	BIND_ENUM_CONSTANT(CARD_EFFECTS);

	BIND_ENUM_CONSTANT(RANK_NAME);
	BIND_ENUM_CONSTANT(RANK_COLOR);
	BIND_ENUM_CONSTANT(RANK_COLOR_ALT);
	BIND_ENUM_CONSTANT(RANK_COLOR_BASED_ON_PREFERENCES);
	BIND_ENUM_CONSTANT(RANK_FRONT);
	BIND_ENUM_CONSTANT(RANK_BACK);

	BIND_ENUM_CONSTANT(TRIBE_NAME);
	BIND_ENUM_CONSTANT(TRIBE_COLOR);
	BIND_ENUM_CONSTANT(TRIBE_IS_HIDDEN);
	BIND_ENUM_CONSTANT(TRIBE_IS_WIDE);

	BIND_ENUM_CONSTANT(STAT_DEF);
	BIND_ENUM_CONSTANT(STAT_NAME);
	BIND_ENUM_CONSTANT(STAT_ICON);
	BIND_ENUM_CONSTANT(STAT_AMOUNT);

	BIND_PROPERTY_ENUM(JigsawCommandLookupDefinitionProperty::Property, property);
	BIND_PROPERTY_RESOURCE(JigsawParameter, definition);
	BIND_PROPERTY_RESOURCE(JigsawParameterLocalVariable, value);
}

IMPLEMENT_PROPERTY(JigsawCommandLookupDefinitionProperty, JigsawCommandLookupDefinitionProperty::Property, property);
IMPLEMENT_PROPERTY(JigsawCommandLookupDefinitionProperty, Ref<JigsawParameter>, definition);
IMPLEMENT_PROPERTY(JigsawCommandLookupDefinitionProperty, Ref<JigsawParameterLocalVariable>, value);

int64_t JigsawCommandLookupDefinitionProperty::get_num_configs() const {
	return 1;
}
String JigsawCommandLookupDefinitionProperty::get_config_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "Property";
}
String JigsawCommandLookupDefinitionProperty::get_config_desc(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "";
}
int64_t JigsawCommandLookupDefinitionProperty::get_config_value(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, 0);

	return _property;
}
void JigsawCommandLookupDefinitionProperty::set_config_value(int64_t i, int64_t value) {
	ERR_FAIL_INDEX(i, 1);

	_property = static_cast<Property>(value);
	emit_changed();
}
PackedStringArray JigsawCommandLookupDefinitionProperty::get_config_options(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, PackedStringArray());

	// if any of these asserts fail, you've broken compatibility with existing game modes.
	static_assert(CARD_DEF == 0);
	static_assert(CARD_NAME == 1);
	static_assert(CARD_RANK == 2);
	static_assert(CARD_COSTS == 3);
	static_assert(CARD_PORTRAIT == 4);
	static_assert(CARD_TRIBES == 5);
	static_assert(CARD_EFFECTS == 6);
	static_assert(RANK_NAME == 7);
	static_assert(RANK_COLOR == 8);
	static_assert(RANK_COLOR_ALT == 9);
	static_assert(RANK_COLOR_BASED_ON_PREFERENCES == 10);
	static_assert(RANK_FRONT == 11);
	static_assert(RANK_BACK == 12);
	static_assert(TRIBE_NAME == 13);
	static_assert(TRIBE_COLOR == 14);
	static_assert(TRIBE_IS_HIDDEN == 15);
	static_assert(TRIBE_IS_WIDE == 16);
	static_assert(STAT_DEF == 17);
	static_assert(STAT_NAME == 18);
	static_assert(STAT_ICON == 19);

	return PackedStringArray{
		"Card: Definition",
		"Card: Name",
		"Card: Rank",
		"Card: Costs",
		"Card: Portrait",
		"Card: Tribes",
		"Card: Effects",
		"Rank: Name",
		"Rank: Color (default)",
		"Rank: Color (alternate)",
		"Rank: Color (based on preferences)",
		"Rank: Front texture",
		"Rank: Back texture",
		"Tribe: Name",
		"Tribe: Color",
		"Tribe: Hidden",
		"Tribe: Wide",
		"Stat: Definition",
		"Stat: Name",
		"Stat: Icon",
	};
}

int64_t JigsawCommandLookupDefinitionProperty::get_num_arguments() const {
	return 1;
}
Ref<JigsawParameter> JigsawCommandLookupDefinitionProperty::get_argument(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, Ref<JigsawParameter>());

	return _definition;
}
TypedArray<JigsawParameter> JigsawCommandLookupDefinitionProperty::get_argument_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, 1, TypedArray<JigsawParameter>());

	switch (_property) {
	case CARD_DEF:
		return Array::make(JigsawParameterCardInstance::make(Ref<CardInstance>()));
	case CARD_NAME:
	case CARD_RANK:
	case CARD_COSTS:
	case CARD_PORTRAIT:
	case CARD_TRIBES:
	case CARD_EFFECTS:
		return Array::make(JigsawParameterCard::make(enums::CardDef::NONE), JigsawParameterCardInstance::make(Ref<CardInstance>()));
	case RANK_NAME:
	case RANK_COLOR:
	case RANK_COLOR_ALT:
	case RANK_COLOR_BASED_ON_PREFERENCES:
	case RANK_FRONT:
	case RANK_BACK:
		return Array::make(JigsawParameterRank::make(enums::RankDef::NONE), JigsawParameterCard::make(enums::CardDef::NONE), JigsawParameterCardInstance::make(Ref<CardInstance>()));
	case TRIBE_NAME:
	case TRIBE_COLOR:
	case TRIBE_IS_HIDDEN:
	case TRIBE_IS_WIDE:
		return Array::make(JigsawParameterTribe::make(enums::TribeDef::NONE));
	case STAT_DEF:
		return Array::make(JigsawParameterStatValue::make(enums::StatDef::NONE, 0));
	case STAT_NAME:
	case STAT_ICON:
		return Array::make(JigsawParameterStat::make(enums::StatDef::NONE), JigsawParameterStatValue::make(enums::StatDef::NONE, 0));
	case STAT_AMOUNT:
		return Array::make(JigsawParameterStatValue::make(enums::StatDef::NONE, 0));
	}

	return TypedArray<JigsawParameter>();
}
void JigsawCommandLookupDefinitionProperty::set_argument(int64_t i, const Ref<JigsawParameter> &arg) {
	ERR_FAIL_INDEX(i, 1);

	_definition = arg;
	emit_changed();
}
String JigsawCommandLookupDefinitionProperty::get_argument_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	switch (_property) {
	case CARD_DEF:
	case CARD_NAME:
	case CARD_RANK:
	case CARD_COSTS:
	case CARD_PORTRAIT:
	case CARD_TRIBES:
	case CARD_EFFECTS:
		return "card";
	case RANK_NAME:
	case RANK_COLOR:
	case RANK_COLOR_ALT:
	case RANK_COLOR_BASED_ON_PREFERENCES:
	case RANK_FRONT:
	case RANK_BACK:
		return "rank";
	case TRIBE_NAME:
	case TRIBE_COLOR:
	case TRIBE_IS_HIDDEN:
	case TRIBE_IS_WIDE:
		return "tribe";
	case STAT_DEF:
	case STAT_NAME:
	case STAT_ICON:
	case STAT_AMOUNT:
		return "stat";
	}

	return "";
}

int64_t JigsawCommandLookupDefinitionProperty::get_num_results() const {
	return 1;
}
Ref<JigsawParameterLocalVariable> JigsawCommandLookupDefinitionProperty::get_result(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, Ref<JigsawParameterLocalVariable>());

	return _value;
}
TypedArray<JigsawParameter> JigsawCommandLookupDefinitionProperty::get_result_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, 1, TypedArray<JigsawParameter>());

	switch (_property) {
	case CARD_DEF:
		return Array::make(JigsawParameterCard::make(enums::CardDef::NONE));
	case CARD_NAME:
		return Array::make(JigsawParameterString::make(""));
	case CARD_RANK:
		return Array::make(JigsawParameterRank::make(enums::RankDef::NONE));
	case CARD_COSTS:
		return Array::make(JigsawParameterOrderedList::make_template(Array::make(JigsawParameterStatValue::make(enums::StatDef::NONE, 0))));
	case CARD_PORTRAIT:
		return Array::make(JigsawParameterIcon::make(enums::IconDef::NONE));
	case CARD_TRIBES:
		return Array::make(JigsawParameterOrderedList::make_template(Array::make(JigsawParameterTribe::make(enums::TribeDef::NONE))));
	case CARD_EFFECTS:
		return Array::make(JigsawParameterOrderedList::make_template(Array::make(JigsawParameterEffectInstance::make(Ref<EffectInstance>()))));
	case RANK_NAME:
		return Array::make(JigsawParameterString::make(""));
	case RANK_COLOR:
	case RANK_COLOR_ALT:
	case RANK_COLOR_BASED_ON_PREFERENCES:
		return Array::make(JigsawParameterColor::make(Color()));
	case RANK_FRONT:
	case RANK_BACK:
		return Array::make(JigsawParameterIcon::make(enums::IconDef::NONE));
	case TRIBE_NAME:
		return Array::make(JigsawParameterString::make(""));
	case TRIBE_COLOR:
		return Array::make(JigsawParameterColor::make(Color()));
	case TRIBE_IS_HIDDEN:
	case TRIBE_IS_WIDE:
		return Array::make(JigsawParameterBoolean::make(false));
	case STAT_DEF:
		return Array::make(JigsawParameterStatValue::make(enums::StatDef::NONE, 0));
	case STAT_NAME:
		return Array::make(JigsawParameterString::make(""));
	case STAT_ICON:
		return Array::make(JigsawParameterIcon::make(enums::IconDef::NONE));
	case STAT_AMOUNT:
		return Array::make(JigsawParameterAmount::make(0));
	}

	return TypedArray<JigsawParameter>();
}
void JigsawCommandLookupDefinitionProperty::set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) {
	ERR_FAIL_INDEX(i, 1);

	_value = result;
	emit_changed();
}
String JigsawCommandLookupDefinitionProperty::get_result_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	switch (_property) {
	case CARD_DEF:
		return "def";
	case CARD_NAME:
		return "name";
	case CARD_RANK:
		return "rank";
	case CARD_COSTS:
		return "costs";
	case CARD_PORTRAIT:
		return "portrait";
	case CARD_TRIBES:
		return "tribes";
	case CARD_EFFECTS:
		return "effects";
	case RANK_NAME:
		return "name";
	case RANK_COLOR:
		return "color";
	case RANK_COLOR_ALT:
		return "color";
	case RANK_COLOR_BASED_ON_PREFERENCES:
		return "color";
	case RANK_FRONT:
		return "front";
	case RANK_BACK:
		return "back";
	case TRIBE_NAME:
		return "name";
	case TRIBE_COLOR:
		return "color";
	case TRIBE_IS_HIDDEN:
		return "is_hidden";
	case TRIBE_IS_WIDE:
		return "is_wide";
	case STAT_DEF:
		return "def";
	case STAT_NAME:
		return "name";
	case STAT_ICON:
		return "icon";
	case STAT_AMOUNT:
		return "amount";
	}

	return "";
}
