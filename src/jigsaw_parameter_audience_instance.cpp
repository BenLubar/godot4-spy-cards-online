#include "jigsaw_parameter_audience_instance.h"

void JigsawParameterAudienceInstance::_bind_methods() {
	BIND_PROPERTY(Variant::INT, group);
	BIND_PROPERTY(Variant::INT, member);
}

IMPLEMENT_PROPERTY(JigsawParameterAudienceInstance, int64_t, group);
IMPLEMENT_PROPERTY(JigsawParameterAudienceInstance, int64_t, member);
