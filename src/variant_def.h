#ifndef VARIANT_DEF_H
#define VARIANT_DEF_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class VariantDef : public Resource {
	GDCLASS(VariantDef, Resource);

protected:
	static void _bind_methods();

private:

public:
	VariantDef();
	~VariantDef();
};

#endif // VARIANT_DEF_H
