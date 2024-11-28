#include "recording_player_data.h"

void RecordingPlayerData::_bind_methods() {
	BIND_PROPERTY(Variant::STRING, display_name);
	BIND_PROPERTY_ENUM(enums::CharacterDef::Character, character);
	BIND_PROPERTY_ENUM_ARRAY(enums::CardDef::Card, initial_deck);
}

IMPLEMENT_PROPERTY(RecordingPlayerData, String, display_name);
IMPLEMENT_PROPERTY(RecordingPlayerData, enums::CharacterDef::Character, character);
IMPLEMENT_PROPERTY(RecordingPlayerData, TypedArray<enums::CardDef::Card>, initial_deck);
