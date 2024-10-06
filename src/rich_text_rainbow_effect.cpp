#include "rich_text_rainbow_effect.h"

void RichTextRainbowEffect::_bind_methods() {
	BIND_PROPERTY(Variant::STRING, bbcode);
}

IMPLEMENT_PROPERTY(RichTextRainbowEffect, String, bbcode);

bool RichTextRainbowEffect::_process_custom_fx(const Ref<CharFXTransform> &p_char_fx) const {
	if (p_char_fx->is_outline()) {
		return true;
	}

	double now = p_char_fx->get_elapsed_time();
	now += p_char_fx->get_relative_index();

	double r = sin((now + 20.0) * 5.9) * 2.0;
	double g = sin((now) * 5.9) * 2.0;
	double b = sin((now + 60.0) * 5.9) * 2.0;
	p_char_fx->set_color(Color(Math::clamp(r, 0.0, 1.0), Math::clamp(g, 0.0, 1.0), Math::clamp(b, 0.0, 1.0), p_char_fx->get_color().a));

	return true;
}
