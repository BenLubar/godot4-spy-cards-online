#include "jigsaw_parameter_effect_instance.h"

void JigsawParameterEffectInstance::_bind_methods() {
	BIND_PROPERTY_RESOURCE(EffectInstance, instance);
}

IMPLEMENT_PROPERTY(JigsawParameterEffectInstance, Ref<EffectInstance>, instance);
