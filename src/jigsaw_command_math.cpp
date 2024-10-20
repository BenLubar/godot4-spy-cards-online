#include "jigsaw_command_math.h"

void JigsawCommandMath::_bind_methods() {
	BIND_ENUM_CONSTANT(EQUALS);
	BIND_ENUM_CONSTANT(LESS_THAN);

	BIND_PROPERTY_ENUM(JigsawCommandMath::Operation, op);
	BIND_PROPERTY_RESOURCE(JigsawParameter, lhs);
	BIND_PROPERTY_RESOURCE(JigsawParameter, rhs);
	BIND_PROPERTY_RESOURCE(JigsawParameterLocalVariable, result);
}

IMPLEMENT_PROPERTY(JigsawCommandMath, JigsawCommandMath::Operation, op);
IMPLEMENT_PROPERTY(JigsawCommandMath, Ref<JigsawParameter>, lhs);
IMPLEMENT_PROPERTY(JigsawCommandMath, Ref<JigsawParameter>, rhs);
IMPLEMENT_PROPERTY(JigsawCommandMath, Ref<JigsawParameterLocalVariable>, result);
