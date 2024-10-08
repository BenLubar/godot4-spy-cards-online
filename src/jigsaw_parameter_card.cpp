#include "jigsaw_parameter_card.h"

void JigsawParameterCard::_bind_methods() {
	BIND_PROPERTY(Variant::INT, card);

	ClassDB::bind_static_method("JigsawParameterCard", D_METHOD("make", "card"), &JigsawParameterCard::make);
}

IMPLEMENT_PROPERTY(JigsawParameterCard, CardDef::Card, card);

Ref<JigsawParameterCard> JigsawParameterCard::make(CardDef::Card card) {
	Ref<JigsawParameterCard> param;
	param.instantiate();
	param->set_card(card);
	return param;
}
