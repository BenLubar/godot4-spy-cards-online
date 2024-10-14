#include "card_filter_card.h"

void CardFilterCard::_bind_methods() {
	BIND_PROPERTY_ENUM(CardDef::Card, card);
}

IMPLEMENT_PROPERTY(CardFilterCard, CardDef::Card, card);

Ref<CardFilter> CardFilter::make_card(CardDef::Card card) {
	Ref<CardFilterCard> filter;
	filter.instantiate();
	filter->set_card(card);
	return filter;
}

bool CardFilterCard::_matches_def(const Ref<JigsawContext> &ctx, enums::CardDef::Card card) const {
	return get_card() == card;
}

bool CardFilterCard::_matches_instance(const Ref<JigsawContext> &ctx, const Ref<CardInstance> &inst) const {
	ERR_FAIL_COND_V(inst.is_null(), false);
	Ref<CardDef> def = inst->get_def();
	ERR_FAIL_COND_V(def.is_null(), false);

	return get_card() == def->get_id();
}
