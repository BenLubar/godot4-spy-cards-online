#include "jigsaw_parameter_card_instance.h"

void JigsawParameterCardInstance::_bind_methods() {
	BIND_PROPERTY(Variant::OBJECT, instance);

	ClassDB::bind_static_method("JigsawParameterCardInstance", D_METHOD("make", "instance"), &JigsawParameterCardInstance::make);
}

IMPLEMENT_PROPERTY(JigsawParameterCardInstance, Ref<CardInstance>, instance);

JigsawParameterCardInstance *JigsawParameterCardInstance::make(const Ref<CardInstance> &instance) {
	JigsawParameterCardInstance *param = memnew(JigsawParameterCardInstance);
	param->set_instance(instance);
	return param;
}
