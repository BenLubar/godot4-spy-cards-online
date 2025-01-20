#include "recording_player_data.h"

void RecordingPlayerData::_bind_methods() {
	BIND_PROPERTY(Variant::STRING, display_name);
	BIND_PROPERTY_ENUM(enums::CharacterDef::Character, character);
	BIND_PROPERTY_PACKED_ENUM_ARRAY(enums::CardDef::Card, initial_deck);
	BIND_PROPERTY_VARIANT_DICTIONARY_RESOURCE(Variant::STRING, JigsawParameter, save_data);
}

IMPLEMENT_PROPERTY(RecordingPlayerData, String, display_name);
IMPLEMENT_PROPERTY(RecordingPlayerData, enums::CharacterDef::Character, character);
IMPLEMENT_PROPERTY(RecordingPlayerData, PackedArray<enums::CardDef::Card>, initial_deck);
IMPLEMENT_PROPERTY(RecordingPlayerData, RecordingPlayerData::SaveDataDictionary, save_data);
