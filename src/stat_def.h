#ifndef STAT_DEF_H
#define STAT_DEF_H

#include "dry.h"
#include "icon_def.h"
#include "jigsaw_command_list.h"

using namespace godot;

class StatDef : public Resource {
	GDCLASS(StatDef, Resource);

public:
	enum Stat {
		NONE = -1,
		ATK = 0,
		DEF = 1,
		HP = 2,
		TP = 3,

		FIRST_CUSTOM = 256,
	};

protected:
	static void _bind_methods();

public:
	StatDef();
	~StatDef();

	DECLARE_PROPERTY(String, name);
	DECLARE_PROPERTY(IconDef::Icon, icon, = IconDef::Icon::NONE);
	DECLARE_PROPERTY(Ref<JigsawCommandList>, format_quantity);
};
VARIANT_ENUM_CAST(StatDef::Stat);

#endif // STAT_DEF_H
