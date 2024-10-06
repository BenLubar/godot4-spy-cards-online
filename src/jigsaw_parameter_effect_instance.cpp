#include "jigsaw_parameter_effect_instance.h"

void JigsawParameterEffectInstance::_bind_methods() {
	BIND_PROPERTY(Variant::OBJECT, instance, PROPERTY_HINT_RESOURCE_TYPE, "EffectInstance");
}

IMPLEMENT_PROPERTY(JigsawParameterEffectInstance, Ref<EffectInstance>, instance);
