#ifndef QUEUED_EFFECT_H
#define QUEUED_EFFECT_H

#include "dry.h"

class QueuedEffect;

#include "card_instance.h"
#include "effect_instance.h"

class QueuedEffect : public RefCounted {
	GDCLASS(QueuedEffect, RefCounted);

protected:
	static void _bind_methods();

public:
	QueuedEffect() = default;
	~QueuedEffect() = default;

	DECLARE_PROPERTY(Ref<CardInstance>, card);
	DECLARE_PROPERTY(Ref<EffectInstance>, effect);
};

#endif // QUEUED_EFFECT_H
