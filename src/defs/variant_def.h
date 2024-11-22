#ifndef VARIANT_DEF_H
#define VARIANT_DEF_H

#include "dry.h"

#include "enums/enums_npc_def.h"

class VariantDef;

#include "jigsaw/procedure/jigsaw_trigger.h"

class VariantDef : public Resource {
	GDCLASS(VariantDef, Resource);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(String, title);
	DECLARE_PROPERTY(int64_t, player_count, = 2);
	DECLARE_PROPERTY(TypedArray<enums::NPCDef::NPC>, npcs);
	DECLARE_PROPERTY(TypedArray<JigsawTriggerVariant>, triggers);

	DEFAULT_TO_STRING();
};

#endif // VARIANT_DEF_H
