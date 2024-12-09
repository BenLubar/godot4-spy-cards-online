#ifndef JIGSAW_PARAMETER_EFFECT_INSTANCE_PARAMETER_H
#define JIGSAW_PARAMETER_EFFECT_INSTANCE_PARAMETER_H

#include "jigsaw/parameter/jigsaw_parameter.h"

class JigsawParameterEffectInstanceParameter;

#include "defs/effect_def.h"

class JigsawParameterEffectInstanceParameter : public JigsawParameter {
	GDCLASS(JigsawParameterEffectInstanceParameter, JigsawParameter);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(Ref<JigsawParameter>, effect_instance);
	DECLARE_PROPERTY(enums::EffectDef::Effect, template_effect, = enums::EffectDef::NONE);
	DECLARE_PROPERTY(int64_t, index, = -1);

	Type get_type() const override { return EFFECT_INSTANCE_PARAMETER; }

	static Ref<JigsawParameterEffectInstanceParameter> make(const Ref<JigsawParameter> &inst, int64_t index);
	static Ref<JigsawParameterEffectInstanceParameter> make_template(enums::EffectDef::Effect effect, int64_t index);
};

#endif // JIGSAW_PARAMETER_EFFECT_INSTANCE_PARAMETER_H
