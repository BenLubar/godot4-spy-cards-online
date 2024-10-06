#include "variant_def.h"

void VariantDef::_bind_methods() {
	BIND_PROPERTY(Variant::STRING, title);
	BIND_PROPERTY(Variant::INT, player_count);
	BIND_PROPERTY(Variant::ARRAY, npcs);
	BIND_PROPERTY(Variant::ARRAY, triggers, PROPERTY_HINT_TYPE_STRING, String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":JigsawTrigger");
}

IMPLEMENT_PROPERTY(VariantDef, String, title);
IMPLEMENT_PROPERTY(VariantDef, int64_t, player_count);
IMPLEMENT_PROPERTY(VariantDef, TypedArray<NPCDef::NPC>, npcs);
IMPLEMENT_PROPERTY(VariantDef, TypedArray<JigsawTrigger>, triggers);
