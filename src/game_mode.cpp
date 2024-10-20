#include "game_mode.h"

void GameMode::_bind_methods() {
	BIND_ENUM_CONSTANT(GROW_LEFT);
	BIND_ENUM_CONSTANT(GROW_RIGHT);
	BIND_ENUM_CONSTANT(GROW_UP);
	BIND_ENUM_CONSTANT(GROW_DOWN);
	BIND_ENUM_CONSTANT(GROW_HORIZONTAL);
	BIND_ENUM_CONSTANT(GROW_VERTICAL);
	BIND_ENUM_CONSTANT(SHRINK_HORIZONTAL);
	BIND_ENUM_CONSTANT(SHRINK_VERTICAL);

	BIND_PROPERTY(Variant::FLOAT, visual_pixel_size);
	BIND_PROPERTY(Variant::TRANSFORM3D, visual_base_transform);
	BIND_PROPERTY(Variant::TRANSFORM3D, visual_portrait_transform);
	BIND_PROPERTY(Variant::TRANSFORM3D, visual_rank_decoration_transform);
	BIND_PROPERTY(Variant::VECTOR3, visual_smooth_corners);

	BIND_PROPERTY_ENUM(enums::IconDef::Icon, visual_card_front);
	BIND_PROPERTY_ENUM(enums::IconDef::Icon, visual_card_front_window);
	BIND_PROPERTY_ENUM(enums::IconDef::Icon, visual_tribe_bubble);
	BIND_PROPERTY_ENUM(enums::IconDef::Icon, visual_tribe_bubble_wide);

	BIND_PROPERTY(Variant::RECT2, visual_card_name_pos);
	BIND_PROPERTY(Variant::VECTOR2, visual_card_name_scale);
	BIND_PROPERTY(Variant::RECT2, visual_card_description_pos);
	BIND_PROPERTY(Variant::VECTOR2, visual_card_description_scale);

	BIND_PROPERTY(Variant::RECT2, visual_card_costs_pos);
	BIND_PROPERTY_ENUM(GrowMode, visual_card_costs_grow_mode);
	BIND_PROPERTY(Variant::BOOL, visual_card_costs_shrink_name);
	BIND_PROPERTY(Variant::FLOAT, visual_card_costs_spacing);
	BIND_PROPERTY(Variant::VECTOR2, visual_card_costs_text_scale);

	BIND_PROPERTY(Variant::RECT2, visual_card_tribes_pos);
	BIND_PROPERTY_ENUM(GrowMode, visual_card_tribes_grow_mode);
	BIND_PROPERTY(Variant::BOOL, visual_card_tribes_shrink_name);
	BIND_PROPERTY(Variant::FLOAT, visual_card_tribes_spacing);
	BIND_PROPERTY(Variant::VECTOR2, visual_card_tribes_text_scale);

	BIND_PROPERTY_RESOURCE_ARRAY(StickerDef, visual_card_stickers);

	BIND_PROPERTY(Variant::INT, visual_effect_highlight_width);
	BIND_PROPERTY(Variant::COLOR, visual_effect_highlight_color);
	BIND_PROPERTY(Variant::COLOR, visual_description_background_color);

	BIND_PROPERTY_RESOURCE_ARRAY(AudienceDef, audience);
	BIND_PROPERTY_RESOURCE_ARRAY(CharacterDef, characters);

	BIND_PROPERTY_RESOURCE(VariantDef, base_variant);
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
}

IMPLEMENT_PROPERTY(GameMode, float, visual_pixel_size);
IMPLEMENT_PROPERTY(GameMode, Transform3D, visual_base_transform);
IMPLEMENT_PROPERTY(GameMode, Transform3D, visual_portrait_transform);
IMPLEMENT_PROPERTY(GameMode, Transform3D, visual_rank_decoration_transform);
IMPLEMENT_PROPERTY(GameMode, Vector3, visual_smooth_corners);

IMPLEMENT_PROPERTY(GameMode, enums::IconDef::Icon, visual_card_front);
IMPLEMENT_PROPERTY(GameMode, enums::IconDef::Icon, visual_card_front_window);
IMPLEMENT_PROPERTY(GameMode, enums::IconDef::Icon, visual_tribe_bubble);
IMPLEMENT_PROPERTY(GameMode, enums::IconDef::Icon, visual_tribe_bubble_wide);

IMPLEMENT_PROPERTY(GameMode, Rect2, visual_card_name_pos);
IMPLEMENT_PROPERTY(GameMode, Vector2, visual_card_name_scale);
IMPLEMENT_PROPERTY(GameMode, Rect2, visual_card_description_pos);
IMPLEMENT_PROPERTY(GameMode, Vector2, visual_card_description_scale);

IMPLEMENT_PROPERTY(GameMode, Rect2, visual_card_costs_pos);
IMPLEMENT_PROPERTY(GameMode, GameMode::GrowMode, visual_card_costs_grow_mode);
IMPLEMENT_PROPERTY(GameMode, bool, visual_card_costs_shrink_name);
IMPLEMENT_PROPERTY(GameMode, float, visual_card_costs_spacing);
IMPLEMENT_PROPERTY(GameMode, Vector2, visual_card_costs_text_scale);

IMPLEMENT_PROPERTY(GameMode, Rect2, visual_card_tribes_pos);
IMPLEMENT_PROPERTY(GameMode, GameMode::GrowMode, visual_card_tribes_grow_mode);
IMPLEMENT_PROPERTY(GameMode, bool, visual_card_tribes_shrink_name);
IMPLEMENT_PROPERTY(GameMode, float, visual_card_tribes_spacing);
IMPLEMENT_PROPERTY(GameMode, Vector2, visual_card_tribes_text_scale);

IMPLEMENT_PROPERTY(GameMode, TypedArray<StickerDef>, visual_card_stickers);

IMPLEMENT_PROPERTY(GameMode, int64_t, visual_effect_highlight_width);
IMPLEMENT_PROPERTY(GameMode, Color, visual_effect_highlight_color);
IMPLEMENT_PROPERTY(GameMode, Color, visual_description_background_color);

IMPLEMENT_PROPERTY(GameMode, TypedArray<AudienceDef>, audience);
IMPLEMENT_PROPERTY(GameMode, TypedArray<CharacterDef>, characters);

IMPLEMENT_PROPERTY(GameMode, Ref<VariantDef>, base_variant);
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
