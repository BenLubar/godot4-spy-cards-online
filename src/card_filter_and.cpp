#include "card_filter_and.h"

void CardFilterAnd::_bind_methods() {
	BIND_PROPERTY_RESOURCE_ARRAY(CardFilter, list);
}

IMPLEMENT_PROPERTY(CardFilterAnd, TypedArray<CardFilter>, list);

Ref<CardFilter> CardFilter::make_and(const TypedArray<CardFilter> &list) {
	TypedArray<CardFilter> filtered_filters;
	for (int64_t i = 0; i < list.size(); i++) {
		Ref<CardFilter> f = Object::cast_to<CardFilter>(list[i]);
		if (f.is_valid()) {
			filtered_filters.append(f);
		}
	}

	if (filtered_filters.size() == 0) {
		return Ref<CardFilter>();
	}
	if (filtered_filters.size() == 1) {
		return Object::cast_to<CardFilter>(filtered_filters[0]);
	}

	Ref<CardFilterAnd> filter;
	filter.instantiate();
	filter->set_list(filtered_filters);

	return filter;
}

bool CardFilterAnd::_matches_def(const Ref<JigsawContext> &ctx, enums::CardDef::Card card) const {
	for (int64_t i = 0; i < list.size(); i++) {
		Ref<CardFilter> sub_filter = list[i];
		ERR_CONTINUE(sub_filter.is_null());

		if (!sub_filter->matches_def(ctx, card)) {
			return false;
		}
	}

	return true;
}

bool CardFilterAnd::_matches_instance(const Ref<JigsawContext> &ctx, const Ref<CardInstance> &inst) const {
	for (int64_t i = 0; i < list.size(); i++) {
		Ref<CardFilter> sub_filter = list[i];
		ERR_CONTINUE(sub_filter.is_null());

		if (!sub_filter->matches_instance(ctx, inst)) {
			return false;
		}
	}

	return true;
}
