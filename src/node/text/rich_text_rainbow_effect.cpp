#include "node/text/rich_text_rainbow_effect.h"

void RichTextRainbowEffect::_bind_methods() {
	BIND_PROPERTY(Variant::STRING, bbcode);
}

IMPLEMENT_PROPERTY(RichTextRainbowEffect, String, bbcode);

bool RichTextRainbowEffect::_process_custom_fx(const Ref<CharFXTransform> &p_char_fx) const {
	if (p_char_fx->is_outline()) {
		return true;
	}

	float now = p_char_fx->get_elapsed_time();
	now += p_char_fx->get_relative_index();

	float r = Math::sin((now + 20.0f) * 5.9f) * 2.0f;
	float g = Math::sin((now) * 5.9f) * 2.0f;
	float b = Math::sin((now + 60.0f) * 5.9f) * 2.0f;
	p_char_fx->set_color(Color(Math::clamp(r, 0.0f, 1.0f), Math::clamp(g, 0.0f, 1.0f), Math::clamp(b, 0.0f, 1.0f), p_char_fx->get_color().a));

	return true;
}
