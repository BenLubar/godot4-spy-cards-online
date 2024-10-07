#include "outline_current_effect.h"

#include "card_instance.h"
#include "effect_instance.h"

void OutlineCurrentEffect::_bind_methods() {
}

bool OutlineCurrentEffect::_process_custom_fx(const Ref<CharFXTransform> &p_char_fx) const {
	if (!p_char_fx->is_outline()) {
		return true;
	}

	Dictionary env = p_char_fx->get_environment();
	CardInstance *card = Object::cast_to<CardInstance>(env["card"]);
	EffectInstance *effect = Object::cast_to<EffectInstance>(env["effect"]);

	ERR_FAIL_NULL_V(card, false);
	ERR_FAIL_NULL_V(effect, false);

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
