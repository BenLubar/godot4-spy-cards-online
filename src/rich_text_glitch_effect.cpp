#include "rich_text_glitch_effect.h"

#include <godot_cpp/classes/text_server_manager.hpp>

void RichTextGlitchEffect::_bind_methods() {
	BIND_PROPERTY(Variant::STRING, bbcode);
}

IMPLEMENT_PROPERTY(RichTextGlitchEffect, String, bbcode);

bool RichTextGlitchEffect::_process_custom_fx(const Ref<CharFXTransform> &p_char_fx) const {
	Dictionary env = p_char_fx->get_environment();
	TypedDictionary<RID, PackedInt32Array> sorted_glyphs = env.get_or_add("_sorted_font_glyphs", TypedDictionary<RID, PackedInt32Array>());

	RID font = p_char_fx->get_font();

	if (!sorted_glyphs.has(font)) {
		Ref<TextServer> text_server = TextServerManager::get_singleton()->get_primary_interface();

		Array supported_glyphs = text_server->font_get_supported_glyphs(font);
		supported_glyphs.shuffle();
		supported_glyphs.sort_custom(callable_mp_static(&RichTextGlitchEffect::_compare_glyph_widths).bind(font, text_server));
		sorted_glyphs[font] = PackedInt32Array(supported_glyphs);
	}

	// TODO
	return true;
}

bool RichTextGlitchEffect::_compare_glyph_widths(int32_t a, int32_t b, RID font, Ref<TextServer> text_server) {
	constexpr int64_t SIZE = 16;
	float advance_a = text_server->font_get_glyph_advance(font, SIZE, a).x;
	float advance_b = text_server->font_get_glyph_advance(font, SIZE, b).x;
	return Math::fast_ftoi(advance_a) < Math::fast_ftoi(advance_b);
}
