#pragma once

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#endif
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/templates/hash_map.hpp>
#include <godot_cpp/templates/hash_set.hpp>
#include <godot_cpp/templates/local_vector.hpp>
#include <godot_cpp/variant/typed_dictionary.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#include <functional>

#if defined(__GNUC__) && defined(DEBUG_ENABLED)
// some warnings that are very useful in development
#pragma GCC diagnostic error "-Wimplicit-fallthrough"
#pragma GCC diagnostic error "-Winit-self"
#pragma GCC diagnostic error "-Wsequence-point"
#pragma GCC diagnostic error "-Wswitch"
#endif

using namespace godot;

#define DEV_ASSERT_ASSUME(m_cond) \
	DEV_ASSERT((m_cond)); \
	[[assume((m_cond))]]

#define DECLARE_PROPERTY(m_type, m_name, ...) \
private: \
	m_type _##m_name __VA_ARGS__; \
public: \
	m_type get_##m_name() const; \
	void set_##m_name(m_type new_##m_name)

#define DECLARE_PROPERTY_IMPLGET(m_type, m_name, ...) \
private: \
	m_type _##m_name __VA_ARGS__; \
public: \
	m_type get_##m_name() const override; \
	void set_##m_name(m_type new_##m_name)

#define DECLARE_PROPERTY_IS(m_type, m_name, ...) \
private: \
	m_type _##m_name __VA_ARGS__; \
public: \
	m_type is_##m_name() const; \
	void set_##m_name(m_type new_##m_name)

#define DECLARE_PROPERTY_OBJECTID(m_type, m_name, ...) \
private: \
	ObjectID _##m_name __VA_ARGS__; \
public: \
	m_type *get_##m_name() const; \
	void set_##m_name(m_type *new_##m_name)

String _spy_cards_enum_property_hint(const String &type_name, bool is_bitfield);

#define BIND_PROPERTY(m_type, m_name, ...) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(m_type, #m_name __VA_OPT__(,) __VA_ARGS__), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_NOT_SAVED(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(m_type, #m_name, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_BITFIELD(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::INT, #m_name, PROPERTY_HINT_FLAGS, _spy_cards_enum_property_hint(enum_qualified_name_to_class_info_name(#m_type), true), PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CLASS_IS_BITFIELD, enum_qualified_name_to_class_info_name(#m_type)), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_ENUM(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::INT, #m_name, PROPERTY_HINT_ENUM, _spy_cards_enum_property_hint(enum_qualified_name_to_class_info_name(#m_type), false), PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CLASS_IS_ENUM, enum_qualified_name_to_class_info_name(#m_type)), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_RESOURCE(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, #m_name, PROPERTY_HINT_RESOURCE_TYPE, #m_type), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_NODE(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, #m_name, PROPERTY_HINT_NODE_TYPE, #m_type), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_RESOURCE_NOT_SAVED(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, #m_name, PROPERTY_HINT_RESOURCE_TYPE, #m_type, PROPERTY_USAGE_EDITOR), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_MULTILINE_TEXT(m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::STRING, #m_name, PROPERTY_HINT_MULTILINE_TEXT), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_VARIANT_ARRAY(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, #m_name, PROPERTY_HINT_TYPE_STRING, itos(m_type) + ":"), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_ENUM_ARRAY(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, #m_name, PROPERTY_HINT_TYPE_STRING, itos(Variant::INT) + "/" + itos(PROPERTY_HINT_ENUM) + ":" + _mm_enum_property_hint(enum_qualified_name_to_class_info_name(#m_type), false)), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_RESOURCE_ARRAY(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, #m_name, PROPERTY_HINT_TYPE_STRING, itos(Variant::OBJECT) + "/" + itos(PROPERTY_HINT_RESOURCE_TYPE) + ":" #m_type), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_RESOURCE_ARRAY_NOT_SAVED(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, #m_name, PROPERTY_HINT_TYPE_STRING, itos(Variant::OBJECT) + "/" + itos(PROPERTY_HINT_RESOURCE_TYPE) + ":" #m_type, PROPERTY_USAGE_EDITOR), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_IS(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("is_" #m_name), &self_type::is_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(m_type, #m_name), "set_" #m_name, "is_" #m_name)

#define BIND_PROPERTY_OBJECTID_NOT_SAVED(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, #m_name, PROPERTY_HINT_RESOURCE_TYPE, #m_type, PROPERTY_USAGE_EDITOR), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_OBJECTID_NODE_NOT_SAVED(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, #m_name, PROPERTY_HINT_NODE_TYPE, #m_type, PROPERTY_USAGE_EDITOR), "set_" #m_name, "get_" #m_name)

#define IMPLEMENT_PROPERTY_ONCHANGE(m_class, m_type, m_name, m_onchange) \
	m_type m_class::get_##m_name() const { return _##m_name; } \
	void m_class::set_##m_name(m_type new_##m_name) { \
		if (_##m_name != new_##m_name) { \
			_##m_name = new_##m_name; \
			m_onchange; \
		} \
	}
#define IMPLEMENT_PROPERTY_ONCHANGE_IS(m_class, m_type, m_name, m_onchange) \
	m_type m_class::is_##m_name() const { return _##m_name; } \
	void m_class::set_##m_name(m_type new_##m_name) { \
		if (_##m_name != new_##m_name) { \
			_##m_name = new_##m_name; \
			m_onchange; \
		} \
	}
#define IMPLEMENT_PROPERTY(m_class, m_type, m_name) \
	IMPLEMENT_PROPERTY_ONCHANGE(m_class, m_type, m_name, emit_changed())
#define IMPLEMENT_PROPERTY_IS(m_class, m_type, m_name) \
	IMPLEMENT_PROPERTY_ONCHANGE_IS(m_class, m_type, m_name, emit_changed())
#define IMPLEMENT_PROPERTY_SIMPLE(m_class, m_type, m_name) \
	IMPLEMENT_PROPERTY_ONCHANGE(m_class, m_type, m_name, )
#define IMPLEMENT_PROPERTY_SIMPLE_IS(m_class, m_type, m_name) \
	IMPLEMENT_PROPERTY_ONCHANGE_IS(m_class, m_type, m_name, )
#define IMPLEMENT_PROPERTY_OBJECTID_ONCHANGE(m_class, m_type, m_name, m_onchange) \
	m_type *m_class::get_##m_name() const { return Object::cast_to<m_type>(ObjectDB::get_instance(_##m_name)); } \
	void m_class::set_##m_name(m_type *new_##m_name) { \
		_##m_name = new_##m_name ? new_##m_name->get_instance_id() : ObjectID(); \
		m_onchange; /* always calls onchange for now; decide whether this needs to check the old value eventually */ \
	}
#define IMPLEMENT_PROPERTY_OBJECTID_SIMPLE(m_class, m_type, m_name) \
	IMPLEMENT_PROPERTY_OBJECTID_ONCHANGE(m_class, m_type, m_name, )
#define IMPLEMENT_PROPERTY_ONSET(m_class, m_type, m_name, m_onset) \
	m_type m_class::get_##m_name() const { return _##m_name; } \
	void m_class::set_##m_name(m_type new_##m_name) { \
		_##m_name = new_##m_name; \
		m_onset; \
	}
#define IMPLEMENT_PROPERTY_ONCHANGE_SIGNAL(m_class, m_type, m_name, m_onchange) \
	m_type m_class::get_##m_name() const { return _##m_name; } \
	void m_class::set_##m_name(m_type new_##m_name) { \
		if (_##m_name != new_##m_name) { \
			if (_##m_name.is_valid()) { \
				_##m_name->disconnect(CHANGED, callable_mp(this, &m_class::m_onchange)); \
			} \
			_##m_name = new_##m_name; \
			if (_##m_name.is_valid()) { \
				_##m_name->connect(CHANGED, callable_mp(this, &m_class::m_onchange)); \
			} \
			m_onchange(); \
		} \
	}
#define IMPLEMENT_PROPERTY_SIGNAL(m_class, m_type, m_name, m_signal, m_callback) \
	m_type m_class::get_##m_name() const { return _##m_name; } \
	void m_class::set_##m_name(m_type new_##m_name) { \
		if (_##m_name != new_##m_name) { \
			if (_##m_name) { \
				_##m_name->disconnect(m_signal, callable_mp(this, m_callback)); \
			} \
			_##m_name = new_##m_name; \
			if (_##m_name) { \
				_##m_name->connect(m_signal, callable_mp(this, m_callback)); \
			} \
		} \
	}
#define IMPLEMENT_PROPERTY_ARRAY_CHANGED(m_class, m_type, m_name, m_callback) \
	m_type m_class::get_##m_name() const { return _##m_name; } \
	void m_class::set_##m_name(m_type new_##m_name) { \
		for (int64_t i = 0; i < _##m_name.size(); i++) { \
			Object *item = _##m_name[i]; \
			if (item) { \
				item->disconnect(CHANGED, callable_mp(this, m_callback)); \
			} \
		} \
		_##m_name = new_##m_name; \
		for (int64_t i = 0; i < _##m_name.size(); i++) { \
			Object *item = _##m_name[i]; \
			if (item) { \
				item->connect(CHANGED, callable_mp(this, m_callback)); \
			} \
		} \
		callable_mp(this, m_callback).call(); \
	}
#define IMPLEMENT_PROPERTY_ARRAY_CHANGED_ITEM(m_class, m_type, m_name, m_callback) \
	m_type m_class::get_##m_name() const { return _##m_name; } \
	void m_class::set_##m_name(m_type new_##m_name) { \
		for (int64_t i = 0; i < _##m_name.size(); i++) { \
			Object *item = _##m_name[i]; \
			if (item) { \
				item->disconnect(CHANGED, callable_mp(this, m_callback).bind(item)); \
			} \
		} \
		_##m_name = new_##m_name; \
		for (int64_t i = 0; i < _##m_name.size(); i++) { \
			Object *item = _##m_name[i]; \
			if (item) { \
				item->connect(CHANGED, callable_mp(this, m_callback).bind(item)); \
				callable_mp(this, m_callback).call(item); \
			} \
		} \
	}

extern Vector<std::function<void()>> _free_lazy_globals;
extern thread_local bool lazy_globals_must_be_first_used_from_the_main_thread;
template<typename T>
class LazyGlobal {
	std::function<Ref<T>()> _init;
	mutable Ref<T> _ref;
	mutable bool _initialized = false;

	void _maybe_init() const {
		if (unlikely(!_initialized)) {
			DEV_ASSERT(lazy_globals_must_be_first_used_from_the_main_thread);

			_initialized = true;
			_ref = _init();
			_free_lazy_globals.append([this]() -> void { _ref = Ref<T>(); _initialized = false; });
		}
	}
public:
	explicit LazyGlobal(const std::function<Ref<T>()> &init) : _init(init) {}

	operator Ref<T> () const { _maybe_init(); return _ref; }
	T *operator*() const { _maybe_init(); return _ref.ptr(); }
	T *operator->() const { _maybe_init(); return _ref.ptr(); }
	void operator=(const Ref<T> &value) { _maybe_init(); _ref = value; }
};

extern std::vector<std::function<void()>> _lazy_init;

template<typename T>
class LazyResource : public LazyGlobal<T> {
public:
	explicit LazyResource(const char *path) : LazyGlobal<T>{ [path]() -> Ref<T> {
		return ResourceLoader::get_singleton()->load(path, T::get_class_static());
	} } {}
};

template<typename T, typename M>
class LazyVariant {
	const M _init;
	mutable T *_variant = nullptr;

	void _maybe_init() const {
		if (unlikely(!_variant)) {
			DEV_ASSERT(lazy_globals_must_be_first_used_from_the_main_thread);

			_variant = memnew(T(_init));
			_free_lazy_globals.append([this]() -> void { if (_variant) { memdelete(_variant); _variant = nullptr; } });
		}
	}
public:
	LazyVariant(const M &init) : _init(init) {}

	operator T &() const { _maybe_init(); return *_variant; }
	T &operator*() const { _maybe_init(); return *_variant; }
	T *operator->() const { _maybe_init(); return _variant; }
};

template<typename T>
class LazyVariant<T, void> {
	mutable T *_variant = nullptr;

	void _maybe_init() const {
		if (unlikely(!_variant)) {
			DEV_ASSERT(lazy_globals_must_be_first_used_from_the_main_thread);

			_variant = memnew(T);
			_free_lazy_globals.append([this]() -> void { if (_variant) { memdelete(_variant); _variant = nullptr; } });
		}
	}
public:
	LazyVariant() {}

	operator T &() const { _maybe_init(); return *_variant; }
	T &operator*() const { _maybe_init(); return *_variant; }
	T *operator->() const { _maybe_init(); return _variant; }
};

using LazyStringName = LazyVariant<StringName, const char *>;
using LazyNodePath = LazyVariant<NodePath, const char *>;

Pair<uint64_t, int64_t> uvarint(const PackedByteArray &p_buffer, int64_t p_offset = 0);
Pair<int64_t, int64_t> svarint(const PackedByteArray &p_buffer, int64_t p_offset = 0);
void append_uvarint(PackedByteArray &p_buffer, uint64_t p_value);
void append_svarint(PackedByteArray &p_buffer, int64_t p_value);
