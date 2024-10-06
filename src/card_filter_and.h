#ifndef CARD_FILTER_AND_H
#define CARD_FILTER_AND_H

#include "dry.h"

#include "card_filter.h"

class CardFilterAnd : public CardFilter {
	GDCLASS(CardFilterAnd, CardFilter);

protected:
	static void _bind_methods();

public:
	CardFilterAnd() = default;
	~CardFilterAnd() = default;

	DECLARE_PROPERTY(TypedArray<CardFilter>, sub);

	Type get_type() const override { return AND; }
};

#endif // CARD_FILTER_AND_H
