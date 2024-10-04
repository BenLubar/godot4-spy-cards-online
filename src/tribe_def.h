#ifndef TRIBE_DEF_H
#define TRIBE_DEF_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class TribeDef : public Resource {
	GDCLASS(TribeDef, Resource);

protected:
	static void _bind_methods();

private:

public:
	TribeDef();
	~TribeDef();
};

#endif // TRIBE_DEF_H
