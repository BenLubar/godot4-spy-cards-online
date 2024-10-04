#ifndef TRIBE_DEF_H
#define TRIBE_DEF_H

#include "dry.h"

using namespace godot;

class TribeDef : public Resource {
	GDCLASS(TribeDef, Resource);

public:
	enum Tribe {
		NONE = -1,

		SEEDLING = 0,
		WASP = 1,
		FUNGI = 2,
		ZOMBIE = 3,
		PLANT = 4,
		BUG = 5,
		BOT = 6,
		THUG = 7,
		UNKNOWN = 8,
		CHOMPER = 9,
		LEAFBUG = 10,
		DEAD_LANDER = 11,
		MOTHFLY = 12,
		SPIDER = 13,
		LEGACY_CUSTOM = 14,
		LEGACY_NONE = 15,

		FIRST_CUSTOM = 16,
	};
	enum DisplayMode {
		NORMAL = 0,
		HIDDEN = 1,
		WIDE = 2,
	};

protected:
	static void _bind_methods();

public:
	TribeDef();
	~TribeDef();

	DECLARE_PROPERTY(String, name);
	DECLARE_PROPERTY(Color, color);
	DECLARE_PROPERTY(DisplayMode, display, = DisplayMode::NORMAL);
};
VARIANT_ENUM_CAST(TribeDef::Tribe);
VARIANT_ENUM_CAST(TribeDef::DisplayMode);

#endif // TRIBE_DEF_H
