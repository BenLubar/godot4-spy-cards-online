#include "game_mode_summary.h"
#include "predefined.hpp"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void GameModeSummary::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_author"), &GameModeSummary::get_author);
	ClassDB::bind_method(D_METHOD("set_author", "author"), &GameModeSummary::set_author);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "author"), "set_author", "get_author");
	ClassDB::bind_method(D_METHOD("get_mode_thumbnail"), &GameModeSummary::get_mode_thumbnail);
	ClassDB::bind_method(D_METHOD("set_mode_thumbnail", "mode_thumbnail"), &GameModeSummary::set_mode_thumbnail);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "mode_thumbnail"), "set_mode_thumbnail", "get_mode_thumbnail");
	ClassDB::bind_method(D_METHOD("get_description"), &GameModeSummary::get_description);
	ClassDB::bind_method(D_METHOD("set_description", "description"), &GameModeSummary::set_description);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "description", PROPERTY_HINT_MULTILINE_TEXT), "set_description", "get_description");
	ClassDB::bind_method(D_METHOD("get_latest_changes"), &GameModeSummary::get_latest_changes);
	ClassDB::bind_method(D_METHOD("set_latest_changes", "latest_changes"), &GameModeSummary::set_latest_changes);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "latest_changes", PROPERTY_HINT_MULTILINE_TEXT), "set_latest_changes", "get_latest_changes");
	ClassDB::bind_method(D_METHOD("get_custom_icons"), &GameModeSummary::get_custom_icons);
	ClassDB::bind_method(D_METHOD("set_custom_icons", "custom_icons"), &GameModeSummary::set_custom_icons);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "custom_icons", PROPERTY_HINT_TYPE_STRING, String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":IconDef"), "set_custom_icons", "get_custom_icons");

	ClassDB::bind_method(D_METHOD("get_icon_texture", "icon"), &GameModeSummary::get_icon_texture);
}

GameModeSummary::GameModeSummary() {}
GameModeSummary::~GameModeSummary() {}

String GameModeSummary::get_author() const {
	return author;
}
void GameModeSummary::set_author(String new_author) {
	if (author != new_author) {
		author = new_author;
		emit_changed();
	}
}
IconDef::Icon GameModeSummary::get_mode_thumbnail() const {
	return mode_thumbnail;
}
void GameModeSummary::set_mode_thumbnail(IconDef::Icon new_mode_thumbnail) {
	if (mode_thumbnail != new_mode_thumbnail) {
		mode_thumbnail = new_mode_thumbnail;
		emit_changed();
	}
}
String GameModeSummary::get_description() const {
	return description;
}
void GameModeSummary::set_description(String new_description) {
	if (description != new_description) {
		description = new_description;
		emit_changed();
	}
}
String GameModeSummary::get_latest_changes() const {
	return latest_changes;
}
void GameModeSummary::set_latest_changes(String new_latest_changes) {
	if (latest_changes != new_latest_changes) {
		latest_changes = new_latest_changes;
		emit_changed();
	}
}
TypedArray<IconDef> GameModeSummary::get_custom_icons() const {
	return custom_icons;
}
void GameModeSummary::set_custom_icons(TypedArray<IconDef> new_custom_icons) {
	if (custom_icons != new_custom_icons) {
		custom_icons = new_custom_icons;
		emit_changed();
	}
}

Ref<Texture2D> GameModeSummary::get_icon_texture(IconDef::Icon icon) const {
	if (icon == IconDef::Icon::NONE) {
		return nullptr;
	}

	if (icon < IconDef::Icon::FIRST_CUSTOM) {
		return get_predefined<Texture2D>("ICON", icon);
	}

	ERR_FAIL_INDEX_V_MSG(icon - IconDef::Icon::FIRST_CUSTOM, custom_icons.size(), nullptr, "icon outside of custom range");

	Ref<IconDef> def = custom_icons[icon - IconDef::Icon::FIRST_CUSTOM];
	ERR_FAIL_NULL_V_MSG(def, nullptr, "custom icon is null");

	return def->get_texture();
}
