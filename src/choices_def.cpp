#include "choices_def.h"

void ChoicesDef::_bind_methods() {
	BIND_ENUM_CONSTANT(NONE);

	BIND_ENUM_CONSTANT(FIRST_CUSTOM);

	BIND_PROPERTY(Variant::STRING, editor_name);
	BIND_PROPERTY(Variant::PACKED_STRING_ARRAY, editor_choice_names);
}

IMPLEMENT_PROPERTY(ChoicesDef, String, editor_name);
IMPLEMENT_PROPERTY(ChoicesDef, PackedStringArray, editor_choice_names);
