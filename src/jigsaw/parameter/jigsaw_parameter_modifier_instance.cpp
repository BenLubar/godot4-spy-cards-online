#include "jigsaw/parameter/jigsaw_parameter_modifier_instance.h"

void JigsawParameterModifierInstance::_bind_methods() {
	BIND_PROPERTY(Variant::INT, card_instance);
	BIND_PROPERTY(Variant::INT, modifier_index);

	ClassDB::bind_static_method("JigsawParameterModifierInstance", D_METHOD("make", "card_instance", "modifier_index"), &JigsawParameterModifierInstance::make);
}

IMPLEMENT_PROPERTY(JigsawParameterModifierInstance, int64_t, card_instance);
IMPLEMENT_PROPERTY(JigsawParameterModifierInstance, int64_t, modifier_index);

Ref<JigsawParameterModifierInstance> JigsawParameterModifierInstance::make(int64_t card_instance, int64_t modifier_index) {
	Ref<JigsawParameterModifierInstance> param;
	param.instantiate();
	param->set_card_instance(card_instance);
	param->set_modifier_index(modifier_index);
	return param;
}
