#include "card_filter_param.h"

#include "jigsaw_global.h"

void CardFilterParam::_bind_methods() {
	BIND_PROPERTY_RESOURCE(JigsawParameterVariable, param);
}

IMPLEMENT_PROPERTY(CardFilterParam, Ref<JigsawParameterVariable>, param);

Ref<CardFilter> CardFilter::make_param(const Ref<JigsawParameterVariable> &param) {
	if (param.is_null()) {
		return Ref<CardFilter>();
	}

	Ref<CardFilterParam> filter;
	filter.instantiate();
	filter->set_param(param);
	return filter;
}

bool CardFilterParam::_matches_def(const Ref<JigsawContext> &context, enums::CardDef::Card card) const {
	ERR_FAIL_COND_V(context.is_null(), false);
	JigsawGlobal *global = context->get_global();
	ERR_FAIL_NULL_V(global, false);
	Ref<GameMode> mode = global->get_mode();
	ERR_FAIL_COND_V(mode.is_null(), false);
	Ref<CardDef> def = mode->get_card(card);
	ERR_FAIL_COND_V(def.is_null(), false);

	return false; // TODO!
}

bool CardFilterParam::_matches_instance(const Ref<JigsawContext> &context, const Ref<CardInstance> &inst) const {
	return false; // TODO!
}
