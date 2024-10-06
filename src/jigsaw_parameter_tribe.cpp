#include "jigsaw_parameter_tribe.h"

void JigsawParameterTribe::_bind_methods() {
	BIND_PROPERTY(Variant::INT, tribe);

	ClassDB::bind_static_method("JigsawParameterTribe", D_METHOD("make", "tribe"), &JigsawParameterTribe::make);
}
	
IMPLEMENT_PROPERTY(JigsawParameterTribe, TribeDef::Tribe, tribe);

JigsawParameterTribe *JigsawParameterTribe::make(TribeDef::Tribe tribe) {
	JigsawParameterTribe *param = memnew(JigsawParameterTribe);
	param->set_tribe(tribe);
	return param;
}
