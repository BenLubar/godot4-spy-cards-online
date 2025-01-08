#include "util/format_helper.h"
#include "util/rng.h"
#include "util/base32.h"
#include "util/why_isnt_this_in_godot.h"
#include "node/single_use_audio_stream_player.h"

#include "defs/formatted_text.h"
#include "node/squish_label.h"
#include "node/text/outline_current_effect.h"
#include "node/text/rich_text_rainbow_effect.h"
#include "node/text/rich_text_shake_effect.h"
#include "node/text/rich_text_glitch_effect.h"

#include "jigsaw/procedure/jigsaw_command_list.h"
#include "jigsaw/jigsaw_global.h"
#include "jigsaw/jigsaw_visual.h"
#include "jigsaw/jigsaw_side.h"
#include "jigsaw/jigsaw_side_location.h"
#include "jigsaw/jigsaw_context.h"
#include "jigsaw/jigsaw_stack_frame.h"
#include "jigsaw/jigsaw_error.h"

#include "jigsaw/procedure/jigsaw_procedure.h"
#include "jigsaw/procedure/jigsaw_trigger.h"
#include "jigsaw/procedure/jigsaw_function.h"

#include "jigsaw/command/jigsaw_command.h"
#include "jigsaw/command/jigsaw_command_crash.h"
#include "jigsaw/command/jigsaw_command_comment.h"
#include "jigsaw/command/jigsaw_command_function.h"
#include "jigsaw/command/jigsaw_command_math.h"
#include "jigsaw/command/jigsaw_command_log.h"
#include "jigsaw/command/jigsaw_command_format_text.h"
#include "jigsaw/command/jigsaw_command_if.h"
#include "jigsaw/command/jigsaw_command_set_variable.h"
#include "jigsaw/command/jigsaw_command_lookup_definition_property.h"
#include "jigsaw/command/jigsaw_command_is_same.h"
#include "jigsaw/command/jigsaw_command_ordered_list.h"
#include "jigsaw/command/jigsaw_command_rng.h"
#include "jigsaw/command/jigsaw_command_audience.h"
#include "jigsaw/command/jigsaw_command_character.h"
#include "jigsaw/command/jigsaw_command_audio.h"
#include "jigsaw/command/jigsaw_command_scene.h"
#include "jigsaw/command/jigsaw_command_sprite.h"
#include "jigsaw/command/jigsaw_command_camera.h"
#include "jigsaw/command/jigsaw_command_ui.h"
#include "jigsaw/command/jigsaw_command_loop.h"
#include "jigsaw/command/jigsaw_command_color.h"

#include "jigsaw/parameter/jigsaw_parameter.h"
#include "jigsaw/parameter/jigsaw_parameter_card_grid.h"
#include "jigsaw/parameter/jigsaw_parameter_ui_label_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_ui_icon_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_audience_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_character_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_sprite_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_scene_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_queued_effect.h"
#include "jigsaw/parameter/jigsaw_parameter_modifier_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_card_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_formatted_text.h"
#include "jigsaw/parameter/jigsaw_parameter_variable.h"
#include "jigsaw/parameter/jigsaw_parameter_local_variable.h"
#include "jigsaw/parameter/jigsaw_parameter_ordered_list.h"
#include "jigsaw/parameter/jigsaw_parameter_boolean.h"
#include "jigsaw/parameter/jigsaw_parameter_amount.h"
#include "jigsaw/parameter/jigsaw_parameter_float.h"
#include "jigsaw/parameter/jigsaw_parameter_string.h"
#include "jigsaw/parameter/jigsaw_parameter_effect_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_card_filter.h"
#include "jigsaw/parameter/jigsaw_parameter_card.h"
#include "jigsaw/parameter/jigsaw_parameter_stat.h"
#include "jigsaw/parameter/jigsaw_parameter_rank.h"
#include "jigsaw/parameter/jigsaw_parameter_tribe.h"
#include "jigsaw/parameter/jigsaw_parameter_modifier.h"
#include "jigsaw/parameter/jigsaw_parameter_location.h"
#include "jigsaw/parameter/jigsaw_parameter_icon.h"
#include "jigsaw/parameter/jigsaw_parameter_audio.h"
#include "jigsaw/parameter/jigsaw_parameter_cid_opus.h"
#include "jigsaw/parameter/jigsaw_parameter_file_id_opus.h"
#include "jigsaw/parameter/jigsaw_parameter_scene.h"
#include "jigsaw/parameter/jigsaw_parameter_file_id_gltf.h"
#include "jigsaw/parameter/jigsaw_parameter_color.h"
#include "jigsaw/parameter/jigsaw_parameter_choice.h"
#include "jigsaw/parameter/jigsaw_parameter_character.h"
#include "jigsaw/parameter/jigsaw_parameter_audience.h"
#include "jigsaw/parameter/jigsaw_parameter_effect_instance_parameter.h"
#include "jigsaw/parameter/jigsaw_parameter_stat_value.h"

#include "defs/card_filter.h"
#include "defs/card_filter_and.h"
#include "defs/card_filter_or.h"
#include "defs/card_filter_card.h"
#include "defs/card_filter_rank.h"
#include "defs/card_filter_tribe.h"
#include "defs/card_filter_cost.h"
#include "defs/card_filter_param.h"

#include "defs/audience_def.h"
#include "defs/card_def.h"
#include "defs/card_design.h"
#include "defs/character_def.h"
#include "defs/choices_def.h"
#include "defs/effect_def.h"
#include "defs/effect_instance.h"
#include "defs/icon_def.h"
#include "defs/location_def.h"
#include "defs/modifier_def.h"
#include "defs/npc_def.h"
#include "defs/numeric_value.h"
#include "defs/rank_def.h"
#include "defs/stat_def.h"
#include "defs/stat_value.h"
#include "defs/sticker_def.h"
#include "defs/tribe_def.h"
#include "defs/variable_def.h"
#include "defs/variant_def.h"

#include "active/card_instance.h"
#include "active/modifier_instance.h"
#include "active/queued_effect.h"
#include "active/audience.h"
#include "node/card_grid.h"

#include "protocol/data_container.h"
#include "protocol/embedded_data_file.h"
#include "protocol/recording_player_data.h"
#include "protocol/recording_round_data.h"
#include "protocol/recording_round_player_data.h"
#include "protocol/matchmaking_handler.h"
#include "protocol/matchmaking_connection.h"
#include "defs/game_mode_summary.h"
#include "defs/game_mode.h"
#include "protocol/recording.h"
#include "protocol/recording_round.h"
#include "protocol/button_input_history.h"
#include "legacy/legacy_parse.h"
#include "protocol/deck.h"

#include <gdextension_interface.h>
#include <godot_cpp/godot.hpp>

void initialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	GDREGISTER_CLASS(FormatHelper);
	GDREGISTER_CLASS(RNG);
	GDREGISTER_CLASS(Base32);
	GDREGISTER_ABSTRACT_CLASS(WhyIsntThisInGodot);
	GDREGISTER_CLASS(SingleUseAudioStreamPlayer);
	GDREGISTER_CLASS(SingleUseAudioStreamPlayer2D);
	GDREGISTER_CLASS(SingleUseAudioStreamPlayer3D);

	GDREGISTER_CLASS(FormattedText);
	GDREGISTER_CLASS(FormattedTextWithIcon);
	GDREGISTER_CLASS(SquishLabel);
	GDREGISTER_CLASS(OutlineCurrentEffect);
	GDREGISTER_CLASS(RichTextRainbowEffect);
	GDREGISTER_CLASS(RichTextShakeEffect);
	GDREGISTER_CLASS(RichTextGlitchEffect);

	GDREGISTER_CLASS(JigsawCommandList);
	GDREGISTER_CLASS(JigsawGlobal);
	GDREGISTER_CLASS(JigsawVisual);
	GDREGISTER_CLASS(JigsawSide);
	GDREGISTER_CLASS(JigsawSideLocation);
	GDREGISTER_CLASS(JigsawContext);
	GDREGISTER_CLASS(JigsawStackFrame);
	GDREGISTER_CLASS(JigsawError);

	GDREGISTER_ABSTRACT_CLASS(JigsawProcedure);
	GDREGISTER_CLASS(JigsawProcedureEffectCurve);
	GDREGISTER_CLASS(JigsawProcedureEffectDescribe);
	GDREGISTER_CLASS(JigsawProcedureEffectSimpleDescribe);
	GDREGISTER_CLASS(JigsawProcedureEffectExtendedDescribe);
	GDREGISTER_CLASS(JigsawProcedureModifierDescribe);
	GDREGISTER_CLASS(JigsawProcedureModifierSimpleDescribe);
	GDREGISTER_CLASS(JigsawProcedureModifierExtendedDescribe);
	GDREGISTER_CLASS(JigsawProcedureNPCBuildDeck);
	GDREGISTER_CLASS(JigsawProcedureNPCDecideChoice);
	GDREGISTER_CLASS(JigsawProcedureNPCDecideRealtime);
	GDREGISTER_CLASS(JigsawProcedureStickerShouldShow);
	GDREGISTER_CLASS(JigsawProcedureStatFormatCost);
	GDREGISTER_CLASS(JigsawProcedureVariantMain);
	GDREGISTER_ABSTRACT_CLASS(JigsawTrigger);
	GDREGISTER_ABSTRACT_CLASS(JigsawTriggerEffect);
	GDREGISTER_CLASS(JigsawTriggerEffectCustom);
	GDREGISTER_ABSTRACT_CLASS(JigsawTriggerModifier);
	GDREGISTER_CLASS(JigsawTriggerModifierCustom);
	GDREGISTER_ABSTRACT_CLASS(JigsawTriggerNPC);
	GDREGISTER_CLASS(JigsawTriggerNPCCustom);
	GDREGISTER_ABSTRACT_CLASS(JigsawTriggerVariant);
	GDREGISTER_CLASS(JigsawTriggerVariantCustom);
	GDREGISTER_CLASS(JigsawTriggerVariantCosmeticInit);
	GDREGISTER_CLASS(JigsawTriggerVariantCharacterInit);
	GDREGISTER_CLASS(JigsawTriggerVariantDeckInit);
	GDREGISTER_CLASS(JigsawTriggerVariantMatchStart);
	GDREGISTER_CLASS(JigsawFunction);

	GDREGISTER_ABSTRACT_CLASS(JigsawCommand);
	GDREGISTER_CLASS(JigsawCommandCrash);
	GDREGISTER_CLASS(JigsawCommandComment);
	GDREGISTER_CLASS(JigsawCommandFunction);
	GDREGISTER_CLASS(JigsawCommandMath);
	GDREGISTER_CLASS(JigsawCommandLog);
	GDREGISTER_CLASS(JigsawCommandFormatText);
	GDREGISTER_CLASS(JigsawCommandIf);
	GDREGISTER_CLASS(JigsawCommandSetVariable);
	GDREGISTER_CLASS(JigsawCommandLookupDefinitionProperty);
	GDREGISTER_CLASS(JigsawCommandIsSame);
	GDREGISTER_CLASS(JigsawCommandOrderedList);
	GDREGISTER_CLASS(JigsawCommandRNG);
	GDREGISTER_CLASS(JigsawCommandAudience);
	GDREGISTER_CLASS(JigsawCommandCharacter);
	GDREGISTER_CLASS(JigsawCommandAudio);
	GDREGISTER_CLASS(JigsawCommandScene);
	GDREGISTER_CLASS(JigsawCommandSprite);
	GDREGISTER_CLASS(JigsawCommandCamera);
	GDREGISTER_CLASS(JigsawCommandUI);
	GDREGISTER_CLASS(JigsawCommandLoop);
	GDREGISTER_CLASS(JigsawCommandColor);

	GDREGISTER_ABSTRACT_CLASS(JigsawParameter);
	GDREGISTER_CLASS(JigsawParameterVariable); // variable first so the type property's default value gets recorded as 0 in the docs
	GDREGISTER_CLASS(JigsawParameterCardGrid);
	GDREGISTER_CLASS(JigsawParameterUILabelInstance);
	GDREGISTER_CLASS(JigsawParameterUIIconInstance);
	GDREGISTER_CLASS(JigsawParameterCharacterInstance);
	GDREGISTER_CLASS(JigsawParameterAudienceInstance);
	GDREGISTER_CLASS(JigsawParameterSpriteInstance);
	GDREGISTER_CLASS(JigsawParameterSceneInstance);
	GDREGISTER_CLASS(JigsawParameterQueuedEffect);
	GDREGISTER_CLASS(JigsawParameterModifierInstance);
	GDREGISTER_CLASS(JigsawParameterCardInstance);
	GDREGISTER_CLASS(JigsawParameterFormattedText);
	GDREGISTER_CLASS(JigsawParameterLocalVariable);
	GDREGISTER_CLASS(JigsawParameterOrderedList);
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
	GDREGISTER_CLASS(JigsawParameterCharacter);
	GDREGISTER_CLASS(JigsawParameterAudience);
	GDREGISTER_CLASS(JigsawParameterEffectInstanceParameter);
	GDREGISTER_CLASS(JigsawParameterStatValue);

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
	GDREGISTER_CLASS(CardDesign);
	GDREGISTER_CLASS(CharacterDef);
	GDREGISTER_CLASS(ChoicesDef);
	GDREGISTER_CLASS(EffectDef);
	GDREGISTER_CLASS(EffectInstance);
	GDREGISTER_CLASS(IconDef);
	GDREGISTER_CLASS(LocationDef);
	GDREGISTER_CLASS(ModifierDef);
	GDREGISTER_CLASS(NPCDef);
	GDREGISTER_CLASS(NumericValue);
	GDREGISTER_CLASS(RankDef);
	GDREGISTER_CLASS(StatDef);
	GDREGISTER_CLASS(StatValue);
	GDREGISTER_CLASS(StickerDef);
	GDREGISTER_CLASS(TribeDef);
	GDREGISTER_CLASS(VariableDef);
	GDREGISTER_CLASS(VariantDef);

	GDREGISTER_CLASS(CardInstance);
	GDREGISTER_CLASS(ModifierInstance);
	GDREGISTER_CLASS(QueuedEffect);
	GDREGISTER_CLASS(Audience);
	GDREGISTER_CLASS(CardGrid2D);
	GDREGISTER_CLASS(CardGrid3D);

	GDREGISTER_CLASS(DataContainer);
	GDREGISTER_CLASS(EmbeddedDataFile);
	GDREGISTER_CLASS(RecordingPlayerData);
	GDREGISTER_CLASS(RecordingRoundData);
	GDREGISTER_CLASS(RecordingRoundPlayerData);
	GDREGISTER_CLASS(MatchmakingHandler);
	GDREGISTER_CLASS(MatchmakingConnection);
	GDREGISTER_CLASS(GameModeSummary);
	GDREGISTER_CLASS(GameMode);
	GDREGISTER_CLASS(Recording);
	GDREGISTER_CLASS(RecordingRound);
	GDREGISTER_ABSTRACT_CLASS(ButtonInputHistory);
	GDREGISTER_ABSTRACT_CLASS(LegacyParse);
	GDREGISTER_ABSTRACT_CLASS(Deck);
}

Vector<std::function<void()>> _free_lazy_globals;
void uninitialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	for (int64_t i = 0; i < _free_lazy_globals.size(); i++) {
		_free_lazy_globals[i]();
	}
	_free_lazy_globals.clear();
}

// Initialization
extern "C" GDExtensionBool GDE_EXPORT spycardsonlinenative_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
	init_obj.register_initializer(initialize_gdextension_types);
	init_obj.register_terminator(uninitialize_gdextension_types);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
