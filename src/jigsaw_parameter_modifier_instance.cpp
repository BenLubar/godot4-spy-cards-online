#include "jigsaw_parameter_modifier_instance.h"

void JigsawParameterModifierInstance::_bind_methods() {
	BIND_PROPERTY(Variant::OBJECT, instance);

	ClassDB::bind_static_method("JigsawParameterModifierInstance", D_METHOD("make", "instance"), &JigsawParameterModifierInstance::make);
}

IMPLEMENT_PROPERTY(JigsawParameterModifierInstance, Ref<ModifierInstance>, instance);

JigsawParameterModifierInstance *JigsawParameterModifierInstance::make(const Ref<ModifierInstance> &instance) {
	JigsawParameterModifierInstance *param = memnew(JigsawParameterModifierInstance);
	param->set_instance(instance);
	return param;
}
