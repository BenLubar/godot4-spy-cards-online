#include "player_preferences_helper.h"

#include <godot_cpp/classes/gd_script.hpp>

static LazyGlobal<GDScript> player_preferences([]() -> Ref<GDScript> { return ResourceLoader::get_singleton()->load("res://api/player_preferences.gd", "GDScript");; });

bool PlayerPreferences::alternate_colors() {
	return player_preferences->get("alternate_colors");
}

bool PlayerPreferences::prefers_reduced_motion() {
	return player_preferences->get("prefers_reduced_motion");
}
