#include "effect_instance.h"

void EffectInstance::_bind_methods() {
	BIND_PROPERTY(Variant::INT, effect);
	BIND_PROPERTY(Variant::INT, priority);
	BIND_PROPERTY(Variant::ARRAY, params, PROPERTY_HINT_TYPE_STRING, String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":JigsawParameter");
}

IMPLEMENT_PROPERTY(EffectInstance, EffectDef::Effect, effect);
IMPLEMENT_PROPERTY(EffectInstance, int64_t, priority);
IMPLEMENT_PROPERTY(EffectInstance, TypedArray<JigsawParameter>, params);
