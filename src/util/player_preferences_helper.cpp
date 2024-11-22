#include "util/player_preferences_helper.h"

#include <godot_cpp/classes/gd_script.hpp>

static LazyGlobal<GDScript> player_preferences([]() -> Ref<GDScript> {
	Ref<GDScript> script = ResourceLoader::get_singleton()->load("res://api/player_preferences.gd", "GDScript");
	if (!script.is_valid()) {
		script.instantiate(); // just avoid crashing
	}
	return script;
});

bool PlayerPreferences::alternate_colors() {
	return player_preferences->get("alternate_colors");
}

bool PlayerPreferences::disable_3d() {
	return player_preferences->get("disable_3d");
}

bool PlayerPreferences::prefers_reduced_motion() {
	return player_preferences->get("prefers_reduced_motion");
}

bool PlayerPreferences::shadows() {
	return player_preferences->get("shadows");
}
