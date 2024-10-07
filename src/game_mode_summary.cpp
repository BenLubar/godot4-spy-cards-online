#include "game_mode_summary.h"

void GameModeSummary::_bind_methods() {
	BIND_PROPERTY(Variant::STRING, author);
	BIND_PROPERTY(Variant::INT, mode_thumbnail);
	BIND_PROPERTY_MULTILINE_TEXT(description);
	BIND_PROPERTY_MULTILINE_TEXT(latest_changes);
	BIND_PROPERTY_RESOURCE_ARRAY(IconDef, custom_icons);

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
		return Ref<Texture2D>();
	}

	if (icon < IconDef::Icon::FIRST_CUSTOM) {
		return get_predefined<Texture2D>(icon);
	}

	ERR_FAIL_INDEX_V_MSG(icon - IconDef::Icon::FIRST_CUSTOM, custom_icons.size(), Ref<Texture2D>(), "icon outside of custom range");

	Ref<IconDef> def = custom_icons[icon - IconDef::Icon::FIRST_CUSTOM];
	ERR_FAIL_NULL_V_MSG(def, Ref<Texture2D>(), "custom icon is null");

	return def->get_texture();
}
