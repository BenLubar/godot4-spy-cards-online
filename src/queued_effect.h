#ifndef QUEUED_EFFECT_H
#define QUEUED_EFFECT_H

#include "card_instance.h"
#include "effect_instance.h"

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class QueuedEffect : public RefCounted {
	GDCLASS(QueuedEffect, RefCounted);

protected:
	static void _bind_methods();

private:
	Ref<CardInstance> card;
	Ref<EffectInstance> effect;

public:
	QueuedEffect();
	~QueuedEffect();

	Ref<CardInstance> get_card() const;
	void set_card(Ref<CardInstance> new_card);
	Ref<EffectInstance> get_effect() const;
	void set_effect(Ref<EffectInstance> new_effect);
};

#endif // QUEUED_EFFECT_H
