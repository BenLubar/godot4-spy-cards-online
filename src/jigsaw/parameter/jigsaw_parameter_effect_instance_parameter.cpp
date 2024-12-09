#include "jigsaw/parameter/jigsaw_parameter_effect_instance_parameter.h"

void JigsawParameterEffectInstanceParameter::_bind_methods() {
	BIND_PROPERTY_RESOURCE(JigsawParameter, effect_instance);
	BIND_PROPERTY_ENUM(EffectDef::Effect, template_effect);
	BIND_PROPERTY(Variant::INT, index);

	ClassDB::bind_static_method("JigsawParameterEffectInstanceParameter", D_METHOD("make", "inst", "index"), &JigsawParameterEffectInstanceParameter::make);
	ClassDB::bind_static_method("JigsawParameterEffectInstanceParameter", D_METHOD("make_template", "effect", "index"), &JigsawParameterEffectInstanceParameter::make_template);
}

IMPLEMENT_PROPERTY(JigsawParameterEffectInstanceParameter, Ref<JigsawParameter>, effect_instance);
IMPLEMENT_PROPERTY(JigsawParameterEffectInstanceParameter, enums::EffectDef::Effect, template_effect);
IMPLEMENT_PROPERTY(JigsawParameterEffectInstanceParameter, int64_t, index);

Ref<JigsawParameterEffectInstanceParameter> JigsawParameterEffectInstanceParameter::make(const Ref<JigsawParameter> &inst, int64_t index) {
	Ref<JigsawParameterEffectInstanceParameter> param;
	param.instantiate();
	param->set_effect_instance(inst);
	param->set_index(index);
	return param;
}
Ref<JigsawParameterEffectInstanceParameter> JigsawParameterEffectInstanceParameter::make_template(enums::EffectDef::Effect effect, int64_t index) {
	Ref<JigsawParameterEffectInstanceParameter> param;
	param.instantiate();
	param->set_template_effect(effect);
	param->set_index(index);
	return param;
}
