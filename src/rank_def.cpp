#include "rank_def.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void RankDef::_bind_methods() {
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
	BIND_PROPERTY(Variant::INT, front);
	BIND_PROPERTY(Variant::INT, back);
}
RankDef::RankDef() {}
RankDef::~RankDef() {}

IMPLEMENT_PROPERTY(RankDef, String, name);
IMPLEMENT_PROPERTY(RankDef, Color, color);
IMPLEMENT_PROPERTY(RankDef, Color, color_alt);
IMPLEMENT_PROPERTY(RankDef, IconDef::Icon, front);
IMPLEMENT_PROPERTY(RankDef, IconDef::Icon, back);
