#ifndef RICH_TEXT_RAINBOW_EFFECT_H
#define RICH_TEXT_RAINBOW_EFFECT_H

#include "dry.h"

#include <godot_cpp/classes/char_fx_transform.hpp>
#include <godot_cpp/classes/rich_text_effect.hpp>

class RichTextRainbowEffect : public RichTextEffect {
	GDCLASS(RichTextRainbowEffect, RichTextEffect);

protected:
	static void _bind_methods();

public:
	RichTextRainbowEffect() = default;
	~RichTextRainbowEffect() = default;

	DECLARE_PROPERTY(String, bbcode, = "sc_rainbow");

	bool _process_custom_fx(const Ref<CharFXTransform> &p_char_fx) const override;
};

#endif // RICH_TEXT_RAINBOW_EFFECT_H
