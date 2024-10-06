#include "game_mode.h"

void GameMode::_bind_methods() {
	BIND_PROPERTY(Variant::INT, visual_effect_highlight_width);
	BIND_PROPERTY(Variant::COLOR, visual_effect_highlight_color);
	BIND_PROPERTY(Variant::COLOR, visual_description_background_color);

	BIND_PROPERTY(Variant::ARRAY, audience, PROPERTY_HINT_TYPE_STRING, String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":AudienceDef");
	BIND_PROPERTY(Variant::ARRAY, characters, PROPERTY_HINT_TYPE_STRING, String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":CharacterDef");

	BIND_PROPERTY(Variant::OBJECT, base_variant, PROPERTY_HINT_RESOURCE_TYPE, "VariantDef");
	BIND_PROPERTY(Variant::ARRAY, variants, PROPERTY_HINT_TYPE_STRING, String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":VariantDef");

	BIND_PROPERTY(Variant::INT, ranks);
	BIND_PROPERTY(Variant::INT, tribes);
	BIND_PROPERTY(Variant::INT, stats);
	BIND_PROPERTY(Variant::INT, modifiers);
	BIND_PROPERTY(Variant::INT, effects);
	BIND_PROPERTY(Variant::INT, npcs);

	BIND_PROPERTY(Variant::ARRAY, custom_ranks, PROPERTY_HINT_TYPE_STRING, String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":RankDef");
	BIND_PROPERTY(Variant::ARRAY, custom_tribes, PROPERTY_HINT_TYPE_STRING, String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":TribeDef");
	BIND_PROPERTY(Variant::ARRAY, custom_stats, PROPERTY_HINT_TYPE_STRING, String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":StatDef");
	BIND_PROPERTY(Variant::ARRAY, custom_modifiers, PROPERTY_HINT_TYPE_STRING, String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":ModifierDef");
	BIND_PROPERTY(Variant::ARRAY, custom_effects, PROPERTY_HINT_TYPE_STRING, String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":EffectDef");
	BIND_PROPERTY(Variant::ARRAY, custom_npcs, PROPERTY_HINT_TYPE_STRING, String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":NPCDef");

	BIND_PROPERTY(Variant::ARRAY, custom_variables, PROPERTY_HINT_TYPE_STRING, String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":VariableDef");
	BIND_PROPERTY(Variant::ARRAY, custom_choices, PROPERTY_HINT_TYPE_STRING, String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":ChoicesDef");
	BIND_PROPERTY(Variant::ARRAY, custom_functions, PROPERTY_HINT_TYPE_STRING, String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":JigsawReusableCommandList");

	BIND_PROPERTY(Variant::ARRAY, card_defs, PROPERTY_HINT_TYPE_STRING, String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":CardDef");

	ClassDB::bind_method(D_METHOD("get_card", "id"), &GameMode::get_card);
	ClassDB::bind_method(D_METHOD("get_rank", "id"), &GameMode::get_rank);
	ClassDB::bind_method(D_METHOD("get_tribe", "id"), &GameMode::get_tribe);
	ClassDB::bind_method(D_METHOD("get_stat", "id"), &GameMode::get_stat);
	ClassDB::bind_method(D_METHOD("get_modifier", "id"), &GameMode::get_modifier);
}

IMPLEMENT_PROPERTY(GameMode, int64_t, visual_effect_highlight_width);
IMPLEMENT_PROPERTY(GameMode, Color, visual_effect_highlight_color);
IMPLEMENT_PROPERTY(GameMode, Color, visual_description_background_color);

IMPLEMENT_PROPERTY(GameMode, TypedArray<AudienceDef>, audience);
IMPLEMENT_PROPERTY(GameMode, TypedArray<CharacterDef>, characters);

IMPLEMENT_PROPERTY(GameMode, Ref<VariantDef>, base_variant);
IMPLEMENT_PROPERTY(GameMode, TypedArray<VariantDef>, variants);

IMPLEMENT_PROPERTY(GameMode, TypedArray<RankDef::Rank>, ranks);
IMPLEMENT_PROPERTY(GameMode, TypedArray<TribeDef::Tribe>, tribes);
IMPLEMENT_PROPERTY(GameMode, TypedArray<StatDef::Stat>, stats);
IMPLEMENT_PROPERTY(GameMode, TypedArray<ModifierDef::Modifier>, modifiers);
IMPLEMENT_PROPERTY(GameMode, TypedArray<EffectDef::Effect>, effects);
IMPLEMENT_PROPERTY(GameMode, TypedArray<NPCDef::NPC>, npcs);

IMPLEMENT_PROPERTY(GameMode, TypedArray<RankDef>, custom_ranks);
IMPLEMENT_PROPERTY(GameMode, TypedArray<TribeDef>, custom_tribes);
IMPLEMENT_PROPERTY(GameMode, TypedArray<StatDef>, custom_stats);
IMPLEMENT_PROPERTY(GameMode, TypedArray<ModifierDef>, custom_modifiers);
IMPLEMENT_PROPERTY(GameMode, TypedArray<EffectDef>, custom_effects);
IMPLEMENT_PROPERTY(GameMode, TypedArray<NPCDef>, custom_npcs);

IMPLEMENT_PROPERTY(GameMode, TypedArray<VariableDef>, custom_variables);
IMPLEMENT_PROPERTY(GameMode, TypedArray<ChoicesDef>, custom_choices);
IMPLEMENT_PROPERTY(GameMode, TypedArray<JigsawReusableCommandList>, custom_functions);

IMPLEMENT_PROPERTY(GameMode, TypedArray<CardDef>, card_defs);

Ref<CardDef> GameMode::get_card(CardDef::Card id) const {
	for (int64_t i = 0; i < card_defs.size(); i++) {
		CardDef *card = Object::cast_to<CardDef>(card_defs[i]);
		if (!card) {
			continue;
		}

		if (card->get_id() == id) {
			return card;
		}
	}

	ERR_FAIL_V_MSG(nullptr, "Custom card not defined");
}

#define IMPLEMENT_TYPE_GETTER(Type, type) \
	Ref<Type##Def> GameMode::get_##type(Type##Def::Type id) const { \
		ERR_FAIL_COND_V_MSG(!type##s.has(id), nullptr, #type " not part of this mode"); \
		if (id < Type##Def::FIRST_CUSTOM) { \
			return get_predefined<Type##Def>(id); \
		} \
		ERR_FAIL_INDEX_V_MSG(id - Type##Def::FIRST_CUSTOM, custom_##type##s.size(), nullptr, "custom " #type " not defined"); \
		return Object::cast_to<Type##Def>(custom_##type##s[id - Type##Def::FIRST_CUSTOM]); \
	}

IMPLEMENT_TYPE_GETTER(Rank, rank);
IMPLEMENT_TYPE_GETTER(Tribe, tribe);
IMPLEMENT_TYPE_GETTER(Stat, stat);
IMPLEMENT_TYPE_GETTER(Modifier, modifier);
IMPLEMENT_TYPE_GETTER(Effect, effect);
IMPLEMENT_TYPE_GETTER(NPC, npc);
