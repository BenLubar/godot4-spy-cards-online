#include "outline_current_effect.h"

#include "card_instance.h"
#include "effect_instance.h"
#include "jigsaw_global.h"

void OutlineCurrentEffect::_bind_methods() {
}

bool OutlineCurrentEffect::_process_custom_fx(const Ref<CharFXTransform> &p_char_fx) const {
	if (!p_char_fx->is_outline()) {
		return true;
	}

	Dictionary env = p_char_fx->get_environment();
	Ref<CardInstance> card = env["card"];
	Ref<EffectInstance> effect = env["effect"];

	ERR_FAIL_COND_V(card.is_null(), false);
	ERR_FAIL_COND_V(effect.is_null(), false);

	JigsawGlobal *global = card->get_global();
	ERR_FAIL_NULL_V(global, false);
	Ref<GameMode> mode = global->get_mode();
	ERR_FAIL_COND_V(mode.is_null(), false);

	if (global->get_current_card_instance() != card) {
		return true;
	}

	if (global->get_current_effect_instance() != effect) {
		return true;
	}

	// TODO: waiting for https://github.com/godotengine/godot/pull/97300
	p_char_fx->set_color(mode->get_visual_effect_highlight_color());

	return true;
}
