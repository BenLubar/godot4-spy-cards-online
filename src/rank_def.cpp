#include "rank_def.h"

void RankDef::_bind_methods() {
	using namespace enums::RankDef;

	BIND_ENUM_CONSTANT(NONE);

	BIND_ENUM_CONSTANT(ATTACKER);
	BIND_ENUM_CONSTANT(EFFECT);
	BIND_ENUM_CONSTANT(MINI_BOSS);
	BIND_ENUM_CONSTANT(BOSS);
	BIND_ENUM_CONSTANT(TOKEN);
	BIND_ENUM_CONSTANT(LEGACY_ENEMY);
	BIND_ENUM_CONSTANT(LEGACY_NONE);

	BIND_ENUM_CONSTANT(FIRST_CUSTOM);

	BIND_PROPERTY(Variant::STRING, name);
	BIND_PROPERTY(Variant::COLOR, color);
	BIND_PROPERTY(Variant::COLOR, color_alt);
	BIND_PROPERTY_ENUM(enums::IconDef::Icon, front);
	BIND_PROPERTY_ENUM(enums::IconDef::Icon, back);
}

IMPLEMENT_PROPERTY(RankDef, String, name);
IMPLEMENT_PROPERTY(RankDef, Color, color);
IMPLEMENT_PROPERTY(RankDef, Color, color_alt);
IMPLEMENT_PROPERTY(RankDef, enums::IconDef::Icon, front);
IMPLEMENT_PROPERTY(RankDef, enums::IconDef::Icon, back);
