#include "jigsaw_parameter_card_instance.h"

void JigsawParameterCardInstance::_bind_methods() {
	BIND_PROPERTY(Variant::OBJECT, instance);
}

IMPLEMENT_PROPERTY(JigsawParameterCardInstance, Ref<CardInstance>, instance);
