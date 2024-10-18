#ifndef GAME_MODE_H
#define GAME_MODE_H

#include "game_mode_summary.h"

#include "enums_icon_def.h"

class GameMode;

#include "audience_def.h"
#include "card_def.h"
#include "character_def.h"
#include "choices_def.h"
#include "effect_def.h"
#include "jigsaw_function.h"
#include "location_def.h"
#include "modifier_def.h"
#include "npc_def.h"
#include "rank_def.h"
#include "stat_def.h"
#include "sticker_def.h"
#include "tribe_def.h"
#include "variable_def.h"
#include "variant_def.h"

class GameMode : public GameModeSummary {
	GDCLASS(GameMode, GameModeSummary);

public:
	enum GrowMode {
		GROW_LEFT = 0,
		GROW_RIGHT = 1,
		GROW_UP = 2,
		GROW_DOWN = 3,
		GROW_HORIZONTAL = 4,
		GROW_VERTICAL = 5,
		SHRINK_HORIZONTAL = 6,
		SHRINK_VERTICAL = 7,
	};

protected:
	static void _bind_methods();

public:
	GameMode() = default;
	~GameMode() = default;

	DECLARE_PROPERTY(float, visual_pixel_size, = 75.0f);
	DECLARE_PROPERTY(Transform3D, visual_base_transform, = Transform3D(Basis(Vector3(2.32f, 0.0f, 0.0f), Vector3(0.0f, 3.16f, 0.0f), Vector3(0.0f, 0.0f, 1.0f)), Vector3(0.0f, 0.0f, 0.0f)));
	DECLARE_PROPERTY(Transform3D, visual_portrait_transform, = Transform3D(Basis(Vector3(1.28f, 0.0f, 0.0f), Vector3(0.0f, 1.28f, 0.0f), Vector3(0.0f, 0.0f, 1.0f)), Vector3(0.0f, 0.45f, -0.05f)));
	DECLARE_PROPERTY(Transform3D, visual_rank_decoration_transform, = Transform3D(Basis(Vector3(0.533333f, 0.0f, 0.0f), Vector3(0.0f, 1.466667f, 0.0f), Vector3(0.0f, 0.0f, 1.0f)), Vector3(0.83f, 0.44f, 0.0f)));
	DECLARE_PROPERTY(Vector3, visual_smooth_corners, = Vector3(174.0f / 5.0f, 237.0f / 5.0f, 1.0f));

	DECLARE_PROPERTY(enums::IconDef::Icon, visual_card_front, = enums::IconDef::Icon::CARD_FRONT);
	DECLARE_PROPERTY(enums::IconDef::Icon, visual_card_front_window, = enums::IconDef::Icon::CARD_FRONT_WINDOW);
	DECLARE_PROPERTY(enums::IconDef::Icon, visual_tribe_bubble, = enums::IconDef::Icon::CARD_TRIBE_BUBBLE);
	DECLARE_PROPERTY(enums::IconDef::Icon, visual_tribe_bubble_wide, = enums::IconDef::Icon::CARD_TRIBE_BUBBLE_WIDE);

	DECLARE_PROPERTY(Rect2, visual_card_name_pos, = Rect2(12, 9, 146, 0));
	DECLARE_PROPERTY(Vector2, visual_card_name_scale, = Vector2(1.0f, 1.0f));
	DECLARE_PROPERTY(Rect2, visual_card_description_pos, = Rect2(18, 140, 136, 76));
	DECLARE_PROPERTY(Vector2, visual_card_description_scale, = Vector2(1.0f, 1.25f));

	DECLARE_PROPERTY(Rect2, visual_card_costs_pos, = Rect2(139, 4, 32, 32));
	DECLARE_PROPERTY(GrowMode, visual_card_costs_grow_mode, = GrowMode::GROW_LEFT);
	DECLARE_PROPERTY(bool, visual_card_costs_shrink_name, = true);
	DECLARE_PROPERTY(float, visual_card_costs_spacing, = 0.0f);
	DECLARE_PROPERTY(Vector2, visual_card_costs_text_scale, = Vector2(1.0f, 1.2f));

	DECLARE_PROPERTY(Rect2, visual_card_tribes_pos, = Rect2(5, 217, 164, 15));
	DECLARE_PROPERTY(GrowMode, visual_card_tribes_grow_mode, = GrowMode::SHRINK_HORIZONTAL);
	DECLARE_PROPERTY(bool, visual_card_tribes_shrink_name, = false);
	DECLARE_PROPERTY(float, visual_card_tribes_spacing, = 0.0f);
	DECLARE_PROPERTY(Vector2, visual_card_tribes_text_scale, = Vector2(1.0f, 1.0f));

	DECLARE_PROPERTY(TypedArray<StickerDef>, visual_card_stickers);

	DECLARE_PROPERTY(int64_t, visual_effect_highlight_width, = 5);
	DECLARE_PROPERTY(Color, visual_effect_highlight_color, = Color("#44aaff"));
	DECLARE_PROPERTY(Color, visual_description_background_color, = Color(1, 1, 1, 1));

	DECLARE_PROPERTY(TypedArray<AudienceDef>, audience);
	DECLARE_PROPERTY(TypedArray<CharacterDef>, characters);

	DECLARE_PROPERTY(Ref<VariantDef>, base_variant);
	DECLARE_PROPERTY(TypedArray<VariantDef>, variants);

	DECLARE_PROPERTY(TypedArray<enums::RankDef::Rank>, ranks);
	DECLARE_PROPERTY(TypedArray<enums::TribeDef::Tribe>, tribes);
	DECLARE_PROPERTY(TypedArray<enums::StatDef::Stat>, stats);
	DECLARE_PROPERTY(TypedArray<enums::ModifierDef::Modifier>, modifiers);
	DECLARE_PROPERTY(TypedArray<enums::EffectDef::Effect>, effects);
	DECLARE_PROPERTY(TypedArray<enums::NPCDef::NPC>, npcs);

	DECLARE_PROPERTY(TypedArray<RankDef>, custom_ranks);
	DECLARE_PROPERTY(TypedArray<TribeDef>, custom_tribes);
	DECLARE_PROPERTY(TypedArray<StatDef>, custom_stats);
	DECLARE_PROPERTY(TypedArray<ModifierDef>, custom_modifiers);
	DECLARE_PROPERTY(TypedArray<EffectDef>, custom_effects);
	DECLARE_PROPERTY(TypedArray<NPCDef>, custom_npcs);

	DECLARE_PROPERTY(TypedArray<LocationDef>, custom_locations);
	DECLARE_PROPERTY(TypedArray<VariableDef>, custom_variables);
	DECLARE_PROPERTY(TypedArray<ChoicesDef>, custom_choices);
	DECLARE_PROPERTY(TypedArray<JigsawFunction>, custom_functions);

	DECLARE_PROPERTY(TypedArray<CardDef>, card_defs);

	Ref<CardDef> get_card(enums::CardDef::Card id) const;
	Ref<RankDef> get_rank(enums::RankDef::Rank id) const;
	Ref<TribeDef> get_tribe(enums::TribeDef::Tribe id) const;
	Ref<StatDef> get_stat(enums::StatDef::Stat id) const;
	Ref<ModifierDef> get_modifier(enums::ModifierDef::Modifier id) const;
	Ref<EffectDef> get_effect(enums::EffectDef::Effect id) const;
	Ref<NPCDef> get_npc(enums::NPCDef::NPC id) const;
};
DECLARE_ENUM(GameMode::GrowMode);

#endif // GAME_MODE_H
