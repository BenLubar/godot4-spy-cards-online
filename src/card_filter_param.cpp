#include "card_filter_param.h"

void CardFilterParam::_bind_methods() {
	BIND_PROPERTY_RESOURCE(JigsawParameter, param);
}

IMPLEMENT_PROPERTY(CardFilterParam, Ref<JigsawParameter>, param);

Ref<CardFilter> CardFilter::make_param(const Ref<JigsawParameter> &param) {
	if (param.is_null()) {
		return Ref<CardFilter>();
	}

	Ref<CardFilterParam> filter;
	filter.instantiate();
	filter->set_param(param);
	return filter;
}

bool CardFilterParam::_matches_def(const Ref<JigsawContext> &ctx, enums::CardDef::Card card) const {
	ERR_FAIL_COND_V(ctx.is_null(), false);
	JigsawGlobal *global = ctx->get_global();
	ERR_FAIL_NULL_V(global, false);
	Ref<GameMode> mode = global->get_mode();
	ERR_FAIL_COND_V(mode.is_null(), false);
	Ref<CardDef> def = mode->get_card(card);
	ERR_FAIL_COND_V(def.is_null(), false);

	return false; // TODO!
}

bool CardFilterParam::_matches_instance(const Ref<JigsawContext> &ctx, const Ref<CardInstance> &inst) const {
	return false; // TODO!
}
