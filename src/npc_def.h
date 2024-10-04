#ifndef NPC_DEF_H
#define NPC_DEF_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class NPCDef : public Resource {
	GDCLASS(NPCDef, Resource);

protected:
	static void _bind_methods();

private:

public:
	NPCDef();
	~NPCDef();
};

#endif // NPC_DEF_H
