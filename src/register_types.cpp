#include "register_types.h"

#include "format_helper.h"
#include "rng.h"

#include "jigsaw_command_list.h"
#include "jigsaw_reusable_command_list.h"
#include "jigsaw_trigger.h"

#include "jigsaw_command.h"

#include "jigsaw_parameter.h"

#include "audience_def.h"
#include "card_def.h"
#include "card_filter.h"
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
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void initialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	GDREGISTER_CLASS(FormatHelper);
	GDREGISTER_CLASS(RNG);

	GDREGISTER_CLASS(JigsawCommandList);
	GDREGISTER_CLASS(JigsawReusableCommandList);
	GDREGISTER_CLASS(JigsawTrigger);

	GDREGISTER_ABSTRACT_CLASS(JigsawCommand);

	GDREGISTER_ABSTRACT_CLASS(JigsawParameter);

	GDREGISTER_CLASS(AudienceDef);
	GDREGISTER_CLASS(CardDef);
	GDREGISTER_CLASS(CardFilter);
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
