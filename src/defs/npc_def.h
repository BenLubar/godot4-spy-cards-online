#ifndef NPC_DEF_H
#define NPC_DEF_H

#include "dry.h"

#include "enums/enums_character_def.h"
#include "enums/enums_npc_def.h"

class NPCDef;

#include "jigsaw/procedure/jigsaw_procedure.h"
#include "jigsaw/procedure/jigsaw_trigger.h"

class NPCDef : public Resource {
	GDCLASS(NPCDef, Resource);

public:
	using NPC = enums::NPCDef::NPC;

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(String, display_name);
	DECLARE_PROPERTY(enums::CharacterDef::Character, character, = enums::CharacterDef::Character::NONE);
	DECLARE_PROPERTY(TypedArray<JigsawTriggerNPC>, triggers);
	DECLARE_PROPERTY(Ref<JigsawProcedureNPCBuildDeck>, build_deck);
	DECLARE_PROPERTY(Ref<JigsawProcedureNPCDecideChoice>, decide_choice);
	DECLARE_PROPERTY(Ref<JigsawProcedureNPCDecideRealtime>, decide_realtime);

	DEFAULT_TO_STRING();
};
DECLARE_PREDEFINED_KEY(NPCDef, NPC);

#endif // NPC_DEF_H
