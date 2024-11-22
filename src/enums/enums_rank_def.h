#ifndef ENUMS_RANK_DEF_H
#define ENUMS_RANK_DEF_H

namespace enums {
namespace RankDef {
enum Rank {
	NONE = -1,

	ATTACKER = 0,
	EFFECT = 1,
	MINI_BOSS = 2,
	BOSS = 3,
	TOKEN = 4,
	LEGACY_ENEMY = 5,
	//UNUSED = 6,
	LEGACY_NONE = 7,

	FIRST_CUSTOM = 8,
};
}
}
DECLARE_ENUM(enums::RankDef::Rank);

#endif // ENUMS_RANK_DEF_H
