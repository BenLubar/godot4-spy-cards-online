#include "stat_def.h"

void StatDef::_bind_methods() {
	using namespace enums::StatDef;

	BIND_ENUM_CONSTANT(NONE);

	BIND_ENUM_CONSTANT(ATK);
	BIND_ENUM_CONSTANT(DEF);
	BIND_ENUM_CONSTANT(HP);
	BIND_ENUM_CONSTANT(TP);

	BIND_ENUM_CONSTANT(FIRST_CUSTOM);

	BIND_PROPERTY(Variant::STRING, name);
	BIND_PROPERTY_ENUM(enums::IconDef::Icon, icon);
	BIND_PROPERTY_RESOURCE(JigsawProcedureStatFormatQuantity, format_quantity);
}

IMPLEMENT_PROPERTY(StatDef, String, name);
IMPLEMENT_PROPERTY(StatDef, enums::IconDef::Icon, icon);
IMPLEMENT_PROPERTY(StatDef, Ref<JigsawProcedureStatFormatQuantity>, format_quantity);
