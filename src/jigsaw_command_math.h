#ifndef JIGSAW_COMMAND_MATH_H
#define JIGSAW_COMMAND_MATH_H

#include "jigsaw_command.h"

class JigsawCommandMath : public JigsawCommand {
	GDCLASS(JigsawCommandMath, JigsawCommand);

public:
	enum Operation {
		EQUALS = 0, // result is boolean
		LESS_THAN = 1, // result is boolean
	};

protected:
	static void _bind_methods();

public:
	JigsawCommandMath() = default;
	~JigsawCommandMath() = default;

	DECLARE_PROPERTY(Operation, op, = Operation::EQUALS);
	DECLARE_PROPERTY(Ref<JigsawParameter>, lhs); // amount or float
	DECLARE_PROPERTY(Ref<JigsawParameter>, rhs); // same type as lhs; for some operations, skipped
	DECLARE_PROPERTY(Ref<JigsawParameterLocalVariable>, result); // see Operation enum

	Type get_type() const override { return MATH; }
};
DECLARE_ENUM(JigsawCommandMath::Operation);

#endif // JIGSAW_COMMAND_MATH_H
