#include "jigsaw/parameter/jigsaw_parameter_queued_effect.h"

#include "jigsaw/jigsaw_global.h"

void JigsawParameterQueuedEffect::_bind_methods() {
	BIND_PROPERTY(Variant::INT, queue_index);
	BIND_PROPERTY(Variant::INT, queue_reset_count);

	ClassDB::bind_static_method("JigsawParameterQueuedEffect", D_METHOD("make", "queue_index", "queue_reset_count"), &JigsawParameterQueuedEffect::make);
}

IMPLEMENT_PROPERTY(JigsawParameterQueuedEffect, int64_t, queue_index);
IMPLEMENT_PROPERTY(JigsawParameterQueuedEffect, int64_t, queue_reset_count);

Ref<QueuedEffect> JigsawParameterQueuedEffect::resolve(JigsawGlobal *global) const {
	ERR_FAIL_NULL_V(global, Ref<QueuedEffect>());
	if (global->get_queue_reset_count() != get_queue_reset_count() || get_queue_index() < 0) {
		return Ref<QueuedEffect>();
	}

	TypedArray<QueuedEffect> queue = global->get_queue();
	if (get_queue_index() >= queue.size()) {
		return Ref<QueuedEffect>();
	}

	return queue[get_queue_index()];
}

Ref<JigsawParameterQueuedEffect> JigsawParameterQueuedEffect::make(int64_t queue_index, int64_t queue_reset_count) {
	Ref<JigsawParameterQueuedEffect> param;
	param.instantiate();
	param->set_queue_index(queue_index);
	param->set_queue_reset_count(queue_reset_count);
	return param;
}
