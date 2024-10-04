#include <godot_cpp/classes/resource_loader.hpp>

template<typename T, typename E>
Ref<T> get_predefined(String type_name, E index)
{
	if (index == -1) {
		return nullptr;
	}

	// TODO: more efficient way of doing this than calling load every time
	Ref<Resource> predefined_defs = ResourceLoader::get_singleton()->load("res://predefined/predefined.gd", "GDScript");
	ERR_FAIL_COND_V_MSG(predefined_defs.is_null(), nullptr, "failed to load predefined defs script");

	Array predefined = predefined_defs->get(type_name);

	ERR_FAIL_INDEX_V_MSG(index, predefined.size(), nullptr, type_name + " outside of predefined range");

	Ref<T> def = predefined[index];
	ERR_FAIL_COND_V_MSG(def.is_null(), nullptr, "predefined " + type_name + " is null");

	return def;
}
