#ifndef QUEUED_EFFECT_H
#define QUEUED_EFFECT_H

#include "dry.h"

class QueuedEffect;

#include "jigsaw/parameter/jigsaw_parameter.h"
#include "defs/effect_instance.h"

class QueuedEffect : public Resource {
	GDCLASS(QueuedEffect, Resource);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(int64_t, card_instance, = -1);
	DECLARE_PROPERTY(Ref<EffectInstance>, effect);
	DECLARE_PROPERTY(VariableParameterDict, variables);

	void assign(const Ref<QueuedEffect> &queued);

	DEFAULT_TO_STRING();
};

#endif // QUEUED_EFFECT_H
