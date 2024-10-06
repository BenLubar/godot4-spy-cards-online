#include "jigsaw_parameter_location.h"

void JigsawParameterLocation::_bind_methods() {
	BIND_PROPERTY(Variant::INT, location);
}

IMPLEMENT_PROPERTY(JigsawParameterLocation, LocationDef::Location, location);
