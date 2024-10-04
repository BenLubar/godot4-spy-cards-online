#ifndef VARIABLE_DEF_H
#define VARIABLE_DEF_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class VariableDef : public Resource {
	GDCLASS(VariableDef, Resource);

protected:
	static void _bind_methods();

private:

public:
	VariableDef();
	~VariableDef();
};

#endif // VARIABLE_DEF_H
