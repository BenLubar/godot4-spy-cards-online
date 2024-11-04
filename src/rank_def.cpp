#include "rank_def.h"

#include "player_preferences_helper.h"

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

	ClassDB::bind_method(D_METHOD("get_color_by_preference"), &RankDef::get_color_by_preference);
}

IMPLEMENT_PROPERTY(RankDef, String, name);
IMPLEMENT_PROPERTY(RankDef, Color, color);
IMPLEMENT_PROPERTY(RankDef, Color, color_alt);
IMPLEMENT_PROPERTY(RankDef, enums::IconDef::Icon, front);
IMPLEMENT_PROPERTY(RankDef, enums::IconDef::Icon, back);

Color RankDef::get_color_by_preference() const {
	return PlayerPreferences::alternate_colors() ? get_color_alt() : get_color();
}
