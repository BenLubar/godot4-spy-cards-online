#ifndef JIGSAW_COMMAND_MATH_H
#define JIGSAW_COMMAND_MATH_H

#include "dry.h"

class JigsawCommandMath;

#include "jigsaw_command.h"
#include "jigsaw_parameter.h"
#include "jigsaw_parameter_local_variable.h"

class JigsawCommandMath : public JigsawCommand {
	GDCLASS(JigsawCommandMath, JigsawCommand);

protected:
	static void _bind_methods();

public:
	JigsawCommandMath() = default;
	~JigsawCommandMath() = default;

	DECLARE_PROPERTY(Ref<JigsawParameter>, op); // choice
	DECLARE_PROPERTY(Ref<JigsawParameter>, lhs); // amount or float
	DECLARE_PROPERTY(Ref<JigsawParameter>, rhs); // same type as lhs; for some operations, skipped
	DECLARE_PROPERTY(Ref<JigsawParameterLocalVariable>, result);

	Type get_type() const override { return MATH; }
};

#endif // JIGSAW_COMMAND_MATH_H
