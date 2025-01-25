#include "util/player_preferences_helper.h"

#include <godot_cpp/classes/gd_script.hpp>

static LazyGlobalFile<GDScript> player_preferences{"res://api/player_preferences.gd"};

bool PlayerPreferences::alternate_colors() {
	ERR_FAIL_NULL_V(*player_preferences, false);
	return player_preferences->get("alternate_colors");
}

bool PlayerPreferences::disable_3d() {
	ERR_FAIL_NULL_V(*player_preferences, false);
	return player_preferences->get("disable_3d");
}

bool PlayerPreferences::prefers_reduced_motion() {
	ERR_FAIL_NULL_V(*player_preferences, false);
	return player_preferences->get("prefers_reduced_motion");
}

bool PlayerPreferences::shadows() {
	ERR_FAIL_NULL_V(*player_preferences, false);
	return player_preferences->get("shadows");
}
