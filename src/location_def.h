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
	DECLARE_PROPERTY(String, editor_name);

	DEFAULT_TO_STRING();
};
DECLARE_PREDEFINED_KEY(LocationDef, LOCATION);

#endif // LOCATION_DEF_H
