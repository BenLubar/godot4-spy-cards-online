#ifndef CARD_FILTER_RANK_H
#define CARD_FILTER_RANK_H

#include "card_filter.h"

class CardFilterRank : public CardFilter {
	GDCLASS(CardFilterRank, CardFilter);

protected:
	static void _bind_methods();

public:
	CardFilterRank() = default;
	~CardFilterRank() = default;

	DECLARE_PROPERTY(enums::RankDef::Rank, rank, = enums::RankDef::Rank::NONE);

	Type get_type() const override { return RANK; }
	bool _matches_def(const Ref<JigsawContext> &ctx, enums::CardDef::Card card) const override;
	bool _matches_instance(const Ref<JigsawContext> &ctx, const Ref<CardInstance> &inst) const override;
};

#endif // CARD_FILTER_RANK_H
