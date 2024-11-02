#ifndef CARD_FILTER_PARAM_H
#define CARD_FILTER_PARAM_H

#include "card_filter.h"

class CardFilterParam : public CardFilter {
	GDCLASS(CardFilterParam, CardFilter);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(Ref<JigsawParameterVariable>, param);

	Type get_type() const override { return PARAM; }
	bool _matches_def(const Ref<JigsawContext> &context, enums::CardDef::Card card) const override;
	bool _matches_instance(const Ref<JigsawContext> &context, const Ref<CardInstance> &inst) const override;
};

#endif // CARD_FILTER_PARAM_H
