#ifndef RANK_DEF_H
#define RANK_DEF_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class RankDef : public Resource {
	GDCLASS(RankDef, Resource);

protected:
	static void _bind_methods();

private:

public:
	RankDef();
	~RankDef();
};

#endif // RANK_DEF_H
