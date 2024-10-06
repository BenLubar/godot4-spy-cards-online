#ifndef VARIANT_DEF_H
#define VARIANT_DEF_H

#include "dry.h"

#include "npc_def.h"
#include "jigsaw_trigger.h"

class VariantDef : public Resource {
	GDCLASS(VariantDef, Resource);

protected:
	static void _bind_methods();

public:
	VariantDef() = default;
	~VariantDef() = default;

	DECLARE_PROPERTY(String, title);
	DECLARE_PROPERTY(int64_t, player_count, = 2);
	DECLARE_PROPERTY(TypedArray<NPCDef::NPC>, npcs);
	DECLARE_PROPERTY(TypedArray<JigsawTrigger>, triggers);
};

#endif // VARIANT_DEF_H
