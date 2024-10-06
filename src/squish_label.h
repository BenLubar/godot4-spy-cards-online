#ifndef SQUISH_LABEL_H
#define SQUISH_LABEL_H

#include "dry.h"

#include "card_instance.h"
#include "formatted_text.h"
#include "outline_current_effect.h"
#include "rich_text_rainbow_effect.h"
#include "rich_text_shake_effect.h"
#include "rich_text_glitch_effect.h"

#include <godot_cpp/classes/rich_text_label.hpp>

class SquishLabel : public RichTextLabel {
	GDCLASS(SquishLabel, RichTextLabel);

protected:
	static void _bind_methods();

private:
	Ref<OutlineCurrentEffect> _outline_current_effect;
	Ref<RichTextRainbowEffect> _rainbow_effect;
	Ref<RichTextShakeEffect> _shake_effect;
	Ref<RichTextGlitchEffect> _glitch_effect;

public:
	SquishLabel();
	~SquishLabel();

	DECLARE_PROPERTY(Vector2, max_size, = Vector2(0, 0));
	DECLARE_PROPERTY(Vector2, text_scale, = Vector2(1, 1));
	DECLARE_PROPERTY_IS(bool, centered, = false);

	void _resize();
	void set_formatted_text(const TypedArray<FormattedText> &formatted_text, const Ref<CardInstance> &card);
};

#endif // SQUISH_LABEL_H
