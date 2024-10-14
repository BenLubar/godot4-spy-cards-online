#include "jigsaw_parameter_audience.h"

void JigsawParameterAudience::_bind_methods() {
	BIND_PROPERTY_ENUM(enums::AudienceDef::Audience, audience);

	ClassDB::bind_static_method("JigsawParameterAudience", D_METHOD("make", "audience"), &JigsawParameterAudience::make);
}

IMPLEMENT_PROPERTY(JigsawParameterAudience, enums::AudienceDef::Audience, audience);

Ref<JigsawParameterAudience> JigsawParameterAudience::make(enums::AudienceDef::Audience audience) {
	Ref<JigsawParameterAudience> param;
	param.instantiate();
	param->set_audience(audience);
	return param;
}
