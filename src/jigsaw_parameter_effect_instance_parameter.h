#ifndef JIGSAW_PARAMETER_EFFECT_INSTANCE_PARAMETER_H
#define JIGSAW_PARAMETER_EFFECT_INSTANCE_PARAMETER_H

#include "jigsaw_parameter.h"

class JigsawParameterEffectInstanceParameter;

#include "effect_def.h"

class JigsawParameterEffectInstanceParameter : public JigsawParameter {
	GDCLASS(JigsawParameterEffectInstanceParameter, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterEffectInstanceParameter() = default;
	~JigsawParameterEffectInstanceParameter() = default;

	DECLARE_PROPERTY(Ref<JigsawParameter>, effect_instance);
	DECLARE_PROPERTY(Ref<EffectDef>, template_effect);
	DECLARE_PROPERTY(int64_t, index, = -1);

	Type get_type() const override { return EFFECT_INSTANCE_PARAMETER; }

	static Ref<JigsawParameterEffectInstanceParameter> make(const Ref<JigsawParameter> &inst, int64_t index);
	static Ref<JigsawParameterEffectInstanceParameter> make_template(const Ref<EffectDef> &effect, int64_t index);
};

#endif // JIGSAW_PARAMETER_EFFECT_INSTANCE_PARAMETER_H
