#ifndef RICH_TEXT_GLITCH_EFFECT_H
#define RICH_TEXT_GLITCH_EFFECT_H

#include "dry.h"

#include <godot_cpp/classes/char_fx_transform.hpp>
#include <godot_cpp/classes/rich_text_effect.hpp>
#include <godot_cpp/classes/text_server.hpp>

class RichTextGlitchEffect : public RichTextEffect {
	GDCLASS(RichTextGlitchEffect, RichTextEffect);

protected:
	static void _bind_methods();

public:
	RichTextGlitchEffect() = default;
	~RichTextGlitchEffect() = default;

	DECLARE_PROPERTY(String, bbcode, = "sc_glitch");

	bool _process_custom_fx(const Ref<CharFXTransform> &p_char_fx) const override;
	static bool _compare_glyph_widths(int32_t a, int32_t b, RID font, Ref<TextServer> text_server);
};

#endif // RICH_TEXT_GLITCH_EFFECT_H
