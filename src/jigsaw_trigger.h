#ifndef JIGSAW_TRIGGER_H
#define JIGSAW_TRIGGER_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class JigsawTrigger : public Resource {
	GDCLASS(JigsawTrigger, Resource);

protected:
	static void _bind_methods();
};

#endif // JIGSAW_TRIGGER_H
