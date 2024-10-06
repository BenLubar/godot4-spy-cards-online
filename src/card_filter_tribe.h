#ifndef CARD_FILTER_TRIBE_H
#define CARD_FILTER_TRIBE_H

#include "dry.h"

#include "card_filter.h"

class CardFilterTribe : public CardFilter {
	GDCLASS(CardFilterTribe, CardFilter);

protected:
	static void _bind_methods();

public:
	CardFilterTribe() = default;
	~CardFilterTribe() = default;

	DECLARE_PROPERTY(TribeDef::Tribe, tribe, = TribeDef::Tribe::NONE);

	Type get_type() const override { return TRIBE; }
};

#endif // CARD_FILTER_TRIBE_H
