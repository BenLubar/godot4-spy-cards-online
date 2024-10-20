#ifndef CARD_GRID_NATIVE_H
#define CARD_GRID_NATIVE_H

#include "dry.h"

#include "enums_location_def.h"

#include <godot_cpp/classes/gd_script.hpp>
#include <godot_cpp/classes/node3d.hpp>

class CardGridNative;

#include "card_instance.h"

class CardGridNative : public Node3D {
	GDCLASS(CardGridNative, Node3D);

protected:
	static void _bind_methods();

private:
	enums::LocationDef::Location auto_location = enums::LocationDef::Location::NONE;
	int64_t auto_location_side = -1;
	TypedArray<CardInstance> custom_card_instances;
	Vector<Node3D *> card_children;

public:
	CardGridNative() = default;
	~CardGridNative() = default;

	GDVIRTUAL2R(Node3D *, _insert_card, int64_t, const Ref<CardInstance> &);
	GDVIRTUAL3(_update_card, int64_t, const Ref<CardInstance> &, Node3D *);
	GDVIRTUAL4(_reorder_card, int64_t, int64_t, const Ref<CardInstance> &, Node3D *);
	GDVIRTUAL3(_remove_card, int64_t, const Ref<CardInstance> &, Node3D *);

	static LazyGlobal<GDScript> impl_script;
	static void set_impl_script(const Ref<GDScript> &script);
	static Ref<GDScript> get_impl_script();
	static CardGridNative *make();
};

#endif // CARD_GRID_NATIVE_H
