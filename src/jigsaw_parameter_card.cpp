#include "jigsaw_parameter_card.h"

void JigsawParameterCard::_bind_methods() {
	BIND_PROPERTY_ENUM(enums::CardDef::Card, card);

	ClassDB::bind_static_method("JigsawParameterCard", D_METHOD("make", "card"), &JigsawParameterCard::make);
}

IMPLEMENT_PROPERTY(JigsawParameterCard, enums::CardDef::Card, card);

Ref<JigsawParameterCard> JigsawParameterCard::make(enums::CardDef::Card card) {
	Ref<JigsawParameterCard> param;
	param.instantiate();
	param->set_card(card);
	return param;
}
