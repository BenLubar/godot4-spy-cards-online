#include "jigsaw_parameter_tribe.h"

void JigsawParameterTribe::_bind_methods() {
	BIND_PROPERTY_ENUM(enums::TribeDef::Tribe, tribe);

	ClassDB::bind_static_method("JigsawParameterTribe", D_METHOD("make", "tribe"), &JigsawParameterTribe::make);
}
	
IMPLEMENT_PROPERTY(JigsawParameterTribe, enums::TribeDef::Tribe, tribe);

Ref<JigsawParameterTribe> JigsawParameterTribe::make(enums::TribeDef::Tribe tribe) {
	Ref<JigsawParameterTribe> param;
	param.instantiate();
	param->set_tribe(tribe);
	return param;
}
