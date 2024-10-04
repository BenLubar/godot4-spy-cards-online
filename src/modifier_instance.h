#ifndef MODIFIER_INSTANCE_H
#define MODIFIER_INSTANCE_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class ModifierInstance : public RefCounted {
	GDCLASS(ModifierInstance, RefCounted);

protected:
	static void _bind_methods();

private:

public:
	ModifierInstance();
	~ModifierInstance();
};

#endif // MODIFIER_INSTANCE_H
