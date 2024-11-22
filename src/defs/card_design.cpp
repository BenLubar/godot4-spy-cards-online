#include "defs/card_design.h"

void CardDesign::_bind_methods() {
	BIND_ENUM_CONSTANT(GROW_LEFT);
	BIND_ENUM_CONSTANT(GROW_RIGHT);
	BIND_ENUM_CONSTANT(GROW_UP);
	BIND_ENUM_CONSTANT(GROW_DOWN);
	BIND_ENUM_CONSTANT(GROW_HORIZONTAL);
	BIND_ENUM_CONSTANT(GROW_VERTICAL);
	BIND_ENUM_CONSTANT(SHRINK_HORIZONTAL);
	BIND_ENUM_CONSTANT(SHRINK_VERTICAL);

	BIND_PROPERTY(Variant::FLOAT, pixel_size);
	BIND_PROPERTY(Variant::TRANSFORM3D, base_transform);
	BIND_PROPERTY(Variant::TRANSFORM3D, portrait_transform);
	BIND_PROPERTY(Variant::TRANSFORM3D, rank_decoration_transform);
	BIND_PROPERTY(Variant::VECTOR3, smooth_corners);

	BIND_PROPERTY_ENUM(enums::IconDef::Icon, card_front);
	BIND_PROPERTY_ENUM(enums::IconDef::Icon, card_front_window);
	BIND_PROPERTY_ENUM(enums::IconDef::Icon, tribe_bubble);
	BIND_PROPERTY_ENUM(enums::IconDef::Icon, tribe_bubble_wide);

	BIND_PROPERTY(Variant::RECT2, card_name_pos);
	BIND_PROPERTY(Variant::VECTOR2, card_name_scale);
	BIND_PROPERTY(Variant::RECT2, card_description_pos);
	BIND_PROPERTY(Variant::VECTOR2, card_description_scale);

	BIND_PROPERTY(Variant::RECT2, card_costs_pos);
	BIND_PROPERTY_ENUM(CardDesign::GrowMode, card_costs_grow_mode);
	BIND_PROPERTY(Variant::BOOL, card_costs_shrink_name);
	BIND_PROPERTY(Variant::FLOAT, card_costs_spacing);
	BIND_PROPERTY(Variant::VECTOR2, card_costs_text_scale);

	BIND_PROPERTY(Variant::RECT2, card_tribes_pos);
	BIND_PROPERTY_ENUM(CardDesign::GrowMode, card_tribes_grow_mode);
	BIND_PROPERTY(Variant::BOOL, card_tribes_shrink_name);
	BIND_PROPERTY(Variant::FLOAT, card_tribes_spacing);
	BIND_PROPERTY(Variant::VECTOR2, card_tribes_text_scale);

	BIND_PROPERTY(Variant::RECT2, simple_description_pos);
	BIND_PROPERTY_ENUM(CardDesign::GrowMode, simple_description_grow_mode);
	BIND_PROPERTY(Variant::FLOAT, simple_description_spacing);
	BIND_PROPERTY(Variant::FLOAT, simple_description_icon_align);
	BIND_PROPERTY(Variant::FLOAT, simple_description_text_align);
	BIND_PROPERTY(Variant::RECT2, simple_description_text_padding);
	BIND_PROPERTY(Variant::VECTOR2, simple_description_text_scale);

	BIND_PROPERTY_RESOURCE_ARRAY(StickerDef, stickers);

	BIND_PROPERTY(Variant::INT, effect_highlight_width);
	BIND_PROPERTY(Variant::COLOR, effect_highlight_color);
	BIND_PROPERTY(Variant::COLOR, description_background_color);
	BIND_PROPERTY(Variant::INT, simple_description_border_thickness);
	BIND_PROPERTY(Variant::COLOR, simple_description_border_color);
}

IMPLEMENT_PROPERTY(CardDesign, float, pixel_size);
IMPLEMENT_PROPERTY(CardDesign, Transform3D, base_transform);
IMPLEMENT_PROPERTY(CardDesign, Transform3D, portrait_transform);
IMPLEMENT_PROPERTY(CardDesign, Transform3D, rank_decoration_transform);
IMPLEMENT_PROPERTY(CardDesign, Vector3, smooth_corners);

IMPLEMENT_PROPERTY(CardDesign, enums::IconDef::Icon, card_front);
IMPLEMENT_PROPERTY(CardDesign, enums::IconDef::Icon, card_front_window);
IMPLEMENT_PROPERTY(CardDesign, enums::IconDef::Icon, tribe_bubble);
IMPLEMENT_PROPERTY(CardDesign, enums::IconDef::Icon, tribe_bubble_wide);

IMPLEMENT_PROPERTY(CardDesign, Rect2, card_name_pos);
IMPLEMENT_PROPERTY(CardDesign, Vector2, card_name_scale);
IMPLEMENT_PROPERTY(CardDesign, Rect2, card_description_pos);
IMPLEMENT_PROPERTY(CardDesign, Vector2, card_description_scale);

IMPLEMENT_PROPERTY(CardDesign, Rect2, card_costs_pos);
IMPLEMENT_PROPERTY(CardDesign, CardDesign::GrowMode, card_costs_grow_mode);
IMPLEMENT_PROPERTY(CardDesign, bool, card_costs_shrink_name);
IMPLEMENT_PROPERTY(CardDesign, float, card_costs_spacing);
IMPLEMENT_PROPERTY(CardDesign, Vector2, card_costs_text_scale);

IMPLEMENT_PROPERTY(CardDesign, Rect2, card_tribes_pos);
IMPLEMENT_PROPERTY(CardDesign, CardDesign::GrowMode, card_tribes_grow_mode);
IMPLEMENT_PROPERTY(CardDesign, bool, card_tribes_shrink_name);
IMPLEMENT_PROPERTY(CardDesign, float, card_tribes_spacing);
IMPLEMENT_PROPERTY(CardDesign, Vector2, card_tribes_text_scale);

IMPLEMENT_PROPERTY(CardDesign, Rect2, simple_description_pos);
IMPLEMENT_PROPERTY(CardDesign, CardDesign::GrowMode, simple_description_grow_mode);
IMPLEMENT_PROPERTY(CardDesign, float, simple_description_spacing);
IMPLEMENT_PROPERTY(CardDesign, float, simple_description_icon_align);
IMPLEMENT_PROPERTY(CardDesign, float, simple_description_text_align);
IMPLEMENT_PROPERTY(CardDesign, Rect2, simple_description_text_padding);

IMPLEMENT_PROPERTY(CardDesign, TypedArray<StickerDef>, stickers);

IMPLEMENT_PROPERTY(CardDesign, int64_t, effect_highlight_width);
IMPLEMENT_PROPERTY(CardDesign, Color, effect_highlight_color);
IMPLEMENT_PROPERTY(CardDesign, Color, description_background_color);
IMPLEMENT_PROPERTY(CardDesign, int64_t, simple_description_border_thickness);
IMPLEMENT_PROPERTY(CardDesign, Color, simple_description_border_color);
IMPLEMENT_PROPERTY(CardDesign, Vector2, simple_description_text_scale);
