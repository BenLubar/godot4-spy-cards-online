#include "jigsaw_parameter_choice.h"

void JigsawParameterChoice::_bind_methods() {
	BIND_PROPERTY(Variant::INT, choices);
	BIND_PROPERTY(Variant::INT, index);
}

IMPLEMENT_PROPERTY(JigsawParameterChoice, ChoicesDef::Choices, choices);
IMPLEMENT_PROPERTY(JigsawParameterChoice, int64_t, index);
