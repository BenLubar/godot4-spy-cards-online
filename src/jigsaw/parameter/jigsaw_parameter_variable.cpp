#include "jigsaw_parameter_variable.h"

void JigsawParameterVariable::_bind_methods() {
	BIND_PROPERTY_ENUM(enums::VariableDef::Variable, variable);
	BIND_PROPERTY_RESOURCE(JigsawParameter, specifier);

	ClassDB::bind_static_method("JigsawParameterVariable", D_METHOD("make", "variable", "specifier"), &JigsawParameterVariable::make, DEFVAL(Ref<JigsawParameter>()));
}

IMPLEMENT_PROPERTY(JigsawParameterVariable, enums::VariableDef::Variable, variable);
IMPLEMENT_PROPERTY(JigsawParameterVariable, Ref<JigsawParameter>, specifier);

Ref<JigsawParameterVariable> JigsawParameterVariable::make(enums::VariableDef::Variable variable, const Ref<JigsawParameter> &specifier) {
	Ref<JigsawParameterVariable> param;
	param.instantiate();
	param->set_variable(variable);
	param->set_specifier(specifier);
	return param;
}
