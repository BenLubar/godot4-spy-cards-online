#ifndef RANK_DEF_H
#define RANK_DEF_H

#include "dry.h"
#include "icon_def.h"

using namespace godot;

class RankDef : public Resource {
	GDCLASS(RankDef, Resource);

public:
	enum Rank {
		NONE = -1,

		ATTACKER = 0,
		EFFECT = 1,
		MINI_BOSS = 2,
		BOSS = 3,
		TOKEN = 4,
		LEGACY_ENEMY = 5,
		//UNUSED = 6,
		LEGACY_NONE = 7,

		FIRST_CUSTOM = 8,
	};

protected:
	static void _bind_methods();

public:
	RankDef();
	~RankDef();

	DECLARE_PROPERTY(String, name);
	DECLARE_PROPERTY(Color, color);
	DECLARE_PROPERTY(Color, color_alt);
	DECLARE_PROPERTY(IconDef::Icon, front, = IconDef::Icon::NONE);
	DECLARE_PROPERTY(IconDef::Icon, back, = IconDef::Icon::NONE);
};
VARIANT_ENUM_CAST(RankDef::Rank);

#endif // RANK_DEF_H
