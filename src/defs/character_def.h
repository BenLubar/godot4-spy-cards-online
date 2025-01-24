#ifndef CHARACTER_DEF_H
#define CHARACTER_DEF_H

#include "dry.h"

#include "enums/enums_character_def.h"
#include "enums/enums_icon_def.h"

#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/mesh_instance2d.hpp>

class CharacterDef;

#include "defs/game_mode.h"

class CharacterDef : public Resource {
	GDCLASS(CharacterDef, Resource);

public:
	using Character = enums::CharacterDef::Character;

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(String, id);
	DECLARE_PROPERTY(String, display_name);
	DECLARE_PROPERTY(PackedStringArray, tags);
	DECLARE_PROPERTY_IS(bool, hidden, = false);

	DECLARE_PROPERTY(enums::IconDef::Icon, idle0, = enums::IconDef::Icon::NONE);
	DECLARE_PROPERTY(enums::IconDef::Icon, idle1, = enums::IconDef::Icon::NONE);
	DECLARE_PROPERTY(enums::IconDef::Icon, angry, = enums::IconDef::Icon::NONE);

	DECLARE_PROPERTY(float, idle0_center_px, = 0.0f);
	DECLARE_PROPERTY(float, idle1_center_px, = 0.0f);
	DECLARE_PROPERTY(float, angry_center_px, = 0.0f);

	DECLARE_PROPERTY(float, pixel_size, = 0.01f);
	DECLARE_PROPERTY(Vector2, portrait_offset);
	DECLARE_PROPERTY(float, portrait_scale, = 1.0f);
	DECLARE_PROPERTY(bool, portrait_crop_top, = false);
	DECLARE_PROPERTY(bool, portrait_crop_front, = false);

	Ref<Mesh> create_portrait(const Ref<GameMode> &mode) const;
	void set_portrait_2d(const Ref<GameMode> &mode, MeshInstance2D *inst) const;
	DEFAULT_TO_STRING();
};

#endif // CHARACTER_DEF_H
