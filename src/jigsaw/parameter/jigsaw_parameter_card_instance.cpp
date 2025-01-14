#include "jigsaw/parameter/jigsaw_parameter_card_instance.h"

#include "jigsaw/jigsaw_global.h"

void JigsawParameterCardInstance::_bind_methods() {
	BIND_PROPERTY(Variant::INT, instance);

	ClassDB::bind_static_method("JigsawParameterCardInstance", D_METHOD("make", "instance"), &JigsawParameterCardInstance::make);
}

IMPLEMENT_PROPERTY(JigsawParameterCardInstance, int64_t, instance);

Ref<CardInstance> JigsawParameterCardInstance::resolve(JigsawGlobal *global) const {
	ERR_FAIL_NULL_V(global, Ref<CardInstance>());

	Ref<JigsawState> state = global->get_state();
	ERR_FAIL_COND_V(state.is_null(), Ref<CardInstance>());

	TypedArray<CardInstance> cards = state->get_cards();
	if (get_instance() < 0 || get_instance() >= cards.size()) {
		return Ref<CardInstance>();
	}

	return cards[get_instance()];
}

Ref<JigsawParameterCardInstance> JigsawParameterCardInstance::make(int64_t instance) {
	Ref<JigsawParameterCardInstance> param;
	param.instantiate();
	param->set_instance(instance);
	return param;
}
