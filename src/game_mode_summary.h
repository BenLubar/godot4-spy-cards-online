#ifndef GAME_MODE_SUMMARY_H
#define GAME_MODE_SUMMARY_H

#include "dry.h"

#include "enums_icon_def.h"

#include <godot_cpp/classes/texture2d.hpp>

class GameModeSummary;

#include "icon_def.h"

class GameModeSummary : public Resource {
	GDCLASS(GameModeSummary, Resource);

protected:
	static void _bind_methods();

public:
	GameModeSummary() = default;
	~GameModeSummary() = default;

	DECLARE_PROPERTY(String, title);
	DECLARE_PROPERTY(String, author);
	DECLARE_PROPERTY(enums::IconDef::Icon, mode_thumbnail, = enums::IconDef::Icon::NONE);
	DECLARE_PROPERTY(String, description);
	DECLARE_PROPERTY(String, latest_changes);
	DECLARE_PROPERTY(TypedArray<IconDef>, custom_icons);

	Ref<Texture2D> get_icon_texture(IconDef::Icon icon) const;
};

#endif // GAME_MODE_SUMMARY_H
