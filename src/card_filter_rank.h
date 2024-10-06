#ifndef CARD_FILTER_RANK_H
#define CARD_FILTER_RANK_H

#include "dry.h"

#include "card_filter.h"

class CardFilterRank : public CardFilter {
	GDCLASS(CardFilterRank, CardFilter);

protected:
	static void _bind_methods();

public:
	CardFilterRank() = default;
	~CardFilterRank() = default;

	DECLARE_PROPERTY(RankDef::Rank, rank, = RankDef::Rank::NONE);

	Type get_type() const override { return RANK; }
};

#endif // CARD_FILTER_RANK_H
