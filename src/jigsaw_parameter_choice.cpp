#include "jigsaw_parameter_choice.h"

void JigsawParameterChoice::_bind_methods() {
	BIND_PROPERTY(Variant::INT, choices);
	BIND_PROPERTY(Variant::INT, index);

	ClassDB::bind_static_method("JigsawParameterChoice", D_METHOD("make", "choices", "index"), &JigsawParameterChoice::make);
}

IMPLEMENT_PROPERTY(JigsawParameterChoice, ChoicesDef::Choices, choices);
IMPLEMENT_PROPERTY(JigsawParameterChoice, int64_t, index);

Ref<JigsawParameterChoice> JigsawParameterChoice::make(ChoicesDef::Choices choices, int64_t index) {
	Ref<JigsawParameterChoice> param;
	param.instantiate();
	param->set_choices(choices);
	param->set_index(index);
	return param;
}
