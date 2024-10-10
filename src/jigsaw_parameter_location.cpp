#include "jigsaw_parameter_location.h"

void JigsawParameterLocation::_bind_methods() {
	BIND_PROPERTY_ENUM(LocationDef::Location, location);

	ClassDB::bind_static_method("JigsawParameterLocation", D_METHOD("make", "location"), &JigsawParameterLocation::make);
}

IMPLEMENT_PROPERTY(JigsawParameterLocation, LocationDef::Location, location);

Ref<JigsawParameterLocation> JigsawParameterLocation::make(LocationDef::Location location) {
	Ref<JigsawParameterLocation> param;
	param.instantiate();
	param->set_location(location);
	return param;
}
