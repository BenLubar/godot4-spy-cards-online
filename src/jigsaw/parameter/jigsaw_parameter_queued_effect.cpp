#include "jigsaw_parameter_queued_effect.h"

void JigsawParameterQueuedEffect::_bind_methods() {
	BIND_PROPERTY(Variant::OBJECT, effect);

	ClassDB::bind_static_method("JigsawParameterQueuedEffect", D_METHOD("make", "effect"), &JigsawParameterQueuedEffect::make);
}

IMPLEMENT_PROPERTY(JigsawParameterQueuedEffect, Ref<QueuedEffect>, effect);

Ref<JigsawParameterQueuedEffect> JigsawParameterQueuedEffect::make(const Ref<QueuedEffect> &effect) {
	Ref<JigsawParameterQueuedEffect> param;
	param.instantiate();
	param->set_effect(effect);
	return param;
}
