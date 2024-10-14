#include "jigsaw_parameter_modifier.h"

void JigsawParameterModifier::_bind_methods() {
	BIND_PROPERTY_ENUM(enums::ModifierDef::Modifier, modifier);

	ClassDB::bind_static_method("JigsawParameterModifier", D_METHOD("make", "modifier"), &JigsawParameterModifier::make);
}

IMPLEMENT_PROPERTY(JigsawParameterModifier, enums::ModifierDef::Modifier, modifier);

Ref<JigsawParameterModifier> JigsawParameterModifier::make(enums::ModifierDef::Modifier modifier) {
	Ref<JigsawParameterModifier> param;
	param.instantiate();
	param->set_modifier(modifier);
	return param;
}
