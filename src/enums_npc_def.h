#ifndef ENUMS_NPC_DEF_H
#define ENUMS_NPC_DEF_H

namespace enums {
namespace NPCDef {
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

	FIRST_CUSTOM = 16,
};
}
}
DECLARE_ENUM(enums::NPCDef::NPC);

#endif // ENUMS_NPC_DEF_H
