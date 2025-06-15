#pragma once

#include "dry.h"

class WhyIsntThisInGodot : public Object {
	GDCLASS(WhyIsntThisInGodot, Object);

protected:
	static void _bind_methods();

public:
	static String find_builtin_enum_key_name(const StringName &p_type, const StringName &p_enum_name, int64_t p_enum_value);
	static uint64_t get_directory_size(const String &p_path);
	static Vector3i get_project_version_vector();
};
