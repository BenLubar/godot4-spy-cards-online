#ifndef CHOICES_DEF_H
#define CHOICES_DEF_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class ChoicesDef : public Resource {
	GDCLASS(ChoicesDef, Resource);

protected:
	static void _bind_methods();

private:

public:
	ChoicesDef();
	~ChoicesDef();
};

#endif // CHOICES_DEF_H
