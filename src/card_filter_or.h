#ifndef CARD_FILTER_OR_H
#define CARD_FILTER_OR_H

#include "card_filter.h"

class CardFilterOr : public CardFilter {
	GDCLASS(CardFilterOr, CardFilter);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(TypedArray<CardFilter>, list);

	Type get_type() const override { return OR; }
	bool _matches_def(const Ref<JigsawContext> &context, enums::CardDef::Card card) const override;
	bool _matches_instance(const Ref<JigsawContext> &context, const Ref<CardInstance> &inst) const override;
};

#endif // CARD_FILTER_OR_H
