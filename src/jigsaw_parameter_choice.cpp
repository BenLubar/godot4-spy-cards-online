#include "jigsaw_parameter_choice.h"

void JigsawParameterChoice::_bind_methods() {
	BIND_PROPERTY_ENUM(enums::ChoicesDef::Choices, choices);
	BIND_PROPERTY(Variant::INT, index);

	ClassDB::bind_static_method("JigsawParameterChoice", D_METHOD("make", "choices", "index"), &JigsawParameterChoice::make);
}

IMPLEMENT_PROPERTY(JigsawParameterChoice, enums::ChoicesDef::Choices, choices);
IMPLEMENT_PROPERTY(JigsawParameterChoice, int64_t, index);

Ref<JigsawParameterChoice> JigsawParameterChoice::make(enums::ChoicesDef::Choices choices, int64_t index) {
	Ref<JigsawParameterChoice> param;
	param.instantiate();
	param->set_choices(choices);
	param->set_index(index);
	return param;
}
