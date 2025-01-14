#ifndef GAME_MODE_H
#define GAME_MODE_H

#include "defs/game_mode_summary.h"

#include "enums/enums_icon_def.h"

class GameMode;

#include "defs/audience_def.h"
#include "defs/card_def.h"
#include "defs/card_design.h"
#include "defs/character_def.h"
#include "defs/choices_def.h"
#include "defs/effect_def.h"
#include "jigsaw/procedure/jigsaw_function.h"
#include "defs/location_def.h"
#include "defs/modifier_def.h"
#include "defs/npc_def.h"
#include "defs/rank_def.h"
#include "defs/stat_def.h"
#include "defs/sticker_def.h"
#include "defs/tribe_def.h"
#include "defs/variable_def.h"
#include "defs/variant_def.h"

class GameMode : public GameModeSummary {
	GDCLASS(GameMode, GameModeSummary);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(TypedArray<AudienceDef>, audience);
	DECLARE_PROPERTY(TypedArray<CharacterDef>, characters);

	DECLARE_PROPERTY(Ref<CardDesign>, default_card_design);
	DECLARE_PROPERTY(TypedArray<JigsawTriggerVariant>, base_triggers);
	DECLARE_PROPERTY(TypedArray<VariantDef>, variants);

	DECLARE_PROPERTY(PackedArray<enums::RankDef::Rank>, ranks);
	DECLARE_PROPERTY(PackedArray<enums::TribeDef::Tribe>, tribes);
	DECLARE_PROPERTY(PackedArray<enums::StatDef::Stat>, stats);
	DECLARE_PROPERTY(PackedArray<enums::ModifierDef::Modifier>, modifiers);
	DECLARE_PROPERTY(PackedArray<enums::EffectDef::Effect>, effects);
	DECLARE_PROPERTY(PackedArray<enums::NPCDef::NPC>, npcs);

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
	Ref<CharacterDef> get_character(enums::CharacterDef::Character id) const;
	Ref<VariableDef> get_variable(enums::VariableDef::Variable id) const;
	Ref<LocationDef> get_location(enums::LocationDef::Location id) const;
};

#endif // GAME_MODE_H
