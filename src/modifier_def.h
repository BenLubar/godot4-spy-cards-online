#ifndef MODIFIER_DEF_H
#define MODIFIER_DEF_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class ModifierDef : public Resource {
	GDCLASS(ModifierDef, Resource);

protected:
	static void _bind_methods();

private:

public:
	ModifierDef();
	~ModifierDef();
};

#endif // MODIFIER_DEF_H
