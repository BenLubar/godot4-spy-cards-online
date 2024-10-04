#ifndef JIGSAW_REUSABLE_COMMAND_LIST_H
#define JIGSAW_REUSABLE_COMMAND_LIST_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class JigsawReusableCommandList : public Resource {
	GDCLASS(JigsawReusableCommandList, Resource);

protected:
	static void _bind_methods();
};

#endif // JIGSAW_REUSABLE_COMMAND_LIST_H
