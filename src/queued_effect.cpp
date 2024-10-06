#include "queued_effect.h"

void QueuedEffect::_bind_methods() {
	BIND_PROPERTY(Variant::OBJECT, card);
	BIND_PROPERTY_RESOURCE(EffectInstance, effect);
}

IMPLEMENT_PROPERTY_SIMPLE(QueuedEffect, Ref<CardInstance>, card);
IMPLEMENT_PROPERTY_SIMPLE(QueuedEffect, Ref<EffectInstance>, effect);
