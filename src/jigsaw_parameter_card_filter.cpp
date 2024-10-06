#include "jigsaw_parameter_card_filter.h"

void JigsawParameterCardFilter::_bind_methods() {
	BIND_PROPERTY_RESOURCE(CardFilter, filter);
}

IMPLEMENT_PROPERTY(JigsawParameterCardFilter, Ref<CardFilter>, filter);
