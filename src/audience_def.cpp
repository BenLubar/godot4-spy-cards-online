#include "audience_def.h"

void AudienceDef::_bind_methods() {
	using namespace enums::AudienceDef;

	BIND_ENUM_CONSTANT(NONE);

	BIND_ENUM_CONSTANT(FIRST_CUSTOM);

	BIND_PROPERTY_ENUM(enums::IconDef::Icon, front);
	BIND_PROPERTY_ENUM(enums::IconDef::Icon, front_cheer);
	BIND_PROPERTY_ENUM(enums::IconDef::Icon, back);
	BIND_PROPERTY_ENUM(enums::IconDef::Icon, back_cheer);
	BIND_PROPERTY(Variant::FLOAT, weight);
	BIND_PROPERTY(Variant::FLOAT, pixel_size);
	BIND_PROPERTY(Variant::DICTIONARY, json_data);
}

IMPLEMENT_PROPERTY(AudienceDef, enums::IconDef::Icon, front);
IMPLEMENT_PROPERTY(AudienceDef, enums::IconDef::Icon, front_cheer);
IMPLEMENT_PROPERTY(AudienceDef, enums::IconDef::Icon, back);
IMPLEMENT_PROPERTY(AudienceDef, enums::IconDef::Icon, back_cheer);
IMPLEMENT_PROPERTY(AudienceDef, double, weight);
IMPLEMENT_PROPERTY(AudienceDef, double, pixel_size);
IMPLEMENT_PROPERTY(AudienceDef, Dictionary, json_data);
