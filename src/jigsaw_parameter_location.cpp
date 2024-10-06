#include "jigsaw_parameter_location.h"

void JigsawParameterLocation::_bind_methods() {
	BIND_PROPERTY(Variant::INT, location);

	ClassDB::bind_static_method("JigsawParameterLocation", D_METHOD("make", "location"), &JigsawParameterLocation::make);
}

IMPLEMENT_PROPERTY(JigsawParameterLocation, LocationDef::Location, location);

JigsawParameterLocation *JigsawParameterLocation::make(LocationDef::Location location) {
	JigsawParameterLocation *param = memnew(JigsawParameterLocation);
	param->set_location(location);
	return param;
}
