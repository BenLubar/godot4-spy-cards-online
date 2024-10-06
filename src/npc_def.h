#ifndef NPC_DEF_H
#define NPC_DEF_H

#include "dry.h"

class NPCDef;

#include "jigsaw_command_list.h"
#include "jigsaw_trigger.h"

class NPCDef : public Resource {
	GDCLASS(NPCDef, Resource);

public:
	enum NPC {
		NONE = -1,
		GENERIC = 0,
		JANET = 1,
		BU_GI = 2,
		JOHNNY = 3,
		KAGE = 4,
		RITCHEE = 5,
		SERENE = 6,
		CARMINA = 7,
		CHUCK = 8,
		ARIE = 9,
		SHAY = 10,
		CROW = 11,

		FIRST_CUSTOM = 256,
	};

protected:
	static void _bind_methods();

public:
	NPCDef() = default;
	~NPCDef() = default;

	DECLARE_PROPERTY(String, display_name);
	DECLARE_PROPERTY(int64_t, character, = -1);
	DECLARE_PROPERTY(Ref<JigsawCommandList>, build_deck);
	DECLARE_PROPERTY(Ref<JigsawCommandList>, play_cards);
	DECLARE_PROPERTY(TypedArray<JigsawTrigger>, triggers);
};
DECLARE_ENUM(NPCDef::NPC);
DECLARE_PREDEFINED_KEY(NPCDef, NPC);

#endif // NPC_DEF_H
