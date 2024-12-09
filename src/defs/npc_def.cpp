#include "npc_def.h"

void NPCDef::_bind_methods() {
	using namespace enums::NPCDef;

	BIND_ENUM_CONSTANT(NONE);

	BIND_ENUM_CONSTANT(GENERIC);
	BIND_ENUM_CONSTANT(JANET);
	BIND_ENUM_CONSTANT(BU_GI);
	BIND_ENUM_CONSTANT(JOHNNY);
	BIND_ENUM_CONSTANT(KAGE);
	BIND_ENUM_CONSTANT(RITCHEE);
	BIND_ENUM_CONSTANT(SERENE);
	BIND_ENUM_CONSTANT(CARMINA);
	BIND_ENUM_CONSTANT(CHUCK);
	BIND_ENUM_CONSTANT(ARIE);
	BIND_ENUM_CONSTANT(SHAY);
	BIND_ENUM_CONSTANT(CROW);

	BIND_ENUM_CONSTANT(FIRST_CUSTOM);

	BIND_PROPERTY(Variant::STRING, display_name);
	BIND_PROPERTY_ENUM(enums::CharacterDef::Character, character);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawTriggerNPC, triggers);
	BIND_PROPERTY_RESOURCE(JigsawProcedureNPCBuildDeck, build_deck);
	BIND_PROPERTY_RESOURCE(JigsawProcedureNPCDecideChoice, decide_choice);
	BIND_PROPERTY_RESOURCE(JigsawProcedureNPCDecideRealtime, decide_realtime);
}

IMPLEMENT_PROPERTY(NPCDef, String, display_name);
IMPLEMENT_PROPERTY(NPCDef, enums::CharacterDef::Character, character);
IMPLEMENT_PROPERTY(NPCDef, TypedArray<JigsawTriggerNPC>, triggers);
IMPLEMENT_PROPERTY(NPCDef, Ref<JigsawProcedureNPCBuildDeck>, build_deck);
IMPLEMENT_PROPERTY(NPCDef, Ref<JigsawProcedureNPCDecideChoice>, decide_choice);
IMPLEMENT_PROPERTY(NPCDef, Ref<JigsawProcedureNPCDecideRealtime>, decide_realtime);
