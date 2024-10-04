#ifndef JIGSAW_COMMAND_H
#define JIGSAW_COMMAND_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class JigsawCommand : public Resource {
	GDCLASS(JigsawCommand, Resource);

public:
	enum Type {
		CRASH = 0,
		COMMENT = 1,
		REUSABLE_COMMAND_LIST = 2,
		MATH = 3,
	};

protected:
	static void _bind_methods();

public:
	virtual Type get_type() = 0;
};
VARIANT_ENUM_CAST(JigsawCommand::Type);

#endif // JIGSAW_COMMAND_H
