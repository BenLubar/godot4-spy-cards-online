#ifndef CARD_FILTER_OR_H
#define CARD_FILTER_OR_H

#include "dry.h"

#include "card_filter.h"

class CardFilterOr : public CardFilter {
	GDCLASS(CardFilterOr, CardFilter);

protected:
	static void _bind_methods();

public:
	CardFilterOr() = default;
	~CardFilterOr() = default;

	DECLARE_PROPERTY(TypedArray<CardFilter>, sub);

	Type get_type() const override { return OR; }
};

#endif // CARD_FILTER_OR_H
