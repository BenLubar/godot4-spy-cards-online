#include "card_filter_cost.h"

void CardFilterCost::_bind_methods() {
	BIND_PROPERTY_ENUM(StatDef::Stat, stat);
	BIND_PROPERTY(Variant::INT, amount);
	BIND_PROPERTY(Variant::INT, amount_inf);
	BIND_PROPERTY_IS(Variant::BOOL, nan);
}

IMPLEMENT_PROPERTY(CardFilterCost, StatDef::Stat, stat);
IMPLEMENT_PROPERTY(CardFilterCost, int64_t, amount);
IMPLEMENT_PROPERTY(CardFilterCost, int64_t, amount_inf);
IMPLEMENT_PROPERTY_IS(CardFilterCost, bool, nan);

Ref<CardFilter> CardFilter::make_cost(StatDef::Stat stat, int64_t amount, int64_t amount_inf) {
	Ref<CardFilterCost> filter;
	filter.instantiate();
	filter->set_stat(stat);
	filter->set_amount(amount);
	filter->set_amount_inf(amount_inf);
	return filter;
}

Ref<CardFilter> CardFilter::make_cost_nan(StatDef::Stat stat) {
	Ref<CardFilterCost> filter;
	filter.instantiate();
	filter->set_stat(stat);
	filter->set_nan(true);
	return filter;
}

bool CardFilterCost::_matches_costs(const TypedArray<StatValue> &costs) const {
	for (int64_t i = 0; i < costs.size(); i++) {
		Ref<StatValue> cost = costs[i];
		ERR_CONTINUE(cost.is_null());

		if (get_stat() != cost->get_stat()) {
			continue;
		}

		if (is_nan() && cost->is_nan()) {
			return true;
		}
		if (is_nan() || cost->is_nan()) {
			return false;
		}

		if (get_amount_inf() != 0 || cost->get_amount_inf() != 0) {
			if (get_amount_inf() > 0 && cost->get_amount_inf() > 0) {
				return true;
			}
			if (get_amount_inf() < 0 && cost->get_amount_inf() < 0) {
				return true;
			}
			return false;
		}

		return get_amount() == cost->get_amount();
	}

	return false;
}

bool CardFilterCost::_matches_def(const Ref<JigsawContext> &ctx, enums::CardDef::Card card) const {
	ERR_FAIL_COND_V(ctx.is_null(), false);
	JigsawGlobal *global = ctx->get_global();
	ERR_FAIL_NULL_V(global, false);
	Ref<GameMode> mode = global->get_mode();
	ERR_FAIL_COND_V(mode.is_null(), false);
	Ref<CardDef> def = mode->get_card(card);
	ERR_FAIL_COND_V(def.is_null(), false);

	return _matches_costs(def->get_costs());
}

bool CardFilterCost::_matches_instance(const Ref<JigsawContext> &ctx, const Ref<CardInstance> &inst) const {
	ERR_FAIL_COND_V(inst.is_null(), false);

	return _matches_costs(inst->get_costs());
}
