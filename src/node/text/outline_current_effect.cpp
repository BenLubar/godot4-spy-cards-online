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

	JigsawGlobal *global = Object::cast_to<JigsawGlobal>(env["global"].get_validated_object());
	ERR_FAIL_NULL_V(global, false);

	int64_t card_index = env["card"];

	Ref<EffectInstance> effect = env["effect"];
	ERR_FAIL_COND_V(effect.is_null(), false);

	TypedArray<CardInstance> cards = global->get_state()->get_cards();
	ERR_FAIL_INDEX_V(card_index, cards.size(), false);

	Ref<CardInstance> card = cards[card_index];
	ERR_FAIL_COND_V(card.is_null(), false);

	if (global->get_current_card_instance() != card) {
		return true;
	}

	if (global->get_current_effect_instance() != effect) {
		return true;
	}

	Ref<CardDesign> design = card->get_design(global);
	ERR_FAIL_COND_V(design.is_null(), false);

	// TODO: waiting for https://github.com/godotengine/godot/pull/97300
	p_char_fx->set_color(design->get_effect_highlight_color());

	return true;
}
