#ifndef STAT_DEF_H
#define STAT_DEF_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class StatDef : public Resource {
	GDCLASS(StatDef, Resource);

protected:
	static void _bind_methods();

private:

public:
	StatDef();
	~StatDef();
};

#endif // STAT_DEF_H
