#ifndef JIGSAW_PARAMETER_EFFECT_INSTANCE_H
#define JIGSAW_PARAMETER_EFFECT_INSTANCE_H

#include "jigsaw_parameter.h"

class JigsawParameterEffectInstance;

#include "effect_instance.h"

class JigsawParameterEffectInstance : public JigsawParameter {
	GDCLASS(JigsawParameterEffectInstance, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterEffectInstance() = default;
	~JigsawParameterEffectInstance() = default;

	DECLARE_PROPERTY(Ref<EffectInstance>, instance);

	Type get_type() const override { return EFFECT_INSTANCE; }

	static Ref<JigsawParameterEffectInstance> make(const Ref<EffectInstance> &instance);
};

#endif // JIGSAW_PARAMETER_EFFECT_INSTANCE_H
