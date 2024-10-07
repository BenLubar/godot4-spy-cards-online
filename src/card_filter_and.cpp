#include "card_filter_and.h"

void CardFilterAnd::_bind_methods() {
	BIND_PROPERTY_RESOURCE_ARRAY(CardFilter, sub);
}

IMPLEMENT_PROPERTY(CardFilterAnd, TypedArray<CardFilter>, sub);

Ref<CardFilter> CardFilter::make_and(const TypedArray<CardFilter> &list) {
	TypedArray<CardFilter> filtered_filters;
	for (int64_t i = 0; i < list.size(); i++) {
		CardFilter *f = Object::cast_to<CardFilter>(list[i]);
		if (f) {
			filtered_filters.append(f);
		}
	}

	if (filtered_filters.size() == 0) {
		return Ref<CardFilter>();
	}
	if (filtered_filters.size() == 1) {
		return Object::cast_to<CardFilter>(filtered_filters[0]);
	}

	CardFilterAnd *filter = memnew(CardFilterAnd);
	filter->set_sub(filtered_filters);

	return filter;
}
