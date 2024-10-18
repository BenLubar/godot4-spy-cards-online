#include "card_filter_rank.h"

#include "jigsaw_global.h"

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

bool CardFilterRank::_matches_def(const Ref<JigsawContext> &ctx, enums::CardDef::Card card) const {
	ERR_FAIL_COND_V(ctx.is_null(), false);
	JigsawGlobal *global = ctx->get_global();
	ERR_FAIL_NULL_V(global, false);
	Ref<GameMode> mode = global->get_mode();
	ERR_FAIL_COND_V(mode.is_null(), false);
	Ref<CardDef> def = mode->get_card(card);
	ERR_FAIL_COND_V(def.is_null(), false);

	return get_rank() == def->get_rank();
}

bool CardFilterRank::_matches_instance(const Ref<JigsawContext> &ctx, const Ref<CardInstance> &inst) const {
	ERR_FAIL_COND_V(inst.is_null(), false);

	return get_rank() == inst->get_rank();
}
