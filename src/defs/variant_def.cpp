#include "variant_def.h"

void VariantDef::_bind_methods() {
	BIND_PROPERTY(Variant::STRING, title);
	BIND_PROPERTY(Variant::INT, player_count);
	BIND_PROPERTY_IS(Variant::BOOL, arcade_names);
	BIND_PROPERTY_ENUM_ARRAY(enums::NPCDef::NPC, npcs);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawTriggerVariant, triggers);
}

IMPLEMENT_PROPERTY(VariantDef, String, title);
IMPLEMENT_PROPERTY(VariantDef, int64_t, player_count);
IMPLEMENT_PROPERTY_IS(VariantDef, bool, arcade_names);
IMPLEMENT_PROPERTY(VariantDef, TypedArray<enums::NPCDef::NPC>, npcs);
IMPLEMENT_PROPERTY(VariantDef, TypedArray<JigsawTriggerVariant>, triggers);
