#include "card_filter_or.h"

void CardFilterOr::_bind_methods() {
	BIND_PROPERTY_RESOURCE_ARRAY(CardFilter, list);
}

IMPLEMENT_PROPERTY(CardFilterOr, TypedArray<CardFilter>, list);

Ref<CardFilter> CardFilter::make_or(const TypedArray<CardFilter> &list) {
	TypedArray<CardFilter> filtered_filters = list.duplicate();
	for (int64_t i = 0; i < list.size(); i++) {
		Ref<CardFilter> f = Object::cast_to<CardFilter>(list[i]);
		if (f.is_null()) {
			return Ref<CardFilter>();
		}
	}

	if (filtered_filters.size() == 0) {
		return Ref<CardFilter>();
	}
	if (filtered_filters.size() == 1) {
		return Object::cast_to<CardFilter>(filtered_filters[0]);
	}

	Ref<CardFilterOr> filter;
	filter.instantiate();
	filter->set_list(filtered_filters);

	return filter;
}

bool CardFilterOr::_matches_def(const Ref<JigsawContext> &ctx, enums::CardDef::Card card) const {
	for (int64_t i = 0; i < list.size(); i++) {
		Ref<CardFilter> sub_filter = list[i];
		ERR_CONTINUE(sub_filter.is_null());

		if (sub_filter->matches_def(ctx, card)) {
			return true;
		}
	}

	return false;
}

bool CardFilterOr::_matches_instance(const Ref<JigsawContext> &ctx, const Ref<CardInstance> &inst) const {
	for (int64_t i = 0; i < list.size(); i++) {
		Ref<CardFilter> sub_filter = list[i];
		ERR_CONTINUE(sub_filter.is_null());

		if (sub_filter->matches_instance(ctx, inst)) {
			return true;
		}
	}

	return false;
}
