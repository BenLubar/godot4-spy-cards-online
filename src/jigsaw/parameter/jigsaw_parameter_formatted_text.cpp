#include "jigsaw_parameter_formatted_text.h"

void JigsawParameterFormattedText::_bind_methods() {
	BIND_PROPERTY_RESOURCE_ARRAY(FormattedText, text);

	ClassDB::bind_static_method("JigsawParameterFormattedText", D_METHOD("make", "text"), &JigsawParameterFormattedText::make);
}

IMPLEMENT_PROPERTY(JigsawParameterFormattedText, TypedArray<FormattedText>, text);

Ref<JigsawParameterFormattedText> JigsawParameterFormattedText::make(const TypedArray<FormattedText> &text) {
	Ref<JigsawParameterFormattedText> param;
	param.instantiate();
	param->set_text(text);
	return param;
}
