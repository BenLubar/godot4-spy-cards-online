#include "jigsaw_parameter_string.h"

void JigsawParameterString::_bind_methods() {
	BIND_PROPERTY(Variant::STRING, string);

	ClassDB::bind_static_method("JigsawParameterString", D_METHOD("make", "string"), &JigsawParameterString::make);
}

IMPLEMENT_PROPERTY(JigsawParameterString, String, string);

Ref<JigsawParameterString> JigsawParameterString::make(const String &string) {
	Ref<JigsawParameterString> param;
	param.instantiate();
	param->set_string(string);
	return param;
}
