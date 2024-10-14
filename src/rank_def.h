#ifndef RANK_DEF_H
#define RANK_DEF_H

#include "dry.h"

#include "enums_icon_def.h"
#include "enums_rank_def.h"

class RankDef : public Resource {
	GDCLASS(RankDef, Resource);

public:
	using Rank = enums::RankDef::Rank;

protected:
	static void _bind_methods();

public:
	RankDef() = default;
	~RankDef() = default;

	DECLARE_PROPERTY(String, name);
	DECLARE_PROPERTY(Color, color);
	DECLARE_PROPERTY(Color, color_alt);
	DECLARE_PROPERTY(enums::IconDef::Icon, front, = enums::IconDef::Icon::NONE);
	DECLARE_PROPERTY(enums::IconDef::Icon, back, = enums::IconDef::Icon::NONE);
};
DECLARE_PREDEFINED_KEY(RankDef, RANK);

#endif // RANK_DEF_H
