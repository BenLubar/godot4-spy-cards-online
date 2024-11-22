#ifndef QUEUED_EFFECT_H
#define QUEUED_EFFECT_H

#include "dry.h"

class QueuedEffect;

#include "active/card_instance.h"
#include "defs/effect_instance.h"

#include <godot_cpp/templates/hash_map.hpp>

class QueuedEffect : public RefCounted {
	GDCLASS(QueuedEffect, RefCounted);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(Ref<CardInstance>, card);
	DECLARE_PROPERTY(Ref<EffectInstance>, effect);

	HashMap<enums::VariableDef::Variable, Ref<JigsawParameter>> _variables;

	DEFAULT_TO_STRING();
};

#endif // QUEUED_EFFECT_H
