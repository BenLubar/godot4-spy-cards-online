#include "jigsaw/jigsaw_state.h"

void JigsawState::_bind_methods() {
	BIND_PROPERTY_RESOURCE_ARRAY(CardInstance, cards);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawSide, sides);
	BIND_PROPERTY_ENUM_DICTIONARY_RESOURCE(enums::VariableDef::Variable, JigsawParameter, variables);
	BIND_PROPERTY_RESOURCE(Audience, audience);

	ClassDB::bind_method(D_METHOD("assign", "state"), &JigsawState::assign);
}

IMPLEMENT_PROPERTY(JigsawState, TypedArray<CardInstance>, cards);
IMPLEMENT_PROPERTY(JigsawState, TypedArray<JigsawSide>, sides);
IMPLEMENT_PROPERTY(JigsawState, VariableParameterDict, variables);
IMPLEMENT_PROPERTY(JigsawState, Ref<Audience>, audience);

void JigsawState::assign(const Ref<JigsawState> &state) {
	ERR_FAIL_COND(state.is_null());

	_cards.resize(state->_cards.size());
	for (int64_t i = 0; i < _cards.size(); i++) {
		Ref<CardInstance> card;
		card.instantiate();
		card->assign(state->_cards[i]);
		_cards[i] = card;
	}

	_sides.resize(state->_sides.size());
	for (int64_t i = 0; i < _sides.size(); i++) {
		Ref<JigsawSide> side;
		side.instantiate();
		side->assign(state->_sides[i]);
		_sides[i] = side;
	}

	_variables.assign(state->_variables);

	_audience.instantiate();
	_audience->assign(state->_audience);
}
