#ifndef JIGSAW_COMMAND_LIST_H
#define JIGSAW_COMMAND_LIST_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class JigsawCommandList : public Resource {
	GDCLASS(JigsawCommandList, Resource);

protected:
	static void _bind_methods();
};

#endif // JIGSAW_COMMAND_LIST_H
