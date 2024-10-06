#include "jigsaw_parameter_string.h"

void JigsawParameterString::_bind_methods() {
	BIND_PROPERTY(Variant::STRING, string);

	ClassDB::bind_static_method("JigsawParameterString", D_METHOD("make", "string"), &JigsawParameterString::make);
}

IMPLEMENT_PROPERTY(JigsawParameterString, String, string);

JigsawParameterString *JigsawParameterString::make(const String &string) {
	JigsawParameterString *param = memnew(JigsawParameterString);
	param->set_string(string);
	return param;
}
