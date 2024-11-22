#include "jigsaw_parameter_local_variable.h"

void JigsawParameterLocalVariable::_bind_methods() {
	BIND_CONSTANT(FRAME_ARGUMENTS);
	BIND_CONSTANT(FRAME_RESULTS);

	BIND_PROPERTY(Variant::INT, frame);
	BIND_PROPERTY(Variant::INT, slot);

	ClassDB::bind_static_method("JigsawParameterLocalVariable", D_METHOD("make", "frame", "slot"), &JigsawParameterLocalVariable::make);
}

IMPLEMENT_PROPERTY(JigsawParameterLocalVariable, int64_t, frame);
IMPLEMENT_PROPERTY(JigsawParameterLocalVariable, int64_t, slot);

Ref<JigsawParameterLocalVariable> JigsawParameterLocalVariable::make(int64_t frame, int64_t slot) {
	Ref<JigsawParameterLocalVariable> param;
	param.instantiate();
	param->set_frame(frame);
	param->set_slot(slot);
	return param;
}
