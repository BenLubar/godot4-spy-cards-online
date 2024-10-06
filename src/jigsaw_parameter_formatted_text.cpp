#include "jigsaw_parameter_formatted_text.h"

void JigsawParameterFormattedText::_bind_methods() {
	BIND_PROPERTY(Variant::ARRAY, text, PROPERTY_HINT_TYPE_STRING, String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":FormattedText");
}

IMPLEMENT_PROPERTY(JigsawParameterFormattedText, TypedArray<FormattedText>, text);
