#ifndef JIGSAW_PARAMETER_QUEUED_EFFECT_H
#define JIGSAW_PARAMETER_QUEUED_EFFECT_H

#include "jigsaw/parameter/jigsaw_parameter.h"

class JigsawParameterQueuedEffect;

#include "active/queued_effect.h"

class JigsawParameterQueuedEffect : public JigsawParameter {
	GDCLASS(JigsawParameterQueuedEffect, JigsawParameter);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(int64_t, queue_index, = -1);
	DECLARE_PROPERTY(int64_t, queue_reset_count, = -1);

	Type get_type() const override { return QUEUED_EFFECT; }

	Ref<QueuedEffect> resolve(JigsawGlobal *global) const;

	static Ref<JigsawParameterQueuedEffect> make(int64_t queue_index, int64_t queue_reset_count);
};

#endif // JIGSAW_PARAMETER_QUEUED_EFFECT_H
