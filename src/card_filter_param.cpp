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
