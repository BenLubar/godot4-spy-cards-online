#include "card_filter_tribe.h"

void CardFilterTribe::_bind_methods() {
	BIND_PROPERTY_ENUM(TribeDef::Tribe, tribe);
}

IMPLEMENT_PROPERTY(CardFilterTribe, TribeDef::Tribe, tribe);

Ref<CardFilter> CardFilter::make_tribe(TribeDef::Tribe tribe) {
	Ref<CardFilterTribe> filter;
	filter.instantiate();
	filter->set_tribe(tribe);
	return filter;
}
