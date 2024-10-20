#ifndef STAT_DEF_H
#define STAT_DEF_H

#include "dry.h"

#include "enums_icon_def.h"
#include "enums_stat_def.h"

class StatDef;

#include "jigsaw_procedure.h"

class StatDef : public Resource {
	GDCLASS(StatDef, Resource);

public:
	using Stat = enums::StatDef::Stat;

protected:
	static void _bind_methods();

public:
	StatDef() = default;
	~StatDef() = default;

	DECLARE_PROPERTY(String, name);
	DECLARE_PROPERTY(enums::IconDef::Icon, icon, = enums::IconDef::Icon::NONE);
	DECLARE_PROPERTY(Ref<JigsawProcedureStatFormatCost>, format_cost);
};
DECLARE_PREDEFINED_KEY(StatDef, STAT);

#endif // STAT_DEF_H
