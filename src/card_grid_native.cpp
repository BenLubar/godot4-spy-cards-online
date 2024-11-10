#include "card_grid_native.h"

#include <godot_cpp/classes/resource_loader.hpp>

void CardGridNative2D::_bind_methods() {
	GDVIRTUAL_BIND(_insert_card, "index", "card");
	GDVIRTUAL_BIND(_update_card, "index", "card", "node");
	GDVIRTUAL_BIND(_reorder_card, "old_index", "new_index", "card", "node");
	GDVIRTUAL_BIND(_remove_card, "index", "card", "node");

	ClassDB::bind_static_method("CardGridNative2D", D_METHOD("set_impl_script", "script"), &CardGridNative2D::set_impl_script);
	ClassDB::bind_static_method("CardGridNative2D", D_METHOD("get_impl_script"), &CardGridNative2D::get_impl_script);
	ClassDB::bind_static_method("CardGridNative2D", D_METHOD("make"), &CardGridNative2D::make);
}

void CardGridNative3D::_bind_methods() {
	GDVIRTUAL_BIND(_insert_card, "index", "card");
	GDVIRTUAL_BIND(_update_card, "index", "card", "node");
	GDVIRTUAL_BIND(_reorder_card, "old_index", "new_index", "card", "node");
	GDVIRTUAL_BIND(_remove_card, "index", "card", "node");

	ClassDB::bind_static_method("CardGridNative3D", D_METHOD("set_impl_script", "script"), &CardGridNative3D::set_impl_script);
	ClassDB::bind_static_method("CardGridNative3D", D_METHOD("get_impl_script"), &CardGridNative3D::get_impl_script);
	ClassDB::bind_static_method("CardGridNative3D", D_METHOD("make"), &CardGridNative3D::make);
}

LazyGlobal<GDScript> CardGridNative2D::impl_script{[]() -> Ref<GDScript> { return ResourceLoader::get_singleton()->load("res://card/card_grid_2d.gd", "GDScript"); }};
LazyGlobal<GDScript> CardGridNative3D::impl_script{[]() -> Ref<GDScript> { return ResourceLoader::get_singleton()->load("res://card/card_grid_3d.gd", "GDScript"); }};

void CardGridNative2D::set_impl_script(const Ref<GDScript> &script) {
	ERR_FAIL_COND(script.is_null());

	impl_script = script;
}

void CardGridNative3D::set_impl_script(const Ref<GDScript> &script) {
	ERR_FAIL_COND(script.is_null());

	impl_script = script;
}

Ref<GDScript> CardGridNative2D::get_impl_script() {
	return impl_script;
}

Ref<GDScript> CardGridNative3D::get_impl_script() {
	return impl_script;
}

CardGridNative2D *CardGridNative2D::make() {
	Ref<GDScript> script = get_impl_script();
	CRASH_COND(script.is_null());

	return Object::cast_to<CardGridNative2D>(script->new_());
}

CardGridNative3D *CardGridNative3D::make() {
	Ref<GDScript> script = get_impl_script();
	CRASH_COND(script.is_null());

	return Object::cast_to<CardGridNative3D>(script->new_());
}
