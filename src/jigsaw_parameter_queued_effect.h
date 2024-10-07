#ifndef JIGSAW_PARAMETER_QUEUED_EFFECT_H
#define JIGSAW_PARAMETER_QUEUED_EFFECT_H

#include "jigsaw_parameter.h"

class JigsawParameterQueuedEffect;

#include "queued_effect.h"

class JigsawParameterQueuedEffect : public JigsawParameter {
	GDCLASS(JigsawParameterQueuedEffect, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterQueuedEffect() = default;
	~JigsawParameterQueuedEffect() = default;

	DECLARE_PROPERTY(Ref<QueuedEffect>, effect);

	Type get_type() const override { return QUEUED_EFFECT; }

	static JigsawParameterQueuedEffect *make(QueuedEffect *effect);
};

#endif // JIGSAW_PARAMETER_QUEUED_EFFECT_H
