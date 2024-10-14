#ifndef NPC_DEF_H
#define NPC_DEF_H

#include "dry.h"

#include "enums_character_def.h"
#include "enums_npc_def.h"

class NPCDef;

#include "jigsaw_command_list.h"
#include "jigsaw_trigger.h"

class NPCDef : public Resource {
	GDCLASS(NPCDef, Resource);

public:
	using NPC = enums::NPCDef::NPC;

protected:
	static void _bind_methods();

public:
	NPCDef() = default;
	~NPCDef() = default;

	DECLARE_PROPERTY(String, display_name);
	DECLARE_PROPERTY(enums::CharacterDef::Character, character, = enums::CharacterDef::Character::NONE);
	DECLARE_PROPERTY(Ref<JigsawCommandList>, build_deck);
	DECLARE_PROPERTY(Ref<JigsawCommandList>, play_cards);
	DECLARE_PROPERTY(TypedArray<JigsawTrigger>, triggers);
};
DECLARE_PREDEFINED_KEY(NPCDef, NPC);

#endif // NPC_DEF_H
