#ifndef DRY_H
#define DRY_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

#include "predefined.hpp"

#define DECLARE_PROPERTY(m_type, m_name, ...) \
private: \
	m_type m_name __VA_ARGS__; \
public: \
	m_type get_##m_name() const; \
	void set_##m_name(m_type new_##m_name)

#define DECLARE_PROPERTY_IS(m_type, m_name, ...) \
private: \
	m_type m_name __VA_ARGS__; \
public: \
	m_type is_##m_name() const; \
	void set_##m_name(m_type new_##m_name)

#define BIND_PROPERTY(m_type, m_name, ...) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(m_type, #m_name, ## __VA_ARGS__), "set_" #m_name, "get_" #m_name)

#define BIND_PROPERTY_IS(m_type, m_name, ...) \
	ClassDB::bind_method(D_METHOD("is_" #m_name), &self_type::is_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(m_type, #m_name, ## __VA_ARGS__), "set_" #m_name, "is_" #m_name)

#define IMPLEMENT_PROPERTY_ONCHANGE(m_class, m_type, m_name, m_onchange) \
	m_type m_class::get_##m_name() const { return m_name; } \
	void m_class::set_##m_name(m_type new_##m_name) { \
		if (m_name != new_##m_name) { \
			m_name = new_##m_name; \
			m_onchange; \
		} \
	}
#define IMPLEMENT_PROPERTY_ONCHANGE_IS(m_class, m_type, m_name, m_onchange) \
	m_type m_class::is_##m_name() const { return m_name; } \
	void m_class::set_##m_name(m_type new_##m_name) { \
		if (m_name != new_##m_name) { \
			m_name = new_##m_name; \
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

#endif // DRY_H
