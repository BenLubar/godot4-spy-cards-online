#include "why_isnt_this_in_godot.h"

void WhyIsntThisInGodot::_bind_methods() {
	ClassDB::bind_static_method("WhyIsntThisInGodot", D_METHOD("find_builtin_enum_key_name", "type", "enum_name", "enum_value"), &WhyIsntThisInGodot::find_builtin_enum_key_name);
}

String WhyIsntThisInGodot::find_builtin_enum_key_name(const StringName &type, const StringName &enum_name, int64_t enum_value) {
	PackedStringArray names = ClassDB::class_get_enum_constants(type, enum_name);
	for (const String name : names) {
		if (ClassDB::class_get_integer_constant(type, name) == enum_value) {
			return name;
		}
	}

	// failsafe: just use the number if we couldn't find any keys
	return vformat("%s.%s(%d)", type, enum_name, enum_value);
}
