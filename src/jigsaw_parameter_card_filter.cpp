#include "jigsaw_parameter_card_filter.h"

void JigsawParameterCardFilter::_bind_methods() {
	BIND_PROPERTY_RESOURCE(CardFilter, filter);

	ClassDB::bind_static_method("JigsawParameterCardFilter", D_METHOD("make", "filter"), &JigsawParameterCardFilter::make);
}

IMPLEMENT_PROPERTY(JigsawParameterCardFilter, Ref<CardFilter>, filter);

JigsawParameterCardFilter *JigsawParameterCardFilter::make(CardFilter *filter) {
	JigsawParameterCardFilter *param = memnew(JigsawParameterCardFilter);
	if (filter) {
		param->set_filter(filter);
	}
	return param;
}
