#include "character_def.h"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void CharacterDef::_bind_methods() {
	BIND_PROPERTY(Variant::STRING, id);
	BIND_PROPERTY(Variant::STRING, display_name);
	BIND_PROPERTY(Variant::PACKED_STRING_ARRAY, tags);
	BIND_PROPERTY(Variant::BOOL, hidden);

	BIND_PROPERTY(Variant::INT, idle0);
	BIND_PROPERTY(Variant::INT, idle1);
	BIND_PROPERTY(Variant::INT, angry);

	BIND_PROPERTY(Variant::FLOAT, idle0_center_px);
	BIND_PROPERTY(Variant::FLOAT, idle1_center_px);
	BIND_PROPERTY(Variant::FLOAT, angry_center_px);

	BIND_PROPERTY(Variant::FLOAT, pixel_size);
	BIND_PROPERTY(Variant::VECTOR2, portrait_offset);
	BIND_PROPERTY(Variant::FLOAT, portrait_scale);
	BIND_PROPERTY(Variant::BOOL, portrait_crop_top);
	BIND_PROPERTY(Variant::BOOL, portrait_crop_front);

	ClassDB::bind_method(D_METHOD("create_portrait", "mode"), &CharacterDef::create_portrait);
}
CharacterDef::CharacterDef() {}
CharacterDef::~CharacterDef() {}

IMPLEMENT_PROPERTY(CharacterDef, String, id);
IMPLEMENT_PROPERTY(CharacterDef, String, display_name);
IMPLEMENT_PROPERTY(CharacterDef, PackedStringArray, tags);
IMPLEMENT_PROPERTY(CharacterDef, bool, hidden);

IMPLEMENT_PROPERTY(CharacterDef, IconDef::Icon, idle0);
IMPLEMENT_PROPERTY(CharacterDef, IconDef::Icon, idle1);
IMPLEMENT_PROPERTY(CharacterDef, IconDef::Icon, angry);

IMPLEMENT_PROPERTY(CharacterDef, double, idle0_center_px);
IMPLEMENT_PROPERTY(CharacterDef, double, idle1_center_px);
IMPLEMENT_PROPERTY(CharacterDef, double, angry_center_px);

IMPLEMENT_PROPERTY(CharacterDef, double, pixel_size);
IMPLEMENT_PROPERTY(CharacterDef, Vector2, portrait_offset);
IMPLEMENT_PROPERTY(CharacterDef, double, portrait_scale);
IMPLEMENT_PROPERTY(CharacterDef, bool, portrait_crop_top);
IMPLEMENT_PROPERTY(CharacterDef, bool, portrait_crop_front);

AtlasTexture *CharacterDef::create_portrait(Ref<GameMode> mode) const {
	ERR_FAIL_COND_V_MSG(mode.is_null(), nullptr, "game mode is null");

	AtlasTexture *portrait = memnew(AtlasTexture);
	portrait->set_atlas(mode->get_icon_texture(idle0));

	double half_square = portrait_scale / pixel_size / 2.0;
	Vector2 size = portrait->get_atlas()->get_size();
	Vector2 center = Vector2(size.x / 2.0, half_square) + portrait_offset;
	Vector2 tl = center - Vector2(half_square, half_square);
	Vector2 br = center + Vector2(half_square, half_square);

	Rect2 region;
	region.set_size(size);
	if (portrait_crop_front) {
		region.position.x = Math::max(tl.x, 0.0f);
	}

	if (portrait_crop_top) {
		region.position.y = Math::max(tl.y, 0.0f);
	}

	region.set_end(size.min(br));
	portrait->set_region(region);

	Rect2 margin = portrait->get_margin();
	margin.size = br - tl.maxf(0.0);
	portrait->set_margin(margin);

	return portrait;
}
