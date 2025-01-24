#include "jigsaw/jigsaw_global.h"

#include "jigsaw/jigsaw_visual.h"
#include "jigsaw/parameter/jigsaw_parameter_amount.h"
#include "jigsaw/parameter/jigsaw_parameter_card.h"
#include "jigsaw/parameter/jigsaw_parameter_character.h"
#include "jigsaw/parameter/jigsaw_parameter_ordered_list.h"

#include <godot_cpp/classes/time.hpp>

void JigsawGlobal::_bind_methods() {
	BIND_PROPERTY_RESOURCE(JigsawInputSource, input_source);
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
	BIND_PROPERTY_RESOURCE_ARRAY(MeshInstance3D, character_nodes);

	ADD_SIGNAL(MethodInfo("current_effect_changed"));

	ClassDB::bind_method(D_METHOD("init_sides"), &JigsawGlobal::init_sides);

	ClassDB::bind_method(D_METHOD("run_procedure_sync", "procedure", "arguments", "results", "rng", "parent"), &JigsawGlobal::run_procedure_sync, DEFVAL(Ref<JigsawContext>()));
	ClassDB::bind_method(D_METHOD("run_select", "side", "procedure", "callback"), &JigsawGlobal::run_select);

	ClassDB::bind_method(D_METHOD("run_mode_init", "timestamp", "shared_seed"), &JigsawGlobal::run_mode_init);
	ClassDB::bind_method(D_METHOD("run_mode_trigger", "type", "arguments", "rng", "parent"), &JigsawGlobal::run_mode_trigger, DEFVAL(Ref<JigsawContext>()));
	ClassDB::bind_method(D_METHOD("run_character_select", "side"), &JigsawGlobal::run_character_select);
	ClassDB::bind_method(D_METHOD("run_deck_builder", "side"), &JigsawGlobal::run_deck_builder);
}

IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, JigsawInputSource *, input_source);
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
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, TypedArray<MeshInstance3D>, character_nodes);

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

		TypedArray<PackedInt32Array> locations;
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

void JigsawGlobal::run_procedure_sync(const Ref<JigsawProcedure> &procedure, const TypedArray<JigsawParameter> &arguments, const TypedArray<JigsawParameter> &results, const Ref<RNG> &rng, const Ref<JigsawContext> &parent) {
	Ref<JigsawContext> context = JigsawContext::make(this, parent);
	context->set_rng(rng);

	_context_stack.append(context);

	Ref<JigsawError> err = context->evaluate(procedure, arguments, results);
	if (unlikely(err.is_valid())) {
		_input_source->on_jigsaw_error(err);
	}

	ERR_FAIL_COND(_context_stack.back() != context);
	_context_stack.remove_at(_context_stack.size() - 1);
}

void JigsawGlobal::run_select(int64_t side, const Ref<JigsawProcedure> &procedure, const Callable &callback) {
	TypedArray<JigsawParameter> results = procedure->get_results().duplicate();
	run_procedure_sync(procedure, Array(), results, Ref<RNG>()); // TODO
	callback.callv(results);
}

void JigsawGlobal::run_mode_init(uint64_t timestamp, const PackedByteArray &shared_seed) {
	Dictionary datetime = Time::get_singleton()->get_datetime_dict_from_unix_time(timestamp / 1000);
	Ref<RNG> rng = RNG::with_seed(shared_seed);

	TypedArray<JigsawParameter> args = Array::make(
		JigsawParameterAmount::make(datetime["year"]),
		JigsawParameterAmount::make(datetime["month"]),
		JigsawParameterAmount::make(datetime["day"]),
		JigsawParameterAmount::make(shared_seed.decode_u32(0))
	);

	run_mode_trigger(JigsawTriggerVariant::COSMETIC_INIT, args, rng);
}

void JigsawGlobal::run_mode_trigger(JigsawTriggerVariant::Type type, const TypedArray<JigsawParameter> &arguments, const Ref<RNG> &rng, const Ref<JigsawContext> &parent) {
	TypedArray<JigsawTriggerVariant> triggers = _mode->get_base_triggers() + _selected_variant->get_triggers();
	for (int64_t i = 0; i < triggers.size(); i++) {
		Ref<JigsawTriggerVariant> trigger = triggers[i];
		if (trigger->get_type() != type) {
			continue;
		}

		run_procedure_sync(trigger, arguments.duplicate(), TypedArray<JigsawParameter>(), rng, parent);
	}
}

static void unwrap_character(const Ref<JigsawParameterCharacter> &character, const Callable &callback) {
	callback.call(character->get_character());
}

void JigsawGlobal::run_character_select(int64_t side, const Callable &on_character) {
	Ref<JigsawProcedureVariantSelectCharacter> select_character = _selected_variant->get_select_character();
	if (select_character.is_null()) {
		on_character.call(enums::CharacterDef::NONE);
		return;
	}

	run_select(side, select_character, callable_mp_static(&unwrap_character).bind(on_character));
}

static enums::CardDef::Card unwrap_single_card(const Ref<JigsawParameterCard> &card) {
	return card->get_card();
}

static void unwrap_deck(const Ref<JigsawParameterOrderedList> &cards, const Callable &callback) {
	callback.call(PackedArray<enums::CardDef::Card>(cards->get_list().map(callable_mp_static(&unwrap_single_card))));
}

void JigsawGlobal::run_deck_builder(int64_t side, const Callable &on_deck) {
	Ref<JigsawProcedureVariantBuildDeck> build_deck = _selected_variant->get_build_deck();
	if (build_deck.is_null()) {
		on_deck.call(PackedArray<enums::CardDef::Card>());
		return;
	}

	run_select(side, build_deck, callable_mp_static(&unwrap_deck).bind(on_deck));
}
