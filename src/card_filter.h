#ifndef CARD_FILTER_H
#define CARD_FILTER_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class CardFilter : public Resource {
	GDCLASS(CardFilter, Resource);

protected:
	static void _bind_methods();

private:

public:
	CardFilter();
	~CardFilter();
};

#endif // CARD_FILTER_H
