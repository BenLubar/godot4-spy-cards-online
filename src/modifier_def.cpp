#include "modifier_def.h"

void ModifierDef::_bind_methods() {
	BIND_ENUM_CONSTANT(NONE);
	BIND_ENUM_CONSTANT(EMPOWER_ATK);
	BIND_ENUM_CONSTANT(EMPOWER_DEF);
	BIND_ENUM_CONSTANT(NUMB);
	BIND_ENUM_CONSTANT(PREVENT_NUMB);

	BIND_ENUM_CONSTANT(FIRST_CUSTOM);

	BIND_PROPERTY(Variant::STRING, editor_name);
	BIND_PROPERTY_RESOURCE(JigsawCommandList, describe);
	BIND_PROPERTY_RESOURCE(JigsawCommandList, simple_describe);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawTrigger, triggers);
}

IMPLEMENT_PROPERTY(ModifierDef, String, editor_name);
IMPLEMENT_PROPERTY(ModifierDef, Ref<JigsawCommandList>, describe);
IMPLEMENT_PROPERTY(ModifierDef, Ref<JigsawCommandList>, simple_describe);
IMPLEMENT_PROPERTY(ModifierDef, TypedArray<JigsawTrigger>, triggers);
