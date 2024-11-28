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
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawSide, sides);

	BIND_PROPERTY(Variant::FLOAT, time_scale);
	BIND_PROPERTY(Variant::INT, pause_frames);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawContext, context_stack);

	BIND_PROPERTY_RESOURCE_ARRAY(Node3D, scene_nodes);
	BIND_PROPERTY_RESOURCE_ARRAY(Sprite3D, sprite_nodes);
	BIND_PROPERTY_RESOURCE_ARRAY(SquishLabel, label_nodes);
	BIND_PROPERTY_RESOURCE_ARRAY(TextureRect, icon_nodes);
	BIND_PROPERTY_RESOURCE(Audience, audience);
	BIND_PROPERTY_RESOURCE_ARRAY(MeshInstance3D, character_nodes);
	BIND_PROPERTY_RESOURCE_ARRAY(CardGrid2D, card_grids_2d);
	BIND_PROPERTY_RESOURCE_ARRAY(CardGrid3D, card_grids_3d);

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
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, TypedArray<JigsawSide>, sides);

IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, double, time_scale);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, uint32_t, pause_frames);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, TypedArray<JigsawContext>, context_stack);

IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, TypedArray<Node3D>, scene_nodes);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, TypedArray<Sprite3D>, sprite_nodes);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, TypedArray<SquishLabel>, label_nodes);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, TypedArray<TextureRect>, icon_nodes);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, Ref<Audience>, audience);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, TypedArray<MeshInstance3D>, character_nodes);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, TypedArray<CardGrid2D>, card_grids_2d);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, TypedArray<CardGrid3D>, card_grids_3d);

JigsawGlobal::~JigsawGlobal() {
	for (int64_t i = 0; i < _scene_nodes.size(); i++) {
		Node3D *node = Object::cast_to<Node3D>(_scene_nodes[i]);
		node->queue_free();
	}
	for (int64_t i = 0; i < _sprite_nodes.size(); i++) {
		Sprite3D *sprite = Object::cast_to<Sprite3D>(_sprite_nodes[i]);
		sprite->queue_free();
	}
	for (int64_t i = 0; i < _label_nodes.size(); i++) {
		SquishLabel *label = Object::cast_to<SquishLabel>(_label_nodes[i]);
		label->queue_free();
	}
	for (int64_t i = 0; i < _icon_nodes.size(); i++) {
		TextureRect *icon = Object::cast_to<TextureRect>(_icon_nodes[i]);
		icon->queue_free();
	}
	for (int64_t i = 0; i < _character_nodes.size(); i++) {
		MeshInstance3D *character = Object::cast_to<MeshInstance3D>(_character_nodes[i]);
		character->queue_free();
	}
	for (int64_t i = 0; i < _card_grids_2d.size(); i++) {
		CardGrid2D *grid = Object::cast_to<CardGrid2D>(_card_grids_2d[i]);
		grid->queue_free();
	}
	for (int64_t i = 0; i < _card_grids_3d.size(); i++) {
		CardGrid3D *grid = Object::cast_to<CardGrid3D>(_card_grids_3d[i]);
		grid->queue_free();
	}
}

void JigsawGlobal::init_sides() {
	ERR_FAIL_COND(_mode.is_null());
	ERR_FAIL_COND(_mode->get_variants().is_empty());
	ERR_FAIL_COND(_selected_variant.is_null());

	int64_t bug_players = _selected_variant->get_player_count();
	int64_t bot_players = _selected_variant->get_npcs().size();
	_sides.resize(1 + bug_players + bot_players);
	for (int64_t i = 0; i < 1 + bug_players + bot_players; i++) {
		Ref<JigsawSide> side;
		side.instantiate();

		TypedArray<JigsawSideLocation> locations;
		locations.resize(enums::LocationDef::FIRST_CUSTOM + _mode->get_custom_locations().size());
		for (int64_t j = 0; j < locations.size(); j++) {
			locations[i] = memnew(JigsawSideLocation);
		}

		side->set_locations(locations);

		TypedArray<NumericValue> stats;
		stats.resize(enums::StatDef::FIRST_CUSTOM + _mode->get_custom_stats().size());
		TypedArray<enums::StatDef::Stat> used_stats = _mode->get_stats();
		for (int64_t j = 0; j < stats.size(); j++) {
			if (used_stats.has(j)) {
				stats[j] = memnew(NumericValue);
			}
		}
		side->set_stats(stats);

		_sides[i] = side;
	}
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
