#include "jigsaw_parameter_modifier.h"

void JigsawParameterModifier::_bind_methods() {
	BIND_PROPERTY(Variant::INT, modifier);
}

IMPLEMENT_PROPERTY(JigsawParameterModifier, ModifierDef::Modifier, modifier);
