#ifndef ENUMS_LOCATION_DEF_H
#define ENUMS_LOCATION_DEF_H

namespace enums {
namespace LocationDef {
enum Location {
	NONE = -1,

	FIELD = 0,
	DECK = 1,
	HAND = 2,
	DISCARD = 3,

	FIRST_CUSTOM = 4,
};
}
}
DECLARE_ENUM(enums::LocationDef::Location);

#endif // ENUMS_LOCATION_DEF_H
