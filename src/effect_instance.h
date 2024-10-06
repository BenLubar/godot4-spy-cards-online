#ifndef EFFECT_INSTANCE_H
#define EFFECT_INSTANCE_H

#include "dry.h"

class EffectInstance;

#include "effect_def.h"
#include "jigsaw_parameter.h"

class EffectInstance : public Resource {
	GDCLASS(EffectInstance, Resource);

protected:
	static void _bind_methods();

public:
	EffectInstance() = default;
	~EffectInstance() = default;

	DECLARE_PROPERTY(EffectDef::Effect, effect, = EffectDef::Effect::NONE);
	DECLARE_PROPERTY(int64_t, priority, = 65);
	DECLARE_PROPERTY(TypedArray<JigsawParameter>, params);
};

#endif // EFFECT_INSTANCE_H
