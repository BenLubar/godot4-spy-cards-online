#include "modifier_def.h"

void ModifierDef::_bind_methods() {
	using namespace enums::ModifierDef;

	BIND_ENUM_CONSTANT(NONE);

	BIND_ENUM_CONSTANT(EMPOWER_ATK);
	BIND_ENUM_CONSTANT(EMPOWER_DEF);
	BIND_ENUM_CONSTANT(NUMB);
	BIND_ENUM_CONSTANT(PREVENT_NUMB);

	BIND_ENUM_CONSTANT(FIRST_CUSTOM);

	BIND_PROPERTY(Variant::STRING, editor_name);
	BIND_PROPERTY_RESOURCE(JigsawProcedureModifierDescribe, describe);
	BIND_PROPERTY_RESOURCE(JigsawProcedureModifierSimpleDescribe, simple_describe);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawTriggerModifier, triggers);
}

IMPLEMENT_PROPERTY(ModifierDef, String, editor_name);
IMPLEMENT_PROPERTY(ModifierDef, Ref<JigsawProcedureModifierDescribe>, describe);
IMPLEMENT_PROPERTY(ModifierDef, Ref<JigsawProcedureModifierSimpleDescribe>, simple_describe);
IMPLEMENT_PROPERTY(ModifierDef, TypedArray<JigsawTriggerModifier>, triggers);
