#include "jigsaw_trigger.h"

void JigsawTrigger::_bind_methods() {
	BIND_PROPERTY_RESOURCE(JigsawCommandList, commands);

	ClassDB::bind_method(D_METHOD("get_type"), &JigsawTrigger::get_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "type"), "", "get_type");
}

IMPLEMENT_PROPERTY(JigsawTrigger, Ref<JigsawCommandList>, commands);
