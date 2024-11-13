#include "card_grid.h"

#include <godot_cpp/classes/resource_loader.hpp>

void CardGrid2D::_bind_methods() {
	BIND_PROPERTY_ENUM(LocationDef::Location, auto_location);
	BIND_PROPERTY(Variant::INT, auto_location_side);
	BIND_PROPERTY_RESOURCE_ARRAY(CardInstance, custom_card_instances);
}

IMPLEMENT_PROPERTY_SIMPLE(CardGrid2D, enums::LocationDef::Location, auto_location);
IMPLEMENT_PROPERTY_SIMPLE(CardGrid2D, int64_t, auto_location_side);
IMPLEMENT_PROPERTY_SIMPLE(CardGrid2D, TypedArray<CardInstance>, custom_card_instances);

void CardGrid3D::_bind_methods() {
	BIND_PROPERTY_ENUM(LocationDef::Location, auto_location);
	BIND_PROPERTY(Variant::INT, auto_location_side);
	BIND_PROPERTY_RESOURCE_ARRAY(CardInstance, custom_card_instances);
}

IMPLEMENT_PROPERTY_SIMPLE(CardGrid3D, enums::LocationDef::Location, auto_location);
IMPLEMENT_PROPERTY_SIMPLE(CardGrid3D, int64_t, auto_location_side);
IMPLEMENT_PROPERTY_SIMPLE(CardGrid3D, TypedArray<CardInstance>, custom_card_instances);
