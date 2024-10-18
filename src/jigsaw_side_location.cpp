#include "jigsaw_side_location.h"

void JigsawSideLocation::_bind_methods() {
	BIND_PROPERTY_RESOURCE_ARRAY(CardInstance, cards);
}

IMPLEMENT_PROPERTY_SIMPLE(JigsawSideLocation, TypedArray<CardInstance>, cards);
