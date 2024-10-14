#ifndef CARD_FILTER_PARAM_H
#define CARD_FILTER_PARAM_H

#include "card_filter.h"

class CardFilterParam;

#include "jigsaw_parameter.h"

class CardFilterParam : public CardFilter {
	GDCLASS(CardFilterParam, CardFilter);

protected:
	static void _bind_methods();

public:
	CardFilterParam() = default;
	~CardFilterParam() = default;

	DECLARE_PROPERTY(Ref<JigsawParameter>, param);

	Type get_type() const override { return PARAM; }
	bool _matches_def(const Ref<JigsawContext> &ctx, enums::CardDef::Card card) const override;
	bool _matches_instance(const Ref<JigsawContext> &ctx, const Ref<CardInstance> &inst) const override;
};

#endif // CARD_FILTER_PARAM_H
