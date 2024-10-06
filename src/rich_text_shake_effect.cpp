#include "rich_text_shake_effect.h"

void RichTextShakeEffect::_bind_methods() {
	BIND_CONSTANT(DEFAULT_RATE);
	BIND_CONSTANT(DEFAULT_STRENGTH);

	BIND_PROPERTY(Variant::STRING, bbcode);
}

RichTextShakeEffect::RichTextShakeEffect() {
	_rng.instantiate();
	_rng->randomize();
}

IMPLEMENT_PROPERTY(RichTextShakeEffect, String, bbcode);

static uint64_t rotate_i64(uint64_t i, int64_t rotate) {
	rotate &= 63;
	return (i >> rotate) | (i << ((-rotate) & 63));
}

bool RichTextShakeEffect::_process_custom_fx(const Ref<CharFXTransform> &p_char_fx) const {
	Dictionary env = p_char_fx->get_environment();
	double rate = env.get("rate", DEFAULT_RATE);
	double strength = env.get("strength", DEFAULT_STRENGTH);
	strength /= 10.0;

	uint64_t prev_rand = env.get("_prev_rand", 0);
	uint64_t cur_rand = env.get("_cur_rand", 0);

	int64_t window = Math::floor(p_char_fx->get_elapsed_time() * rate);
	if (int64_t(env.get("_window", -1)) != window) {
		prev_rand = cur_rand;
		env["_prev_rand"] = prev_rand;
		cur_rand = (uint64_t(_rng->randi()) << 32) | _rng->randi();
		env["_cur_rand"] = cur_rand;
		env["_window"] = window;
		p_char_fx->set_environment(env);
	}

	double prev_offset = Math::fposmod(rotate_i64(prev_rand, p_char_fx->get_relative_index()) / 2147483647.0, 1.0) * Math_TAU;
	double cur_offset = Math::fposmod(rotate_i64(cur_rand, p_char_fx->get_relative_index()) / 2147483647.0, 1.0) * Math_TAU;
	double interp = Math::min(Math::fmod(p_char_fx->get_elapsed_time(), 1.0 / rate) * 2.0, 1.0);

	p_char_fx->set_offset(p_char_fx->get_offset() + Vector2(Math::cos(prev_offset), Math::sin(prev_offset)).lerp(Vector2(Math::cos(cur_offset), Math::sin(cur_offset)), interp) * strength);

	return true;
}
