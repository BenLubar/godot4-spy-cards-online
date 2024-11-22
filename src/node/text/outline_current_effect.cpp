#include "node/text/outline_current_effect.h"

#include "active/card_instance.h"
#include "defs/effect_instance.h"
#include "jigsaw/jigsaw_global.h"

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

	if (global->get_current_card_instance() != card) {
		return true;
	}

	if (global->get_current_effect_instance() != effect) {
		return true;
	}

	Ref<CardDesign> design = card->get_design();
	ERR_FAIL_COND_V(design.is_null(), false);

	// TODO: waiting for https://github.com/godotengine/godot/pull/97300
	p_char_fx->set_color(design->get_effect_highlight_color());

	return true;
}
