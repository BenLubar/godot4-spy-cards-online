#ifndef DRY_H
#define DRY_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>
#include <godot_cpp/variant/variant_internal.hpp>

#include <functional>

using namespace godot;

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

#define BIND_PROPERTY(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(m_type, #m_name), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_NOT_SAVED(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(m_type, #m_name, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_BITFIELD(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::INT, #m_name, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CLASS_IS_BITFIELD, enum_qualified_name_to_class_info_name(#m_type)), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_ENUM(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::INT, #m_name, PROPERTY_HINT_NONE, "", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_CLASS_IS_ENUM, enum_qualified_name_to_class_info_name(#m_type)), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_RESOURCE(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, #m_name, PROPERTY_HINT_RESOURCE_TYPE, #m_type), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_RESOURCE_NOT_SAVED(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, #m_name, PROPERTY_HINT_RESOURCE_TYPE, #m_type, PROPERTY_USAGE_NONE), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_MULTILINE_TEXT(m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::STRING, #m_name, PROPERTY_HINT_MULTILINE_TEXT), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_VARIANT_ARRAY(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, #m_name, PROPERTY_HINT_TYPE_STRING, String::num(m_type) + ":"), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_ENUM_ARRAY(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, #m_name, PROPERTY_HINT_TYPE_STRING, String::num(Variant::INT) + ":"), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_PACKED_ENUM_ARRAY(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(PackedArrayHelper<m_type>::variant, #m_name), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_RESOURCE_ARRAY(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, #m_name, PROPERTY_HINT_TYPE_STRING, String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":" #m_type), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_VARIANT_DICTIONARY_RESOURCE(m_key, m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, #m_name, PROPERTY_HINT_DICTIONARY_TYPE, String::num(m_key) + ":;" + String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":" #m_type), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_ENUM_DICTIONARY_RESOURCE(m_enum, m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, #m_name, PROPERTY_HINT_DICTIONARY_TYPE, String::num(Variant::INT) + ":;" + String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":" #m_type), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_IS(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("is_" #m_name), &self_type::is_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(m_type, #m_name), "set_" #m_name, "is_" #m_name)

#define BIND_PROPERTY_OBJECTID_NOT_SAVED(m_type, m_name) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, #m_name, PROPERTY_HINT_RESOURCE_TYPE, #m_type, PROPERTY_USAGE_NONE), "set_" #m_name, "get_" #m_name)

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
#define IMPLEMENT_PROPERTY_OBJECTID_SIMPLE(m_class, m_type, m_name) \
	m_type *m_class::get_##m_name() const { return Object::cast_to<m_type>(ObjectDB::get_instance(_##m_name)); } \
	void m_class::set_##m_name(m_type *new_##m_name) { \
		_##m_name = new_##m_name ? new_##m_name->get_instance_id() : ObjectID(); \
	}

// TODO: https://github.com/godotengine/godot-cpp/issues/1584
#define DECLARE_ENUM(m_enum) \
	VARIANT_ENUM_CAST(m_enum) \
	namespace godot { \
	/* MAKE_TYPED_ARRAY_INFO */ \
	template<> \
	struct GetTypeInfo<TypedArray<m_enum>> { \
		static constexpr GDExtensionVariantType VARIANT_TYPE = GDEXTENSION_VARIANT_TYPE_ARRAY; \
		static constexpr GDExtensionClassMethodArgumentMetadata METADATA = GDEXTENSION_METHOD_ARGUMENT_METADATA_NONE; \
		static inline PropertyInfo get_class_info() { \
			return make_property_info(Variant::ARRAY, "", PROPERTY_HINT_ARRAY_TYPE, Variant::get_type_name(Variant::INT).utf8().get_data()); \
		} \
	}; \
	template<> \
	struct GetTypeInfo<const TypedArray<m_enum> &> { \
		static constexpr GDExtensionVariantType VARIANT_TYPE = GDEXTENSION_VARIANT_TYPE_ARRAY; \
		static constexpr GDExtensionClassMethodArgumentMetadata METADATA = GDEXTENSION_METHOD_ARGUMENT_METADATA_NONE; \
		static inline PropertyInfo get_class_info() { \
			return make_property_info(Variant::ARRAY, "", PROPERTY_HINT_ARRAY_TYPE, Variant::get_type_name(Variant::INT).utf8().get_data()); \
		} \
	}; \
	/* MAKE_TYPED_ARRAY */ \
	template<> \
	class TypedArray<m_enum> : public Array { \
	public: \
		_FORCE_INLINE_ void operator=(const Array &p_array) { \
			ERR_FAIL_COND_MSG(!is_same_typed(p_array), "Cannot assign an array with a different element type."); \
			_ref(p_array); \
		} \
		_FORCE_INLINE_ TypedArray(const Variant &p_variant) : \
			TypedArray(Array(p_variant)) { \
		} \
		_FORCE_INLINE_ TypedArray(const Array &p_array) { \
			set_typed(Variant::INT, StringName(), Variant()); \
			if (is_same_typed(p_array)) { \
				_ref(p_array); \
			} else { \
				assign(p_array); \
			} \
		} \
		_FORCE_INLINE_ TypedArray() { \
			set_typed(Variant::INT, StringName(), Variant()); \
		} \
	}; \
	}

#define DEFAULT_TO_STRING() \
	String _to_string() const { return vformat("<%s#%d>", get_class(), get_instance_id()); }

extern Vector<std::function<void()>> _free_lazy_globals;
template<typename T>
class LazyGlobal {
	std::function<Ref<T>()> _init;
	mutable Ref<T> _ref;

	void _maybe_init() const {
		if (_ref.is_null()) {
			_ref = _init();
			_free_lazy_globals.append([this]() -> void { _ref = Ref<T>(); });
		}
	}
public:
	LazyGlobal(const std::function<Ref<T>()> &init) : _init(init) {}

	operator Ref<T>() const { _maybe_init(); return _ref; }
	T *operator*() const { _maybe_init(); return _ref.ptr(); }
	T *operator->() const { _maybe_init(); return _ref.ptr(); }
	void operator=(const Ref<T> &ref) { _ref = ref; }
};

template<typename T>
class LazyGlobalNode {
	std::function<T *()> _init;
	mutable ObjectID _node;

	void _maybe_init() const {
		if (_node.is_null()) {
			T *node = _init();
			_node = node ? node->get_instance_id() : ObjectID();
			_free_lazy_globals.append([this]() -> void { if (_node.is_valid()) (*this)->queue_free(); });
		}
	}
public:
	LazyGlobalNode(const std::function<T *()> &init) : _init(init) {}

	operator T *() const { _maybe_init(); return Object::cast_to<T>(ObjectDB::get_instance(_node)); }
	T *operator*() const { _maybe_init(); return Object::cast_to<T>(ObjectDB::get_instance(_node)); }
	T *operator->() const { _maybe_init(); return Object::cast_to<T>(ObjectDB::get_instance(_node)); }
	void operator=(T *node) { _node = node ? node->get_instance_id() : ObjectID(); }
};

class LazyStringName {
	const char * const _text;
	mutable StringName *_string_name = nullptr;

public:
	LazyStringName(const char *text) : _text(text) {}

	_FORCE_INLINE_ operator StringName() const {
		if (likely(_string_name)) {
			return *_string_name;
		}

		_string_name = memnew(StringName(_text));
		_free_lazy_globals.append([this]() -> void { memdelete(_string_name); _string_name = nullptr; });
		return *_string_name;
	}
};

template<typename TEnum, size_t TSize = sizeof(TEnum)>
struct PackedArrayHelper {};

template<typename TEnum>
struct PackedArrayHelper<TEnum, 4> {
	typedef PackedInt32Array base_type;
	static constexpr Variant::Type variant = Variant::PACKED_INT32_ARRAY;
	static constexpr GDExtensionVariantType extension_variant = GDEXTENSION_VARIANT_TYPE_PACKED_INT32_ARRAY;
};

template<typename TEnum>
struct PackedArrayHelper<TEnum, 8> {
	typedef PackedInt64Array base_type;
	static constexpr Variant::Type variant = Variant::PACKED_INT64_ARRAY;
	static constexpr GDExtensionVariantType extension_variant = GDEXTENSION_VARIANT_TYPE_PACKED_INT64_ARRAY;
};

template<typename TEnum, typename TBase = typename PackedArrayHelper<TEnum>::base_type>
class PackedArray : public TBase {
public:
	_FORCE_INLINE_ PackedArray() : TBase() {};
	_FORCE_INLINE_ PackedArray(const TBase &p_from) : TBase(p_from) {}
	_FORCE_INLINE_ PackedArray(const Array &p_from) : TBase(p_from) {}
	_FORCE_INLINE_ PackedArray(TBase &&p_other) : TBase(p_other) {}
	_FORCE_INLINE_ PackedArray(std::initializer_list<TEnum> p_init) : TBase(*reinterpret_cast<const std::initializer_list<decltype(*TBase::ptrw())> *>(&p_init)) {}

	_FORCE_INLINE_ const TEnum &operator[](int64_t p_index) const { return reinterpret_cast<const TEnum &>(TBase::operator[](p_index)); }
	_FORCE_INLINE_ TEnum &operator[](int64_t p_index) { return reinterpret_cast<TEnum &>(TBase::operator[](p_index)); }
};

namespace godot {
	template<typename TEnum, typename TBase>
	struct GetTypeInfo<PackedArray<TEnum, TBase>> {
		static constexpr GDExtensionVariantType VARIANT_TYPE = GetTypeInfo<TBase>::VARIANT_TYPE;
		static constexpr GDExtensionClassMethodArgumentMetadata METADATA = GDEXTENSION_METHOD_ARGUMENT_METADATA_NONE;
		static inline PropertyInfo get_class_info() {
			return make_property_info((Variant::Type)VARIANT_TYPE, "");
		}
	};
	template<typename TEnum, typename TBase>
	struct GetTypeInfo<const PackedArray<TEnum, TBase> &> {
		static constexpr GDExtensionVariantType VARIANT_TYPE = GetTypeInfo<const TBase &>::VARIANT_TYPE;
		static constexpr GDExtensionClassMethodArgumentMetadata METADATA = GDEXTENSION_METHOD_ARGUMENT_METADATA_NONE;
		static inline PropertyInfo get_class_info() {
			return make_property_info((Variant::Type)VARIANT_TYPE, "");
		}
	};
	template<typename TEnum, typename TBase>
	struct PtrToArg<PackedArray<TEnum, TBase>> {
		_FORCE_INLINE_ static PackedArray<TEnum, TBase> convert(const void *p_ptr) {
			return *reinterpret_cast<const PackedArray<TEnum, TBase> *>(p_ptr);
		}
		typedef PackedArray<TEnum, TBase> EncodeT;
		_FORCE_INLINE_ static void encode(PackedArray<TEnum, TBase> p_val, void *p_ptr) {
			*reinterpret_cast<PackedArray<TEnum, TBase> *>(p_ptr) = p_val;
		}
	};
	template<typename TEnum, typename TBase>
	struct PtrToArg<const PackedArray<TEnum, TBase> &> {
		_FORCE_INLINE_ static PackedArray<TEnum, TBase> convert(const void *p_ptr) {
			return *reinterpret_cast<const PackedArray<TEnum, TBase> *>(p_ptr);
		}
		typedef PackedArray<TEnum, TBase> EncodeT;
		_FORCE_INLINE_ static void encode(PackedArray<TEnum, TBase> p_val, void *p_ptr) {
			*reinterpret_cast<PackedArray<TEnum, TBase> *>(p_ptr) = p_val;
		}
	};
	template<typename TEnum, typename TBase>
	struct VariantCaster<PackedArray<TEnum, TBase>> {
		static _FORCE_INLINE_ PackedArray<TEnum, TBase> cast(const Variant &p_variant) {
			return static_cast<PackedArray<TEnum, TBase>>(p_variant.operator TBase());
		}
	};
	template<typename TEnum, typename TBase>
	struct VariantCaster<const PackedArray<TEnum, TBase> &> {
		static _FORCE_INLINE_ PackedArray<TEnum, TBase> cast(const Variant &p_variant) {
			return static_cast<PackedArray<TEnum, TBase>>(p_variant.operator TBase());
		}
	};
	namespace internal {
		template<typename TEnum, typename TBase>
		struct VariantInternalType<PackedArray<TEnum, TBase>> {
			static constexpr Variant::Type type = VariantInternalType<TBase>::type;
		};
	}
}

#include "predefined.hpp"

#endif // DRY_H
