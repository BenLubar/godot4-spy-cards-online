#include <godot_cpp/classes/resource_loader.hpp>

template<typename T>
struct PredefinedScriptKey
{
};

#define DECLARE_PREDEFINED_KEY(m_type, m_name) \
	template<> \
	struct PredefinedScriptKey<m_type> \
	{ \
		static constexpr char name[] = #m_name; \
	}

template<typename T, typename E>
Ref<T> get_predefined(E index)
{
	if (index == -1) {
		return Ref<T>();
	}

	// TODO: more efficient way of doing this than calling load every time
	Ref<Resource> predefined_defs = ResourceLoader::get_singleton()->load("res://predefined/predefined.gd", "GDScript");
	ERR_FAIL_COND_V_MSG(predefined_defs.is_null(), Ref<T>(), "failed to load predefined defs script");

	Array predefined = predefined_defs->get(PredefinedScriptKey<T>::name);

	ERR_FAIL_INDEX_V_MSG(index, predefined.size(), Ref<T>(), String(PredefinedScriptKey<T>::name) + " outside of predefined range");

	Ref<T> def = predefined[index];
	ERR_FAIL_COND_V_MSG(def.is_null(), Ref<T>(), String("predefined ") + PredefinedScriptKey<T>::name + " is null");

	return def;
}
