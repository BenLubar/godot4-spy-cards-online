#include "effect_instance.h"

void EffectInstance::_bind_methods() {
	BIND_PROPERTY(Variant::INT, effect);
	BIND_PROPERTY(Variant::INT, priority);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawParameter, params);
}

IMPLEMENT_PROPERTY(EffectInstance, EffectDef::Effect, effect);
IMPLEMENT_PROPERTY(EffectInstance, int64_t, priority);
IMPLEMENT_PROPERTY(EffectInstance, TypedArray<JigsawParameter>, params);
