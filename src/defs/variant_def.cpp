#include "variant_def.h"

void VariantDef::_bind_methods() {
	BIND_BITFIELD_FLAG(ARCADE_NAMES);

	BIND_PROPERTY(Variant::STRING, title);
	BIND_PROPERTY(Variant::STRING, content_warning);
	BIND_PROPERTY_BITFIELD(VariantDef::VariantFlags, flags);
	BIND_PROPERTY(Variant::INT, player_count);
	BIND_PROPERTY_ENUM_ARRAY(enums::NPCDef::NPC, npcs);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawTriggerVariant, triggers);
}

IMPLEMENT_PROPERTY(VariantDef, String, title);
IMPLEMENT_PROPERTY(VariantDef, String, content_warning);
IMPLEMENT_PROPERTY(VariantDef, BitField<VariantDef::VariantFlags>, flags);
IMPLEMENT_PROPERTY(VariantDef, int64_t, player_count);
IMPLEMENT_PROPERTY(VariantDef, TypedArray<enums::NPCDef::NPC>, npcs);
IMPLEMENT_PROPERTY(VariantDef, TypedArray<JigsawTriggerVariant>, triggers);
