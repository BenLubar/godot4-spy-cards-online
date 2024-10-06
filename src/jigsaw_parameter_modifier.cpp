#include "jigsaw_parameter_modifier.h"

void JigsawParameterModifier::_bind_methods() {
	BIND_PROPERTY(Variant::INT, modifier);

	ClassDB::bind_static_method("JigsawParameterModifier", D_METHOD("make", "modifier"), &JigsawParameterModifier::make);
}

IMPLEMENT_PROPERTY(JigsawParameterModifier, ModifierDef::Modifier, modifier);

JigsawParameterModifier *JigsawParameterModifier::make(ModifierDef::Modifier modifier) {
	JigsawParameterModifier *param = memnew(JigsawParameterModifier);
	param->set_modifier(modifier);
	return param;
}
