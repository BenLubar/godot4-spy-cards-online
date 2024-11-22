#include "defs/game_mode.h"

void GameMode::_bind_methods() {
	BIND_PROPERTY_RESOURCE_ARRAY(AudienceDef, audience);
	BIND_PROPERTY_RESOURCE_ARRAY(CharacterDef, characters);

	BIND_PROPERTY_RESOURCE(CardDesign, default_card_design);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawTriggerVariant, base_triggers);
	BIND_PROPERTY_RESOURCE_ARRAY(VariantDef, variants);

	BIND_PROPERTY_ENUM_ARRAY(enums::RankDef::Rank, ranks);
	BIND_PROPERTY_ENUM_ARRAY(enums::TribeDef::Tribe, tribes);
	BIND_PROPERTY_ENUM_ARRAY(enums::StatDef::Stat, stats);
	BIND_PROPERTY_ENUM_ARRAY(enums::ModifierDef::Modifier, modifiers);
	BIND_PROPERTY_ENUM_ARRAY(enums::EffectDef::Effect, effects);
	BIND_PROPERTY_ENUM_ARRAY(enums::NPCDef::NPC, npcs);

	BIND_PROPERTY_RESOURCE_ARRAY(RankDef, custom_ranks);
	BIND_PROPERTY_RESOURCE_ARRAY(TribeDef, custom_tribes);
	BIND_PROPERTY_RESOURCE_ARRAY(StatDef, custom_stats);
	BIND_PROPERTY_RESOURCE_ARRAY(ModifierDef, custom_modifiers);
	BIND_PROPERTY_RESOURCE_ARRAY(EffectDef, custom_effects);
	BIND_PROPERTY_RESOURCE_ARRAY(NPCDef, custom_npcs);

	BIND_PROPERTY_RESOURCE_ARRAY(LocationDef, custom_locations);
	BIND_PROPERTY_RESOURCE_ARRAY(VariableDef, custom_variables);
	BIND_PROPERTY_RESOURCE_ARRAY(ChoicesDef, custom_choices);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawFunction, custom_functions);

	BIND_PROPERTY_RESOURCE_ARRAY(CardDef, card_defs);

	ClassDB::bind_method(D_METHOD("get_card", "id"), &GameMode::get_card);
	ClassDB::bind_method(D_METHOD("get_rank", "id"), &GameMode::get_rank);
	ClassDB::bind_method(D_METHOD("get_tribe", "id"), &GameMode::get_tribe);
	ClassDB::bind_method(D_METHOD("get_stat", "id"), &GameMode::get_stat);
	ClassDB::bind_method(D_METHOD("get_modifier", "id"), &GameMode::get_modifier);
	ClassDB::bind_method(D_METHOD("get_variable", "id"), &GameMode::get_variable);
	ClassDB::bind_method(D_METHOD("get_location", "id"), &GameMode::get_location);
}

IMPLEMENT_PROPERTY(GameMode, TypedArray<AudienceDef>, audience);
IMPLEMENT_PROPERTY(GameMode, TypedArray<CharacterDef>, characters);

IMPLEMENT_PROPERTY(GameMode, Ref<CardDesign>, default_card_design);
IMPLEMENT_PROPERTY(GameMode, TypedArray<JigsawTriggerVariant>, base_triggers);
IMPLEMENT_PROPERTY(GameMode, TypedArray<VariantDef>, variants);

IMPLEMENT_PROPERTY(GameMode, TypedArray<enums::RankDef::Rank>, ranks);
IMPLEMENT_PROPERTY(GameMode, TypedArray<enums::TribeDef::Tribe>, tribes);
IMPLEMENT_PROPERTY(GameMode, TypedArray<enums::StatDef::Stat>, stats);
IMPLEMENT_PROPERTY(GameMode, TypedArray<enums::ModifierDef::Modifier>, modifiers);
IMPLEMENT_PROPERTY(GameMode, TypedArray<enums::EffectDef::Effect>, effects);
IMPLEMENT_PROPERTY(GameMode, TypedArray<enums::NPCDef::NPC>, npcs);

IMPLEMENT_PROPERTY(GameMode, TypedArray<RankDef>, custom_ranks);
IMPLEMENT_PROPERTY(GameMode, TypedArray<TribeDef>, custom_tribes);
IMPLEMENT_PROPERTY(GameMode, TypedArray<StatDef>, custom_stats);
IMPLEMENT_PROPERTY(GameMode, TypedArray<ModifierDef>, custom_modifiers);
IMPLEMENT_PROPERTY(GameMode, TypedArray<EffectDef>, custom_effects);
IMPLEMENT_PROPERTY(GameMode, TypedArray<NPCDef>, custom_npcs);

IMPLEMENT_PROPERTY(GameMode, TypedArray<LocationDef>, custom_locations);
IMPLEMENT_PROPERTY(GameMode, TypedArray<VariableDef>, custom_variables);
IMPLEMENT_PROPERTY(GameMode, TypedArray<ChoicesDef>, custom_choices);
IMPLEMENT_PROPERTY(GameMode, TypedArray<JigsawFunction>, custom_functions);

IMPLEMENT_PROPERTY(GameMode, TypedArray<CardDef>, card_defs);

Ref<CardDef> GameMode::get_card(enums::CardDef::Card id) const {
	for (int64_t i = 0; i < _card_defs.size(); i++) {
		Ref<CardDef> card = _card_defs[i];
		if (card.is_valid() && card->get_id() == id) {
			return card;
		}
	}

	ERR_FAIL_V_MSG(Ref<CardDef>(), String("custom card %d not defined") % Array::make(id));
}

#define IMPLEMENT_TYPE_GETTER(Type, type) \
	Ref<Type##Def> GameMode::get_##type(enums::Type##Def::Type id) const { \
		ERR_FAIL_COND_V_MSG(!_##type##s.has(id), Ref<Type##Def>(), vformat("%s %d not part of this mode", #type, id)); \
		if (id < enums::Type##Def::FIRST_CUSTOM) { \
			return get_predefined<Type##Def>(id); \
		} \
		ERR_FAIL_INDEX_V_MSG(id - enums::Type##Def::FIRST_CUSTOM, _custom_##type##s.size(), Ref<Type##Def>(), vformat("custom %s %d not defined", #type, id)); \
		Ref<Type##Def> obj = _custom_##type##s[id - enums::Type##Def::FIRST_CUSTOM]; \
		ERR_FAIL_COND_V_MSG(obj.is_null(), Ref<Type##Def>(), vformat("custom %s %d is null", #type, id)); \
		return obj; \
	}

IMPLEMENT_TYPE_GETTER(Rank, rank);
IMPLEMENT_TYPE_GETTER(Tribe, tribe);
IMPLEMENT_TYPE_GETTER(Stat, stat);
IMPLEMENT_TYPE_GETTER(Modifier, modifier);
IMPLEMENT_TYPE_GETTER(Effect, effect);
IMPLEMENT_TYPE_GETTER(NPC, npc);

#define IMPLEMENT_TYPE_GETTER_NO_VALIDITY_CHECK(Type, type) \
	Ref<Type##Def> GameMode::get_##type(enums::Type##Def::Type id) const { \
		if (id < enums::Type##Def::FIRST_CUSTOM) { \
			return get_predefined<Type##Def>(id); \
		} \
		ERR_FAIL_INDEX_V_MSG(id - enums::Type##Def::FIRST_CUSTOM, _custom_##type##s.size(), Ref<Type##Def>(), vformat("custom %s %d not defined", #type, id)); \
		Ref<Type##Def> obj = _custom_##type##s[id - enums::Type##Def::FIRST_CUSTOM]; \
		ERR_FAIL_COND_V_MSG(obj.is_null(), Ref<Type##Def>(), vformat("custom %s %d is null", #type, id)); \
		return obj; \
	}

IMPLEMENT_TYPE_GETTER_NO_VALIDITY_CHECK(Variable, variable);
IMPLEMENT_TYPE_GETTER_NO_VALIDITY_CHECK(Location, location);
