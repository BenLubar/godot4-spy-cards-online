#include "jigsaw_parameter_card_filter.h"

void JigsawParameterCardFilter::_bind_methods() {
	BIND_PROPERTY(Variant::OBJECT, filter, PROPERTY_HINT_RESOURCE_TYPE, "CardFilter");
}

IMPLEMENT_PROPERTY(JigsawParameterCardFilter, Ref<CardFilter>, filter);
