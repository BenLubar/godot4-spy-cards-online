#ifndef NPC_DEF_H
#define NPC_DEF_H

#include "dry.h"

#include "enums/enums_character_def.h"
#include "enums/enums_npc_def.h"

class NPCDef;

#include "jigsaw/procedure/jigsaw_command_list.h"
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
	DECLARE_PROPERTY(Ref<JigsawCommandList>, build_deck);
	DECLARE_PROPERTY(Ref<JigsawCommandList>, play_cards);
	DECLARE_PROPERTY(TypedArray<JigsawTrigger>, triggers);

	DEFAULT_TO_STRING();
};
DECLARE_PREDEFINED_KEY(NPCDef, NPC);

#endif // NPC_DEF_H
