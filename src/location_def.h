#ifndef LOCATION_DEF_H
#define LOCATION_DEF_H

#include "dry.h"

class LocationDef : public Resource {
	GDCLASS(LocationDef, Resource);

public:
	enum Location {
		NONE = -1,
		FIELD = 0,
		DECK = 1,
		HAND = 2,
		DISCARD = 3,

		FIRST_CUSTOM = 256,
	};

protected:
	static void _bind_methods();

public:
	LocationDef() = default;
	~LocationDef() = default;
};
DECLARE_ENUM(LocationDef::Location);

#endif // LOCATION_DEF_H
