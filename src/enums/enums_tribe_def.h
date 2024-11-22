#ifndef ENUMS_TRIBE_DEF_H
#define ENUMS_TRIBE_DEF_H

namespace enums {
namespace TribeDef {
enum Tribe {
	NONE = -1,

	SEEDLING = 0,
	WASP = 1,
	FUNGI = 2,
	ZOMBIE = 3,
	PLANT = 4,
	BUG = 5,
	BOT = 6,
	THUG = 7,
	UNKNOWN = 8,
	CHOMPER = 9,
	LEAFBUG = 10,
	DEAD_LANDER = 11,
	MOTHFLY = 12,
	SPIDER = 13,
	LEGACY_CUSTOM = 14,
	LEGACY_NONE = 15,

	FIRST_CUSTOM = 16,
};
}
}
DECLARE_ENUM(enums::TribeDef::Tribe);

#endif // ENUMS_TRIBE_DEF_H
