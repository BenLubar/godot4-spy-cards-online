#ifndef CHARACTER_DEF_H
#define CHARACTER_DEF_H

#include "dry.h"
#include "icon_def.h"
#include "game_mode.h"

#include <godot_cpp/classes/atlas_texture.hpp>

using namespace godot;

class CharacterDef : public Resource {
	GDCLASS(CharacterDef, Resource);

protected:
	static void _bind_methods();

public:
	CharacterDef();
	~CharacterDef();

	DECLARE_PROPERTY(String, id);
	DECLARE_PROPERTY(String, display_name);
	DECLARE_PROPERTY(PackedStringArray, tags);
	DECLARE_PROPERTY(bool, hidden, = false);

	DECLARE_PROPERTY(IconDef::Icon, idle0, = IconDef::Icon::NONE);
	DECLARE_PROPERTY(IconDef::Icon, idle1, = IconDef::Icon::NONE);
	DECLARE_PROPERTY(IconDef::Icon, angry, = IconDef::Icon::NONE);

	DECLARE_PROPERTY(double, idle0_center_px, = 0.0);
	DECLARE_PROPERTY(double, idle1_center_px, = 0.0);
	DECLARE_PROPERTY(double, angry_center_px, = 0.0);

	DECLARE_PROPERTY(double, pixel_size, = 0.01);
	DECLARE_PROPERTY(Vector2, portrait_offset);
	DECLARE_PROPERTY(double, portrait_scale, = 1.0);
	DECLARE_PROPERTY(bool, portrait_crop_top, = false);
	DECLARE_PROPERTY(bool, portrait_crop_front, = false);

	AtlasTexture *create_portrait(Ref<GameMode> mode) const;
};

#endif // CHARACTER_DEF_H
