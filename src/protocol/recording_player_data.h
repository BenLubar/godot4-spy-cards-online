#ifndef RECORDING_PLAYER_DATA_H
#define RECORDING_PLAYER_DATA_H

#include "dry.h"

class RecordingPlayerData;

#include "enums/enums_card_def.h"
#include "enums/enums_character_def.h"

class RecordingPlayerData : public Resource {
	GDCLASS(RecordingPlayerData, Resource);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(String, display_name);
	DECLARE_PROPERTY(enums::CharacterDef::Character, character);
	DECLARE_PROPERTY(PackedArray<enums::CardDef::Card>, initial_deck);
};

#endif // RECORDING_PLAYER_DATA_H
