#include <godot_cpp/classes/gd_script.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

template<typename T>
struct PredefinedScriptKey {
};

#define DECLARE_PREDEFINED_KEY(m_type, m_name) \
	template<> \
	struct PredefinedScriptKey<m_type> { \
		static constexpr char name[] = #m_name; \
	}

inline Ref<GDScript> get_predefined_defs() {
	static LazyGlobalFile<GDScript> defs{"res://predefined/predefined.gd"};

	return defs;
}

template<typename T, typename E>
Ref<T> get_predefined(E index) {
	if (index == -1) {
		return Ref<T>();
	}

	Ref<GDScript> predefined_defs = get_predefined_defs();
	ERR_FAIL_COND_V_MSG(predefined_defs.is_null(), Ref<T>(), "failed to load predefined defs script");

	Array predefined = predefined_defs->get(PredefinedScriptKey<T>::name);

	ERR_FAIL_INDEX_V_MSG(index, predefined.size(), Ref<T>(), vformat("%s %d outside of predefined range", PredefinedScriptKey<T>::name, index));

	Ref<T> def = predefined[index];
	ERR_FAIL_COND_V_MSG(def.is_null(), Ref<T>(), vformat("predefined %s %d is null", PredefinedScriptKey<T>::name, index));

	return def;
}

template<typename T>
class LazyPredefined : public LazyGlobal<T> {
public:
	LazyPredefined(const char *name) : LazyGlobal<T>([name]() -> Ref<T> {
		Ref<GDScript> defs = get_predefined_defs();
		ERR_FAIL_COND_V(defs.is_null(), Ref<T>());
		Ref<T> value = defs->get(name);
		if (unlikely(value.is_null())) {
			ERR_PRINT(vformat("failed to retrieve value Predefined.%s; check to make sure predefined.gd contains this key.", name));
		}
		return value;
	}) {}
};
