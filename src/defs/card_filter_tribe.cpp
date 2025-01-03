#include "defs/card_filter_tribe.h"

#include "jigsaw/jigsaw_global.h"

void CardFilterTribe::_bind_methods() {
	BIND_PROPERTY_ENUM(enums::TribeDef::Tribe, tribe);
}

IMPLEMENT_PROPERTY(CardFilterTribe, enums::TribeDef::Tribe, tribe);

Ref<CardFilter> CardFilter::make_tribe(enums::TribeDef::Tribe tribe) {
	ERR_FAIL_COND_V_MSG(tribe == enums::TribeDef::LEGACY_NONE, Ref<CardFilter>(), "calling code needs to be updated to handle TribeDef::LEGACY_NONE");

	Ref<CardFilterTribe> filter;
	filter.instantiate();
	filter->set_tribe(tribe);
	return filter;
}

bool CardFilterTribe::_matches_def(const Ref<JigsawContext> &context, enums::CardDef::Card card) const {
	ERR_FAIL_COND_V(context.is_null(), false);
	JigsawGlobal *global = context->get_global();
	ERR_FAIL_NULL_V(global, false);
	Ref<GameMode> mode = global->get_mode();
	ERR_FAIL_COND_V(mode.is_null(), false);
	Ref<CardDef> def = mode->get_card(card);
	ERR_FAIL_COND_V(def.is_null(), false);

	return def->get_tribes().has(get_tribe());
}

bool CardFilterTribe::_matches_instance(const Ref<JigsawContext> &context, const Ref<CardInstance> &inst) const {
	ERR_FAIL_COND_V(inst.is_null(), false);

	return inst->get_tribes().has(get_tribe());
}
