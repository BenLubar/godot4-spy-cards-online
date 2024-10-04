#ifndef GAME_MODE_SUMMARY_H
#define GAME_MODE_SUMMARY_H

#include "dry.h"
#include "icon_def.h"

using namespace godot;

class GameModeSummary : public Resource {
	GDCLASS(GameModeSummary, Resource);

protected:
	static void _bind_methods();

public:
	GameModeSummary();
	~GameModeSummary();

	DECLARE_PROPERTY(String, author);
	DECLARE_PROPERTY(IconDef::Icon, mode_thumbnail, = IconDef::Icon::NONE);
	DECLARE_PROPERTY(String, description);
	DECLARE_PROPERTY(String, latest_changes);
	DECLARE_PROPERTY(TypedArray<IconDef>, custom_icons);

	Ref<Texture2D> get_icon_texture(IconDef::Icon icon) const;
};

#endif // GAME_MODE_SUMMARY_H
