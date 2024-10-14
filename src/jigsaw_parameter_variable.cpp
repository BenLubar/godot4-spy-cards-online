#include "jigsaw_parameter_variable.h"

void JigsawParameterVariable::_bind_methods() {
	BIND_PROPERTY_ENUM(enums::VariableDef::Variable, variable);

	ClassDB::bind_static_method("JigsawParameterVariable", D_METHOD("make", "variable"), &JigsawParameterVariable::make);
}

IMPLEMENT_PROPERTY(JigsawParameterVariable, enums::VariableDef::Variable, variable);

Ref<JigsawParameterVariable> JigsawParameterVariable::make(enums::VariableDef::Variable variable) {
	Ref<JigsawParameterVariable> param;
	param.instantiate();
	param->set_variable(variable);
	return param;
}
