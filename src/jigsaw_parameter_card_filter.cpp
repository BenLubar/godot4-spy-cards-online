#include "jigsaw_parameter_card_filter.h"

void JigsawParameterCardFilter::_bind_methods() {
	BIND_PROPERTY_RESOURCE(CardFilter, filter);

	ClassDB::bind_static_method("JigsawParameterCardFilter", D_METHOD("make", "filter"), &JigsawParameterCardFilter::make);
}

IMPLEMENT_PROPERTY(JigsawParameterCardFilter, Ref<CardFilter>, filter);

Ref<JigsawParameterCardFilter> JigsawParameterCardFilter::make(const Ref<CardFilter> &filter) {
	Ref<JigsawParameterCardFilter> param;
	param.instantiate();
	param->set_filter(filter);
	return param;
}
