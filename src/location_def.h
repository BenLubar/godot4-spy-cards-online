#ifndef LOCATION_DEF_H
#define LOCATION_DEF_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class LocationDef : public Resource {
	GDCLASS(LocationDef, Resource);

protected:
	static void _bind_methods();

private:

public:
	LocationDef();
	~LocationDef();
};

#endif // LOCATION_DEF_H
