#include "game_mode_summary.h"

void GameModeSummary::_bind_methods() {
	BIND_PROPERTY(Variant::STRING, author);
	BIND_PROPERTY(Variant::INT, mode_thumbnail);
	BIND_PROPERTY(Variant::STRING, description, PROPERTY_HINT_MULTILINE_TEXT);
	BIND_PROPERTY(Variant::STRING, latest_changes, PROPERTY_HINT_MULTILINE_TEXT);
	BIND_PROPERTY(Variant::ARRAY, custom_icons, PROPERTY_HINT_TYPE_STRING, String::num(Variant::OBJECT) + "/" + String::num(PROPERTY_HINT_RESOURCE_TYPE) + ":IconDef");

	ClassDB::bind_method(D_METHOD("get_icon_texture", "icon"), &GameModeSummary::get_icon_texture);
}

GameModeSummary::GameModeSummary() {}
GameModeSummary::~GameModeSummary() {}

IMPLEMENT_PROPERTY(GameModeSummary, String, author);
IMPLEMENT_PROPERTY(GameModeSummary, IconDef::Icon, mode_thumbnail);
IMPLEMENT_PROPERTY(GameModeSummary, String, description);
IMPLEMENT_PROPERTY(GameModeSummary, String, latest_changes);
IMPLEMENT_PROPERTY(GameModeSummary, TypedArray<IconDef>, custom_icons);

Ref<Texture2D> GameModeSummary::get_icon_texture(IconDef::Icon icon) const {
	if (icon == IconDef::Icon::NONE) {
		return nullptr;
	}

	if (icon < IconDef::Icon::FIRST_CUSTOM) {
		return get_predefined<Texture2D>(icon);
	}

	ERR_FAIL_INDEX_V_MSG(icon - IconDef::Icon::FIRST_CUSTOM, custom_icons.size(), nullptr, "icon outside of custom range");

	Ref<IconDef> def = custom_icons[icon - IconDef::Icon::FIRST_CUSTOM];
	ERR_FAIL_NULL_V_MSG(def, nullptr, "custom icon is null");

	return def->get_texture();
}
