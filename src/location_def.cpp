#include "location_def.h"

void LocationDef::_bind_methods() {
	using namespace enums::LocationDef;

	BIND_ENUM_CONSTANT(NONE);

	BIND_ENUM_CONSTANT(FIELD);
	BIND_ENUM_CONSTANT(DECK);
	BIND_ENUM_CONSTANT(HAND);
	BIND_ENUM_CONSTANT(DISCARD);

	BIND_ENUM_CONSTANT(FIRST_CUSTOM);
}
