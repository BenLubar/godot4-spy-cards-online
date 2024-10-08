#include "jigsaw_parameter_variable.h"

void JigsawParameterVariable::_bind_methods() {
	BIND_PROPERTY(Variant::INT, variable);

	ClassDB::bind_static_method("JigsawParameterVariable", D_METHOD("make", "variable"), &JigsawParameterVariable::make);
}

IMPLEMENT_PROPERTY(JigsawParameterVariable, VariableDef::Variable, variable);

Ref<JigsawParameterVariable> JigsawParameterVariable::make(VariableDef::Variable variable) {
	Ref<JigsawParameterVariable> param;
	param.instantiate();
	param->set_variable(variable);
	return param;
}
