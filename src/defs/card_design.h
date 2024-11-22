#ifndef CARD_DESIGN_H
#define CARD_DESIGN_H

#include "dry.h"

class CardDesign;

#include "enums/enums_icon_def.h"
#include "defs/sticker_def.h"

class CardDesign : public Resource {
	GDCLASS(CardDesign, Resource);

public:
	enum GrowMode {
		GROW_LEFT = 0,
		GROW_RIGHT = 1,
		GROW_UP = 2,
		GROW_DOWN = 3,
		GROW_HORIZONTAL = 4,
		GROW_VERTICAL = 5,
		SHRINK_HORIZONTAL = 6,
		SHRINK_VERTICAL = 7,
	};

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(float, pixel_size, = 75.0f);
	DECLARE_PROPERTY(Transform3D, base_transform, = Transform3D(Basis(Vector3(2.32f, 0.0f, 0.0f), Vector3(0.0f, 3.16f, 0.0f), Vector3(0.0f, 0.0f, 1.0f)), Vector3(0.0f, 0.0f, 0.0f)));
	DECLARE_PROPERTY(Transform3D, portrait_transform, = Transform3D(Basis(Vector3(1.28f, 0.0f, 0.0f), Vector3(0.0f, 1.28f, 0.0f), Vector3(0.0f, 0.0f, 1.0f)), Vector3(0.0f, 0.45f, -0.05f)));
	DECLARE_PROPERTY(Transform3D, rank_decoration_transform, = Transform3D(Basis(Vector3(0.533333f, 0.0f, 0.0f), Vector3(0.0f, 1.466667f, 0.0f), Vector3(0.0f, 0.0f, 1.0f)), Vector3(0.83f, 0.44f, 0.0f)));
	DECLARE_PROPERTY(Vector3, smooth_corners, = Vector3(174.0f / 5.0f, 237.0f / 5.0f, 1.0f));

	DECLARE_PROPERTY(enums::IconDef::Icon, card_front, = enums::IconDef::Icon::CARD_FRONT);
	DECLARE_PROPERTY(enums::IconDef::Icon, card_front_window, = enums::IconDef::Icon::CARD_FRONT_WINDOW);
	DECLARE_PROPERTY(enums::IconDef::Icon, tribe_bubble, = enums::IconDef::Icon::CARD_TRIBE_BUBBLE);
	DECLARE_PROPERTY(enums::IconDef::Icon, tribe_bubble_wide, = enums::IconDef::Icon::CARD_TRIBE_BUBBLE_WIDE);

	DECLARE_PROPERTY(Rect2, card_name_pos, = Rect2(12, 9, 146, 0));
	DECLARE_PROPERTY(Vector2, card_name_scale, = Vector2(1.0f, 1.0f));
	DECLARE_PROPERTY(Rect2, card_description_pos, = Rect2(18, 140, 136, 76));
	DECLARE_PROPERTY(Vector2, card_description_scale, = Vector2(1.0f, 1.25f));

	DECLARE_PROPERTY(Rect2, card_costs_pos, = Rect2(136, 4, 32, 32));
	DECLARE_PROPERTY(GrowMode, card_costs_grow_mode, = GROW_LEFT);
	DECLARE_PROPERTY(bool, card_costs_shrink_name, = true);
	DECLARE_PROPERTY(float, card_costs_spacing, = 0.0f);
	DECLARE_PROPERTY(Vector2, card_costs_text_scale, = Vector2(1.0f, 1.2f));

	DECLARE_PROPERTY(Rect2, card_tribes_pos, = Rect2(5, 217, 164, 15));
	DECLARE_PROPERTY(GrowMode, card_tribes_grow_mode, = SHRINK_HORIZONTAL);
	DECLARE_PROPERTY(bool, card_tribes_shrink_name, = false);
	DECLARE_PROPERTY(float, card_tribes_spacing, = 0.0f);
	DECLARE_PROPERTY(Vector2, card_tribes_text_scale, = Vector2(1.0f, 1.0f));

	DECLARE_PROPERTY(Rect2, simple_description_pos, = Rect2(18, 146, 136, 64));
	DECLARE_PROPERTY(GrowMode, simple_description_grow_mode, = SHRINK_HORIZONTAL);
	DECLARE_PROPERTY(float, simple_description_spacing, = 0.0f);
	DECLARE_PROPERTY(float, simple_description_icon_align, = -0.7f);
	DECLARE_PROPERTY(float, simple_description_text_align, = 1.0f);
	DECLARE_PROPERTY(Rect2, simple_description_text_padding, = Rect2(0, 0, 16, 0));
	DECLARE_PROPERTY(Vector2, simple_description_text_scale, = Vector2(1.0f, 1.2f));

	DECLARE_PROPERTY(TypedArray<StickerDef>, stickers);

	DECLARE_PROPERTY(int64_t, effect_highlight_width, = 5);
	DECLARE_PROPERTY(Color, effect_highlight_color, = Color("#44aaff"));
	DECLARE_PROPERTY(Color, description_background_color, = Color("#efefef"));
	DECLARE_PROPERTY(int64_t, simple_description_border_thickness, = 10);
	DECLARE_PROPERTY(Color, simple_description_border_color, = Color("#000000"));
};
DECLARE_ENUM(CardDesign::GrowMode);

#endif // CARD_DESIGN_H
