#include "jigsaw_command_math.h"

void JigsawCommandMath::_bind_methods() {
	BIND_PROPERTY_RESOURCE(JigsawParameter, op);
	BIND_PROPERTY_RESOURCE(JigsawParameter, lhs);
	BIND_PROPERTY_RESOURCE(JigsawParameter, rhs);
	BIND_PROPERTY_RESOURCE(JigsawParameterLocalVariable, result);
}

IMPLEMENT_PROPERTY(JigsawCommandMath, Ref<JigsawParameter>, op);
IMPLEMENT_PROPERTY(JigsawCommandMath, Ref<JigsawParameter>, lhs);
IMPLEMENT_PROPERTY(JigsawCommandMath, Ref<JigsawParameter>, rhs);
IMPLEMENT_PROPERTY(JigsawCommandMath, Ref<JigsawParameterLocalVariable>, result);
