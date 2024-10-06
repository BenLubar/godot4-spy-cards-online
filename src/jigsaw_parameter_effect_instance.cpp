#include "jigsaw_parameter_effect_instance.h"

void JigsawParameterEffectInstance::_bind_methods() {
	BIND_PROPERTY_RESOURCE(EffectInstance, instance);

	ClassDB::bind_static_method("JigsawParameterEffectInstance", D_METHOD("make", "instance"), &JigsawParameterEffectInstance::make);
}

IMPLEMENT_PROPERTY(JigsawParameterEffectInstance, Ref<EffectInstance>, instance);

JigsawParameterEffectInstance *JigsawParameterEffectInstance::make(const Ref<EffectInstance> &instance) {
	JigsawParameterEffectInstance *param = memnew(JigsawParameterEffectInstance);
	param->set_instance(instance);
	return param;
}
