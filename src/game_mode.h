#ifndef GAME_MODE_H
#define GAME_MODE_H

#include "dry.h"

class GameMode;

#include "game_mode_summary.h"
#include "variant_def.h"
#include "audience_def.h"
#include "character_def.h"
#include "card_def.h"
#include "effect_def.h"
#include "rank_def.h"
#include "tribe_def.h"
#include "stat_def.h"
#include "modifier_def.h"
#include "choices_def.h"
#include "variable_def.h"
#include "jigsaw_reusable_command_list.h"

class GameMode : public GameModeSummary {
	GDCLASS(GameMode, GameModeSummary);

protected:
	static void _bind_methods();

public:
	GameMode() = default;
	~GameMode() = default;

	DECLARE_PROPERTY(int64_t, visual_effect_highlight_width, = 5);
	DECLARE_PROPERTY(Color, visual_effect_highlight_color, = Color("#44aaff"));
	DECLARE_PROPERTY(Color, visual_description_background_color, = Color(1, 1, 1, 1));
/*

enum GrowMode {
	GROW_LEFT,
	GROW_RIGHT,
	GROW_UP,
	GROW_DOWN,
	GROW_HORIZONTAL,
	GROW_VERTICAL,
	SHRINK_HORIZONTAL,
	SHRINK_VERTICAL,
}

const DEFAULT_VISUAL_PIXEL_SIZE := 75.0
const DEFAULT_VISUAL_BASE_TRANSFORM := Transform3D(Basis(Vector3(2.32, 0, 0), Vector3(0, 3.16, 0), Vector3.BACK), Vector3.ZERO)
const DEFAULT_VISUAL_PORTRAIT_TRANSFORM := Transform3D(Basis(Vector3(1.28, 0, 0), Vector3(0, 1.28, 0), Vector3.BACK), Vector3(0, 0.45, -0.05))
const DEFAULT_VISUAL_RANK_DECORATION_TRANSFORM := Transform3D(Basis(Vector3(0.533333, 0, 0), Vector3(0, 1.466667, 0), Vector3.BACK), Vector3(0.83, 0.44, 0))
const DEFAULT_VISUAL_SMOOTH_CORNERS := Vector3(174.0 / 5.0, 237.0 / 5.0, 1.0)
const DEFAULT_VISUAL_CARD_NAME_POS := Rect2(12, 9,146, 0)
const DEFAULT_VISUAL_CARD_NAME_SCALE := Vector2(1, 1)
const DEFAULT_VISUAL_CARD_DESCRIPTION_POS := Rect2(18, 140, 136, 76)
const DEFAULT_VISUAL_CARD_DESCRIPTION_SCALE := Vector2(1, 1.25)
const DEFAULT_VISUAL_CARD_COSTS_POS := Rect2(139, 4, 32, 32)
const DEFAULT_VISUAL_CARD_COSTS_GROW_MODE := GrowMode.GROW_LEFT
const DEFAULT_VISUAL_CARD_COSTS_SHRINK_NAME := true
const DEFAULT_VISUAL_CARD_COSTS_SPACING := 0.0
const DEFAULT_VISUAL_CARD_COSTS_TEXT_SCALE := Vector2(1.0, 1.2)
const DEFAULT_VISUAL_CARD_TRIBES_POS := Rect2(5, 217, 164, 15)
const DEFAULT_VISUAL_CARD_TRIBES_GROW_MODE := GrowMode.SHRINK_HORIZONTAL
const DEFAULT_VISUAL_CARD_TRIBES_SHRINK_NAME := false
const DEFAULT_VISUAL_CARD_TRIBES_SPACING := 0.0
const DEFAULT_VISUAL_CARD_TRIBES_TEXT_SCALE := Vector2(1.0, 1.0)
const DEFAULT_VISUAL_CARD_STICKERS: Array[StickerDef] = [
	preload("res://predefined/sticker/sticker_description.tres"),
	preload("res://predefined/sticker/sticker_card_name.tres"),
	preload("res://predefined/sticker/sticker_card_name_nocost.tres"),
]

@export var visual_pixel_size := DEFAULT_VISUAL_PIXEL_SIZE
@export var visual_base_transform := DEFAULT_VISUAL_BASE_TRANSFORM
@export var visual_portrait_transform := DEFAULT_VISUAL_PORTRAIT_TRANSFORM
@export var visual_rank_decoration_transform := DEFAULT_VISUAL_RANK_DECORATION_TRANSFORM
@export var visual_smooth_corners := DEFAULT_VISUAL_SMOOTH_CORNERS

@export var visual_card_front := IconDef.Icon.CARD_FRONT
@export var visual_card_front_window := IconDef.Icon.CARD_FRONT_WINDOW
@export var visual_tribe_bubble := IconDef.Icon.CARD_TRIBE_BUBBLE
@export var visual_tribe_bubble_wide := IconDef.Icon.CARD_TRIBE_BUBBLE_WIDE

@export var visual_card_name_pos := DEFAULT_VISUAL_CARD_NAME_POS
@export var visual_card_name_scale := DEFAULT_VISUAL_CARD_NAME_SCALE
@export var visual_card_description_pos := DEFAULT_VISUAL_CARD_DESCRIPTION_POS
@export var visual_card_description_scale := DEFAULT_VISUAL_CARD_DESCRIPTION_SCALE
@export var visual_card_costs_pos := DEFAULT_VISUAL_CARD_COSTS_POS
@export var visual_card_costs_shrink_name := DEFAULT_VISUAL_CARD_COSTS_SHRINK_NAME
@export var visual_card_costs_grow_mode := DEFAULT_VISUAL_CARD_COSTS_GROW_MODE
@export var visual_card_costs_spacing := DEFAULT_VISUAL_CARD_COSTS_SPACING
@export var visual_card_costs_text_scale := DEFAULT_VISUAL_CARD_COSTS_TEXT_SCALE
@export var visual_card_tribes_pos := DEFAULT_VISUAL_CARD_TRIBES_POS
@export var visual_card_tribes_shrink_name := DEFAULT_VISUAL_CARD_TRIBES_SHRINK_NAME
@export var visual_card_tribes_grow_mode := DEFAULT_VISUAL_CARD_TRIBES_GROW_MODE
@export var visual_card_tribes_spacing := DEFAULT_VISUAL_CARD_TRIBES_SPACING
@export var visual_card_tribes_text_scale := DEFAULT_VISUAL_CARD_TRIBES_TEXT_SCALE
@export var visual_card_stickers: Array[StickerDef] = DEFAULT_VISUAL_CARD_STICKERS.duplicate(true)

*/

	DECLARE_PROPERTY(TypedArray<AudienceDef>, audience);
	DECLARE_PROPERTY(TypedArray<CharacterDef>, characters);

	DECLARE_PROPERTY(Ref<VariantDef>, base_variant);
	DECLARE_PROPERTY(TypedArray<VariantDef>, variants);

	DECLARE_PROPERTY(TypedArray<RankDef::Rank>, ranks);
	DECLARE_PROPERTY(TypedArray<TribeDef::Tribe>, tribes);
	DECLARE_PROPERTY(TypedArray<StatDef::Stat>, stats);
	DECLARE_PROPERTY(TypedArray<ModifierDef::Modifier>, modifiers);
	DECLARE_PROPERTY(TypedArray<EffectDef::Effect>, effects);
	DECLARE_PROPERTY(TypedArray<NPCDef::NPC>, npcs);

	DECLARE_PROPERTY(TypedArray<RankDef>, custom_ranks);
	DECLARE_PROPERTY(TypedArray<TribeDef>, custom_tribes);
	DECLARE_PROPERTY(TypedArray<StatDef>, custom_stats);
	DECLARE_PROPERTY(TypedArray<ModifierDef>, custom_modifiers);
	DECLARE_PROPERTY(TypedArray<EffectDef>, custom_effects);
	DECLARE_PROPERTY(TypedArray<NPCDef>, custom_npcs);

	DECLARE_PROPERTY(TypedArray<VariableDef>, custom_variables);
	DECLARE_PROPERTY(TypedArray<ChoicesDef>, custom_choices);
	DECLARE_PROPERTY(TypedArray<JigsawReusableCommandList>, custom_functions);

	DECLARE_PROPERTY(TypedArray<CardDef>, card_defs);

	Ref<CardDef> get_card(CardDef::Card id) const;
	Ref<RankDef> get_rank(RankDef::Rank id) const;
	Ref<TribeDef> get_tribe(TribeDef::Tribe id) const;
	Ref<StatDef> get_stat(StatDef::Stat id) const;
	Ref<ModifierDef> get_modifier(ModifierDef::Modifier id) const;
	Ref<EffectDef> get_effect(EffectDef::Effect id) const;
	Ref<NPCDef> get_npc(NPCDef::NPC id) const;
};

#endif // GAME_MODE_H
