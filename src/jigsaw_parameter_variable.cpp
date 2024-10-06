#include "jigsaw_parameter_variable.h"

void JigsawParameterVariable::_bind_methods() {
	BIND_PROPERTY(Variant::INT, variable);
}

IMPLEMENT_PROPERTY(JigsawParameterVariable, VariableDef::Variable, variable);
