#include "card_filter_rank.h"

void CardFilterRank::_bind_methods() {
	BIND_PROPERTY_ENUM(RankDef::Rank, rank);
}

IMPLEMENT_PROPERTY(CardFilterRank, RankDef::Rank, rank);

Ref<CardFilter> CardFilter::make_rank(RankDef::Rank rank) {
	Ref<CardFilterRank> filter;
	filter.instantiate();
	filter->set_rank(rank);
	return filter;
}
