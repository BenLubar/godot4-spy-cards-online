#ifndef JIGSAW_PARAMETER_LOCAL_VARIABLE_H
#define JIGSAW_PARAMETER_LOCAL_VARIABLE_H

#include "jigsaw_parameter.h"

class JigsawParameterLocalVariable : public JigsawParameter {
	GDCLASS(JigsawParameterLocalVariable, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterLocalVariable() = default;
	~JigsawParameterLocalVariable() = default;

	Type get_type() const override { return LOCAL_VARIABLE; }
};

#endif // JIGSAW_PARAMETER_LOCAL_VARIABLE_H
