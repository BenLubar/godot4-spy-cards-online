#include "jigsaw_parameter_card.h"

void JigsawParameterCard::_bind_methods() {
	BIND_PROPERTY(Variant::INT, card);
}

IMPLEMENT_PROPERTY(JigsawParameterCard, CardDef::Card, card);
