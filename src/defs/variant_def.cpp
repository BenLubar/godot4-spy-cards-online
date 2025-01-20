#include "variant_def.h"

void VariantDef::_bind_methods() {
	BIND_BITFIELD_FLAG(ARCADE_NAMES);
	BIND_BITFIELD_FLAG(HAS_SAVED_DATA);

	BIND_PROPERTY(Variant::STRING, title);
	BIND_PROPERTY(Variant::STRING, content_warning);
	BIND_PROPERTY_BITFIELD(VariantDef::VariantFlags, flags);
	BIND_PROPERTY(Variant::INT, player_count);
	BIND_PROPERTY_PACKED_ENUM_ARRAY(enums::NPCDef::NPC, npcs);
	BIND_PROPERTY_RESOURCE(JigsawProcedureVariantSelectCharacter, select_character);
	BIND_PROPERTY_RESOURCE(JigsawProcedureVariantBuildDeck, build_deck);
	BIND_PROPERTY_RESOURCE(JigsawProcedureVariantValidateDeck, validate_deck);
	BIND_PROPERTY_RESOURCE(JigsawProcedureVariantMain, main);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawTriggerVariant, triggers);
}

IMPLEMENT_PROPERTY(VariantDef, String, title);
IMPLEMENT_PROPERTY(VariantDef, String, content_warning);
IMPLEMENT_PROPERTY(VariantDef, BitField<VariantDef::VariantFlags>, flags);
IMPLEMENT_PROPERTY(VariantDef, int64_t, player_count);
IMPLEMENT_PROPERTY(VariantDef, PackedArray<enums::NPCDef::NPC>, npcs);
IMPLEMENT_PROPERTY(VariantDef, Ref<JigsawProcedureVariantSelectCharacter>, select_character);
IMPLEMENT_PROPERTY(VariantDef, Ref<JigsawProcedureVariantBuildDeck>, build_deck);
IMPLEMENT_PROPERTY(VariantDef, Ref<JigsawProcedureVariantValidateDeck>, validate_deck);
IMPLEMENT_PROPERTY(VariantDef, Ref<JigsawProcedureVariantMain>, main);
IMPLEMENT_PROPERTY(VariantDef, TypedArray<JigsawTriggerVariant>, triggers);
