#include "util/why_isnt_this_in_godot.h"

#include <filesystem>

#include <godot_cpp/classes/project_settings.hpp>

void WhyIsntThisInGodot::_bind_methods() {
	ClassDB::bind_static_method("WhyIsntThisInGodot", D_METHOD("find_builtin_enum_key_name", "type", "enum_name", "enum_value"), &WhyIsntThisInGodot::find_builtin_enum_key_name);
	ClassDB::bind_static_method("WhyIsntThisInGodot", D_METHOD("get_directory_size", "path"), &WhyIsntThisInGodot::get_directory_size);
	ClassDB::bind_static_method("WhyIsntThisInGodot", D_METHOD("get_project_version_vector"), &WhyIsntThisInGodot::get_project_version_vector);
}

String WhyIsntThisInGodot::find_builtin_enum_key_name(const StringName &type, const StringName &enum_name, int64_t enum_value) {
	PackedStringArray names = ClassDB::class_get_enum_constants(type, enum_name);
	for (int64_t i = 0; i < names.size(); i++) {
		if (ClassDB::class_get_integer_constant(type, names[i]) == enum_value) {
			return names[i];
		}
	}

	// failsafe: just use the number if we couldn't find any keys
	return vformat("%s.%s(%d)", type, enum_name, enum_value);
}

uint64_t WhyIsntThisInGodot::get_directory_size(const String &path) {
	std::error_code ec;
	uint64_t total_size = 0;
	for (const std::filesystem::directory_entry &entry : std::filesystem::directory_iterator{path.utf8().ptr(), ec}) {
		if (entry.is_regular_file(ec)) {
			total_size += entry.file_size(ec);
		}
	}

	return total_size;
}

Vector3i WhyIsntThisInGodot::get_project_version_vector() {
	String string_version = ProjectSettings::get_singleton()->get("application/config/version");

	PackedStringArray string_array_version = string_version.split(".");

	ERR_FAIL_COND_V(string_array_version.size() != 3, Vector3i());
	ERR_FAIL_COND_V(!string_array_version[0].is_valid_int(), Vector3i());
	ERR_FAIL_COND_V(!string_array_version[1].is_valid_int(), Vector3i());
	ERR_FAIL_COND_V(!string_array_version[2].is_valid_int(), Vector3i());

	return Vector3i(string_array_version[0].to_int(), string_array_version[1].to_int(), string_array_version[2].to_int());
}
