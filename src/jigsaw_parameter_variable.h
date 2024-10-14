#ifndef JIGSAW_PARAMETER_VARIABLE_H
#define JIGSAW_PARAMETER_VARIABLE_H

#include "jigsaw_parameter.h"

#include "enums_variable_def.h"

class JigsawParameterVariable : public JigsawParameter {
	GDCLASS(JigsawParameterVariable, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterVariable() = default;
	~JigsawParameterVariable() = default;

	DECLARE_PROPERTY(enums::VariableDef::Variable, variable, = enums::VariableDef::Variable::NONE);

	Type get_type() const override { return VARIABLE; }

	static Ref<JigsawParameterVariable> make(enums::VariableDef::Variable variable);
};

#endif // JIGSAW_PARAMETER_VARIABLE_H
