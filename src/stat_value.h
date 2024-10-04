#ifndef STAT_VALUE_H
#define STAT_VALUE_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class StatValue : public Resource {
	GDCLASS(StatValue, Resource);

protected:
	static void _bind_methods();

private:

public:
	StatValue();
	~StatValue();
};

#endif // STAT_VALUE_H
