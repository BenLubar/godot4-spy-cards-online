#ifndef TRIBE_DEF_H
#define TRIBE_DEF_H

#include "dry.h"

#include "enums_tribe_def.h"

class TribeDef : public Resource {
	GDCLASS(TribeDef, Resource);

public:
	using Tribe = enums::TribeDef::Tribe;

	enum DisplayMode {
		NORMAL = 0,
		HIDDEN = 1,
		WIDE = 2,
	};

protected:
	static void _bind_methods();

public:
	TribeDef() = default;
	~TribeDef() = default;

	DECLARE_PROPERTY(String, name);
	DECLARE_PROPERTY(Color, color);
	DECLARE_PROPERTY(DisplayMode, display, = DisplayMode::NORMAL);
};
DECLARE_ENUM(TribeDef::DisplayMode);
DECLARE_PREDEFINED_KEY(TribeDef, TRIBE);

#endif // TRIBE_DEF_H
