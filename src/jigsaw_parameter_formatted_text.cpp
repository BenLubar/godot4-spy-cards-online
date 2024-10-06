#include "jigsaw_parameter_formatted_text.h"

void JigsawParameterFormattedText::_bind_methods() {
	BIND_PROPERTY_RESOURCE_ARRAY(FormattedText, text);
}

IMPLEMENT_PROPERTY(JigsawParameterFormattedText, TypedArray<FormattedText>, text);
