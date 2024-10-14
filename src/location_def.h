#ifndef LOCATION_DEF_H
#define LOCATION_DEF_H

#include "dry.h"

#include "enums_location_def.h"

class LocationDef : public Resource {
	GDCLASS(LocationDef, Resource);

public:
	using Location = enums::LocationDef::Location;

protected:
	static void _bind_methods();

public:
	LocationDef() = default;
	~LocationDef() = default;
};

#endif // LOCATION_DEF_H
