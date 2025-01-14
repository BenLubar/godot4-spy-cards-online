#ifndef JIGSAW_STATE_H
#define JIGSAW_STATE_H

#include "dry.h"

#include "jigsaw/jigsaw_side.h"
#include "jigsaw/parameter/jigsaw_parameter.h"
#include "active/audience.h"
#include "active/card_instance.h"

class JigsawState : public Resource {
	GDCLASS(JigsawState, Resource);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(TypedArray<CardInstance>, cards);
	DECLARE_PROPERTY(TypedArray<JigsawSide>, sides);
	DECLARE_PROPERTY(VariableParameterDict, variables);
	DECLARE_PROPERTY(Ref<Audience>, audience);

	void assign(const Ref<JigsawState> &state);
};

#endif // JIGSAW_STATE_H
