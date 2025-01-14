#include "active/queued_effect.h"

void QueuedEffect::_bind_methods() {
	BIND_PROPERTY_RESOURCE(int64_t, card_instance);
	BIND_PROPERTY_RESOURCE(EffectInstance, effect);
	BIND_PROPERTY_ENUM_DICTIONARY_RESOURCE(enums::VariableDef::Variable, JigsawParameter, variables);

	ClassDB::bind_method(D_METHOD("assign", "queued"), &QueuedEffect::assign);
}

IMPLEMENT_PROPERTY(QueuedEffect, int64_t, card_instance);
IMPLEMENT_PROPERTY(QueuedEffect, Ref<EffectInstance>, effect);
IMPLEMENT_PROPERTY(QueuedEffect, VariableParameterDict, variables);

void QueuedEffect::assign(const Ref<QueuedEffect> &queued) {
	ERR_FAIL_COND(queued.is_null());

	_card_instance = queued->_card_instance;
	_effect = queued->_effect;
	_variables.assign(queued->_variables);
}
