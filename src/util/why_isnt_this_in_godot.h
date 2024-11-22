#ifndef WHY_ISNT_THIS_IN_GODOT_H
#define WHY_ISNT_THIS_IN_GODOT_H

#include "dry.h"

class WhyIsntThisInGodot : public Object {
	GDCLASS(WhyIsntThisInGodot, Object);

protected:
	static void _bind_methods();

public:
	static String find_builtin_enum_key_name(const StringName &type, const StringName &enum_name, int64_t enum_value);
	static uint64_t get_directory_size(const String &path);
};

#endif // WHY_ISNT_THIS_IN_GODOT_H
