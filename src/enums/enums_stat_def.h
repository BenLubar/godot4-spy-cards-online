#ifndef ENUMS_STAT_DEF_H
#define ENUMS_STAT_DEF_H

namespace enums {
namespace StatDef {
enum Stat {
	NONE = -1,

	ATK = 0,
	DEF = 1,
	HP = 2,
	TP = 3,

	FIRST_CUSTOM = 4,
};
}
}
DECLARE_ENUM(enums::StatDef::Stat);

#endif // ENUMS_STAT_DEF_H
