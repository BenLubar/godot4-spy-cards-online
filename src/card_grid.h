#ifndef CARD_GRID_H
#define CARD_GRID_H

#include "dry.h"

#include "enums_location_def.h"

#include <godot_cpp/classes/gd_script.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/node3d.hpp>

class CardGrid2D;
class CardGrid3D;

#include "card_instance.h"

class CardGrid2D : public Control {
	GDCLASS(CardGrid2D, Control);

protected:
	static void _bind_methods();

private:
	Vector<Control *> _card_children;

public:
	DECLARE_PROPERTY(enums::LocationDef::Location, auto_location, = enums::LocationDef::NONE);
	DECLARE_PROPERTY(int64_t, auto_location_side, = -1);
	DECLARE_PROPERTY(TypedArray<CardInstance>, custom_card_instances);

	DEFAULT_TO_STRING();
};

class CardGrid3D : public Node3D {
	GDCLASS(CardGrid3D, Node3D);

protected:
	static void _bind_methods();

private:
	Vector<Node3D *> _card_children;

public:
	DECLARE_PROPERTY(enums::LocationDef::Location, auto_location, = enums::LocationDef::NONE);
	DECLARE_PROPERTY(int64_t, auto_location_side, = -1);
	DECLARE_PROPERTY(TypedArray<CardInstance>, custom_card_instances);

	DEFAULT_TO_STRING();
};

#endif // CARD_GRID_H
