#include "jigsaw/jigsaw_global.h"

#include "jigsaw/jigsaw_visual.h"

void JigsawGlobal::_bind_methods() {
	BIND_PROPERTY_RESOURCE(JigsawVisual, visual);
	BIND_PROPERTY_RESOURCE(GameMode, mode);
	BIND_PROPERTY_RESOURCE(VariantDef, selected_variant);

	BIND_PROPERTY_RESOURCE(CardInstance, current_card_instance);
	BIND_PROPERTY_RESOURCE(EffectInstance, current_effect_instance);
	BIND_PROPERTY_RESOURCE(QueuedEffect, current_queued_effect);
	BIND_PROPERTY(Variant::INT, current_side);

	BIND_PROPERTY_RESOURCE(JigsawState, state);

	BIND_PROPERTY(Variant::FLOAT, time_scale);
	BIND_PROPERTY(Variant::INT, pause_frames);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawContext, context_stack);

	BIND_PROPERTY(Variant::INT, queue_reset_count);
	BIND_PROPERTY_RESOURCE_ARRAY(QueuedEffect, queue);

	BIND_PROPERTY_RESOURCE_ARRAY(JigsawSound, sounds);

	ADD_SIGNAL(MethodInfo("current_effect_changed"));

	ClassDB::bind_method(D_METHOD("init_sides"), &JigsawGlobal::init_sides);
	ClassDB::bind_method(D_METHOD("run_variant_triggers", "type", "args", "rng", "copy_rng"), &JigsawGlobal::run_variant_triggers);
}

IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, JigsawVisual *, visual);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, Ref<GameMode>, mode);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, Ref<VariantDef>, selected_variant);
IMPLEMENT_PROPERTY_ONCHANGE(JigsawGlobal, Ref<CardInstance>, current_card_instance, emit_signal("current_effect_changed"));
IMPLEMENT_PROPERTY_ONCHANGE(JigsawGlobal, Ref<EffectInstance>, current_effect_instance, emit_signal("current_effect_changed"));
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, Ref<QueuedEffect>, current_queued_effect);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, int32_t, current_side);

IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, Ref<JigsawState>, state);

IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, double, time_scale);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, uint32_t, pause_frames);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, TypedArray<JigsawContext>, context_stack);

IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, int64_t, queue_reset_count);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, TypedArray<QueuedEffect>, queue);

IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, TypedArray<JigsawSound>, sounds);

JigsawGlobal::~JigsawGlobal() {
	for (int64_t i = 0; i < _sounds.size(); i++) {
		Ref<JigsawSound> sound = _sounds[i];
		if (sound.is_valid()) {
			sound->kill_node();
		}
	}
}

void JigsawGlobal::init_sides() {
	ERR_FAIL_COND(_mode.is_null());
	ERR_FAIL_COND(_mode->get_variants().is_empty());
	ERR_FAIL_COND(_selected_variant.is_null());
	ERR_FAIL_COND(_state.is_null());

	int64_t bug_players = _selected_variant->get_player_count();
	int64_t bot_players = _selected_variant->get_npcs().size();
	TypedArray<JigsawSide> sides;
	sides.resize(1 + bug_players + bot_players);
	for (int64_t i = 0; i < 1 + bug_players + bot_players; i++) {
		Ref<JigsawSide> side;
		side.instantiate();

		TypedArray<PackedInt64Array> locations;
		locations.resize(enums::LocationDef::FIRST_CUSTOM + _mode->get_custom_locations().size());
		side->set_location_card_instances(locations);

		TypedArray<NumericValue> stats;
		stats.resize(enums::StatDef::FIRST_CUSTOM + _mode->get_custom_stats().size());
		PackedArray<enums::StatDef::Stat> used_stats = _mode->get_stats();
		for (int64_t j = 0; j < stats.size(); j++) {
			if (used_stats.has(j)) {
				stats[j] = memnew(NumericValue);
			}
		}
		side->set_stats(stats);

		sides[i] = side;
	}

	_state->set_sides(sides);
}

Ref<JigsawError> JigsawGlobal::run_variant_triggers(JigsawTriggerVariant::Type type, const TypedArray<JigsawParameter> &args, const Ref<RNG> &rng, bool copy_rng) {
	ERR_FAIL_COND_V(!_context_stack.is_empty(), Ref<JigsawError>());

	TypedArray<JigsawTriggerVariant> triggers;

	if (likely(_mode.is_valid())) {
		triggers.append_array(_mode->get_base_triggers());
	}

	if (likely(_selected_variant.is_valid())) {
		triggers.append_array(_selected_variant->get_triggers());
	}

	for (int64_t i = 0; i < triggers.size(); i++) {
		Ref<JigsawTriggerVariant> trigger = triggers[i];
		if (trigger->get_type() == type) {
			Ref<JigsawContext> context = JigsawContext::make(this, Ref<JigsawContext>());
			context->set_rng(copy_rng ? rng->duplicate() : rng);
			_context_stack.push_back(context);
			Ref<JigsawError> err = context->evaluate(trigger, args, TypedArray<JigsawParameter>());
			_context_stack.pop_back();
			if (unlikely(err.is_valid())) {
				return err;
			}
		}
	}

	return Ref<JigsawError>();
}
