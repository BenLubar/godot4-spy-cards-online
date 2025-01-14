#include "jigsaw_side.h"

void JigsawSide::_bind_methods() {
	BIND_PROPERTY(Variant::STRING, player_name);
	BIND_PROPERTY_ENUM(enums::CharacterDef::Character, character);
	BIND_PROPERTY_ENUM_ARRAY(enums::CardDef::Card, initial_deck);

	BIND_PROPERTY_VARIANT_ARRAY(Variant::PACKED_INT32_ARRAY, location_card_instances);
	BIND_PROPERTY_RESOURCE_ARRAY(NumericValue, stats);
	BIND_PROPERTY_ENUM_DICTIONARY_RESOURCE(enums::VariableDef::Variable, JigsawParameter, variables);

	ClassDB::bind_method(D_METHOD("assign", "side"), &JigsawSide::assign);
}

IMPLEMENT_PROPERTY_SIMPLE(JigsawSide, String, player_name);
IMPLEMENT_PROPERTY_SIMPLE(JigsawSide, enums::CharacterDef::Character, character);
IMPLEMENT_PROPERTY_SIMPLE(JigsawSide, TypedArray<enums::CardDef::Card>, initial_deck);

IMPLEMENT_PROPERTY_SIMPLE(JigsawSide, TypedArray<PackedInt32Array>, location_card_instances);
IMPLEMENT_PROPERTY_SIMPLE(JigsawSide, TypedArray<NumericValue>, stats);
IMPLEMENT_PROPERTY_SIMPLE(JigsawSide, VariableParameterDict, variables);

void JigsawSide::assign(const Ref<JigsawSide> &side) {
	ERR_FAIL_COND(side.is_null());

	_player_name = side->_player_name;
	_character = side->_character;
	_initial_deck.assign(side->_initial_deck);

	_location_card_instances.append_array(side->_location_card_instances);
	_stats.assign(side->_stats);
	_variables.assign(side->_variables);
}
