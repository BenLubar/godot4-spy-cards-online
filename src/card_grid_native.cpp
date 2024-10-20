#include "card_grid_native.h"

#include <godot_cpp/classes/resource_loader.hpp>

void CardGridNative::_bind_methods() {
	GDVIRTUAL_BIND(_insert_card, "index", "card");
	GDVIRTUAL_BIND(_update_card, "index", "card", "node");
	GDVIRTUAL_BIND(_reorder_card, "old_index", "new_index", "card", "node");
	GDVIRTUAL_BIND(_remove_card, "index", "card", "node");

	ClassDB::bind_static_method("CardGridNative", D_METHOD("set_impl_script", "script"), &CardGridNative::set_impl_script);
	ClassDB::bind_static_method("CardGridNative", D_METHOD("get_impl_script"), &CardGridNative::get_impl_script);
	// this doesn't create a static property :(
	//ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "impl_script", PROPERTY_HINT_RESOURCE_TYPE, "GDScript", PROPERTY_USAGE_NONE), "set_impl_script", "get_impl_script");
	ClassDB::bind_static_method("CardGridNative", D_METHOD("make"), &CardGridNative::make);
}

LazyGlobal<GDScript> CardGridNative::impl_script{[]() -> Ref<GDScript> { return ResourceLoader::get_singleton()->load("res://card/card_grid.gd", "GDScript"); }};

void CardGridNative::set_impl_script(const Ref<GDScript> &script) {
	ERR_FAIL_COND(script.is_null());

	impl_script = script;
}

Ref<GDScript> CardGridNative::get_impl_script() {
	return impl_script;
}

CardGridNative *CardGridNative::make() {
	Ref<GDScript> script = get_impl_script();
	CRASH_COND(script.is_null());

	return Object::cast_to<CardGridNative>(script->new_());
}
