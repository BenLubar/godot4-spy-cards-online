#include "format_helper.h"
#include "rng.h"
#include "base32.h"

#include "formatted_text.h"
#include "squish_label.h"
#include "outline_current_effect.h"
#include "rich_text_rainbow_effect.h"
#include "rich_text_shake_effect.h"
#include "rich_text_glitch_effect.h"

#include "jigsaw_command_list.h"
#include "jigsaw_reusable_command_list.h"
#include "jigsaw_trigger.h"
#include "jigsaw_global.h"

#include "jigsaw_command.h"

#include "jigsaw_parameter.h"
#include "jigsaw_parameter_sprite_instance.h"
#include "jigsaw_parameter_scene_instance.h"
#include "jigsaw_parameter_queued_effect.h"
#include "jigsaw_parameter_modifier_instance.h"
#include "jigsaw_parameter_card_instance.h"
#include "jigsaw_parameter_formatted_text.h"
#include "jigsaw_parameter_ordered_list.h"
#include "jigsaw_parameter_local_variable.h"
#include "jigsaw_parameter_variable.h"
#include "jigsaw_parameter_boolean.h"
#include "jigsaw_parameter_amount.h"
#include "jigsaw_parameter_float.h"
#include "jigsaw_parameter_string.h"
#include "jigsaw_parameter_effect_instance.h"
#include "jigsaw_parameter_card_filter.h"
#include "jigsaw_parameter_card.h"
#include "jigsaw_parameter_stat.h"
#include "jigsaw_parameter_rank.h"
#include "jigsaw_parameter_tribe.h"
#include "jigsaw_parameter_modifier.h"
#include "jigsaw_parameter_location.h"
#include "jigsaw_parameter_icon.h"
#include "jigsaw_parameter_audio.h"
#include "jigsaw_parameter_cid_opus.h"
#include "jigsaw_parameter_file_id_opus.h"
#include "jigsaw_parameter_scene.h"
#include "jigsaw_parameter_file_id_gltf.h"
#include "jigsaw_parameter_color.h"
#include "jigsaw_parameter_choice.h"

#include "card_filter.h"
#include "card_filter_and.h"
#include "card_filter_or.h"
#include "card_filter_card.h"
#include "card_filter_rank.h"
#include "card_filter_tribe.h"
#include "card_filter_cost.h"
#include "card_filter_param.h"

#include "audience_def.h"
#include "card_def.h"
#include "character_def.h"
#include "choices_def.h"
#include "effect_def.h"
#include "effect_instance.h"
#include "icon_def.h"
#include "location_def.h"
#include "modifier_def.h"
#include "npc_def.h"
#include "rank_def.h"
#include "stat_def.h"
#include "stat_value.h"
#include "sticker_def.h"
#include "tribe_def.h"
#include "variable_def.h"
#include "variant_def.h"

#include "deck.h"
#include "game_mode_summary.h"
#include "game_mode.h"
#include "recording.h"
#include "recording_round.h"

#include "card_instance.h"
#include "modifier_instance.h"
#include "queued_effect.h"

#include <gdextension_interface.h>
#include <godot_cpp/godot.hpp>

void initialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	GDREGISTER_CLASS(FormatHelper);
	GDREGISTER_CLASS(RNG);
	GDREGISTER_CLASS(Base32);

	GDREGISTER_CLASS(FormattedText);
	GDREGISTER_CLASS(SquishLabel);
	GDREGISTER_CLASS(OutlineCurrentEffect);
	GDREGISTER_CLASS(RichTextRainbowEffect);
	GDREGISTER_CLASS(RichTextShakeEffect);
	GDREGISTER_CLASS(RichTextGlitchEffect);

	GDREGISTER_CLASS(JigsawCommandList);
	GDREGISTER_CLASS(JigsawReusableCommandList);
	GDREGISTER_CLASS(JigsawTrigger);
	GDREGISTER_CLASS(JigsawGlobal);

	GDREGISTER_ABSTRACT_CLASS(JigsawCommand);

	GDREGISTER_ABSTRACT_CLASS(JigsawParameter);
	GDREGISTER_CLASS(JigsawParameterVariable); // variable first so the type property's default value gets recorded as 0 in the docs
	GDREGISTER_CLASS(JigsawParameterSpriteInstance);
	GDREGISTER_CLASS(JigsawParameterSceneInstance);
	GDREGISTER_CLASS(JigsawParameterQueuedEffect);
	GDREGISTER_CLASS(JigsawParameterModifierInstance);
	GDREGISTER_CLASS(JigsawParameterCardInstance);
	GDREGISTER_CLASS(JigsawParameterFormattedText);
	GDREGISTER_CLASS(JigsawParameterOrderedList);
	GDREGISTER_CLASS(JigsawParameterLocalVariable);
	GDREGISTER_CLASS(JigsawParameterBoolean);
	GDREGISTER_CLASS(JigsawParameterAmount);
	GDREGISTER_CLASS(JigsawParameterFloat);
	GDREGISTER_CLASS(JigsawParameterString);
	GDREGISTER_CLASS(JigsawParameterEffectInstance);
	GDREGISTER_CLASS(JigsawParameterCardFilter);
	GDREGISTER_CLASS(JigsawParameterCard);
	GDREGISTER_CLASS(JigsawParameterStat);
	GDREGISTER_CLASS(JigsawParameterRank);
	GDREGISTER_CLASS(JigsawParameterTribe);
	GDREGISTER_CLASS(JigsawParameterModifier);
	GDREGISTER_CLASS(JigsawParameterLocation);
	GDREGISTER_CLASS(JigsawParameterIcon);
	GDREGISTER_ABSTRACT_CLASS(JigsawParameterAudio);
	GDREGISTER_CLASS(JigsawParameterCIDOpus);
	GDREGISTER_CLASS(JigsawParameterFileIDOpus);
	GDREGISTER_ABSTRACT_CLASS(JigsawParameterScene);
	GDREGISTER_CLASS(JigsawParameterFileIDGLTF);
	GDREGISTER_CLASS(JigsawParameterColor);
	GDREGISTER_CLASS(JigsawParameterChoice);

	GDREGISTER_ABSTRACT_CLASS(CardFilter);
	GDREGISTER_CLASS(CardFilterAnd);
	GDREGISTER_CLASS(CardFilterOr);
	GDREGISTER_CLASS(CardFilterCard);
	GDREGISTER_CLASS(CardFilterRank);
	GDREGISTER_CLASS(CardFilterTribe);
	GDREGISTER_CLASS(CardFilterCost);
	GDREGISTER_CLASS(CardFilterParam);

	GDREGISTER_CLASS(AudienceDef);
	GDREGISTER_CLASS(CardDef);
	GDREGISTER_CLASS(CharacterDef);
	GDREGISTER_CLASS(ChoicesDef);
	GDREGISTER_CLASS(EffectDef);
	GDREGISTER_CLASS(EffectInstance);
	GDREGISTER_CLASS(IconDef);
	GDREGISTER_CLASS(LocationDef);
	GDREGISTER_CLASS(ModifierDef);
	GDREGISTER_CLASS(NPCDef);
	GDREGISTER_CLASS(RankDef);
	GDREGISTER_CLASS(StatDef);
	GDREGISTER_CLASS(StatValue);
	GDREGISTER_CLASS(StickerDef);
	GDREGISTER_CLASS(TribeDef);
	GDREGISTER_CLASS(VariableDef);
	GDREGISTER_CLASS(VariantDef);

	GDREGISTER_CLASS(Deck);
	GDREGISTER_CLASS(GameModeSummary);
	GDREGISTER_CLASS(GameMode);
	GDREGISTER_CLASS(Recording);
	GDREGISTER_CLASS(RecordingRound);

	GDREGISTER_CLASS(CardInstance);
	GDREGISTER_CLASS(ModifierInstance);
	GDREGISTER_CLASS(QueuedEffect);
}

void uninitialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

// Initialization
extern "C" GDExtensionBool GDE_EXPORT spycardsonlinenative_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
	init_obj.register_initializer(initialize_gdextension_types);
	init_obj.register_terminator(uninitialize_gdextension_types);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
