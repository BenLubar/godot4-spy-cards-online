#ifndef CHARACTER_DEF_H
#define CHARACTER_DEF_H

#include "dry.h"

#include "enums_character_def.h"
#include "enums_icon_def.h"

#include <godot_cpp/classes/atlas_texture.hpp>

class CharacterDef;

#include "game_mode.h"

class CharacterDef : public Resource {
	GDCLASS(CharacterDef, Resource);

public:
	using Character = enums::CharacterDef::Character;

protected:
	static void _bind_methods();

public:
	CharacterDef() = default;
	~CharacterDef() = default;

	DECLARE_PROPERTY(String, id);
	DECLARE_PROPERTY(String, display_name);
	DECLARE_PROPERTY(PackedStringArray, tags);
	DECLARE_PROPERTY_IS(bool, hidden, = false);

	DECLARE_PROPERTY(enums::IconDef::Icon, idle0, = enums::IconDef::Icon::NONE);
	DECLARE_PROPERTY(enums::IconDef::Icon, idle1, = enums::IconDef::Icon::NONE);
	DECLARE_PROPERTY(enums::IconDef::Icon, angry, = enums::IconDef::Icon::NONE);

	DECLARE_PROPERTY(double, idle0_center_px, = 0.0);
	DECLARE_PROPERTY(double, idle1_center_px, = 0.0);
	DECLARE_PROPERTY(double, angry_center_px, = 0.0);

	DECLARE_PROPERTY(double, pixel_size, = 0.01);
	DECLARE_PROPERTY(Vector2, portrait_offset);
	DECLARE_PROPERTY(double, portrait_scale, = 1.0);
	DECLARE_PROPERTY(bool, portrait_crop_top, = false);
	DECLARE_PROPERTY(bool, portrait_crop_front, = false);

	Ref<AtlasTexture> create_portrait(const Ref<GameMode> &mode) const;
};

#endif // CHARACTER_DEF_H
