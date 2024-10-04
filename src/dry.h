#ifndef DRY_H
#define DRY_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

#define DECLARE_PROPERTY(m_type, m_name, ...) \
private: \
	m_type m_name __VA_ARGS__; \
public: \
	m_type get_##m_name() const; \
	void set_##m_name(m_type new_##m_name)

#define BIND_PROPERTY(m_type, m_name, ...) \
	ClassDB::bind_method(D_METHOD("get_" #m_name), &self_type::get_##m_name); \
	ClassDB::bind_method(D_METHOD("set_" #m_name, #m_name), &self_type::set_##m_name); \
	ADD_PROPERTY(PropertyInfo(m_type, #m_name __VA_OPT__(,) __VA_ARGS__), "set_" #m_name, "get_" #m_name)

#define IMPLEMENT_PROPERTY(m_class, m_type, m_name) \
	m_type m_class::get_##m_name() const { return m_name; } \
	void m_class::set_##m_name(m_type new_##m_name) { \
		if (m_name != new_##m_name) { \
			m_name = new_##m_name; \
			emit_changed(); \
		} \
	}

#endif // DRY_H
