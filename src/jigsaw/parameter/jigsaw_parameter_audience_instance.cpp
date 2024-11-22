#include "jigsaw/parameter/jigsaw_parameter_audience_instance.h"

void JigsawParameterAudienceInstance::_bind_methods() {
	BIND_PROPERTY(Variant::INT, group);
	BIND_PROPERTY(Variant::INT, member);
}

IMPLEMENT_PROPERTY(JigsawParameterAudienceInstance, int64_t, group);
IMPLEMENT_PROPERTY(JigsawParameterAudienceInstance, int64_t, member);

Ref<JigsawParameterAudienceInstance> JigsawParameterAudienceInstance::make(int64_t group, int64_t member) {
	Ref<JigsawParameterAudienceInstance> param;
	param.instantiate();
	param->set_group(group);
	param->set_member(member);
	return param;
}
