#include "audience_def.h"

void AudienceDef::_bind_methods() {
	BIND_PROPERTY_ENUM(IconDef::Icon, front);
	BIND_PROPERTY_ENUM(IconDef::Icon, front_cheer);
	BIND_PROPERTY_ENUM(IconDef::Icon, back);
	BIND_PROPERTY_ENUM(IconDef::Icon, back_cheer);
	BIND_PROPERTY(Variant::VECTOR2, hue_range);
	BIND_PROPERTY(Variant::FLOAT, weight);
	BIND_PROPERTY(Variant::FLOAT, pixel_size);
	BIND_PROPERTY_IS(Variant::BOOL, very_tall);
}

IMPLEMENT_PROPERTY(AudienceDef, IconDef::Icon, front);
IMPLEMENT_PROPERTY(AudienceDef, IconDef::Icon, front_cheer);
IMPLEMENT_PROPERTY(AudienceDef, IconDef::Icon, back);
IMPLEMENT_PROPERTY(AudienceDef, IconDef::Icon, back_cheer);
IMPLEMENT_PROPERTY(AudienceDef, Vector2, hue_range);
IMPLEMENT_PROPERTY(AudienceDef, double, weight);
IMPLEMENT_PROPERTY(AudienceDef, double, pixel_size);
IMPLEMENT_PROPERTY_IS(AudienceDef, bool, very_tall);
