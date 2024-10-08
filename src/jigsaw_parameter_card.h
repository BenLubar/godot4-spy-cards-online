#ifndef JIGSAW_PARAMETER_CARD_H
#define JIGSAW_PARAMETER_CARD_H

#include "jigsaw_parameter.h"

class JigsawParameterCard;

#include "card_def.h"

class JigsawParameterCard : public JigsawParameter {
	GDCLASS(JigsawParameterCard, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterCard() = default;
	~JigsawParameterCard() = default;

	DECLARE_PROPERTY(CardDef::Card, card, = CardDef::Card::NONE);

	Type get_type() const override { return CARD; }

	static Ref<JigsawParameterCard> make(CardDef::Card card);
};

#endif // JIGSAW_PARAMETER_CARD_H
