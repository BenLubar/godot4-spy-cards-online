#include "jigsaw_global.h"

void JigsawGlobal::_bind_methods() {
	BIND_PROPERTY_RESOURCE(GameMode, mode);
	BIND_PROPERTY_RESOURCE(VariantDef, selected_variant);
	BIND_PROPERTY_RESOURCE(CardInstance, current_card_instance);
	BIND_PROPERTY_RESOURCE(EffectInstance, current_effect_instance);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawSide, sides);

	BIND_PROPERTY(Variant::FLOAT, time_scale);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawContext, context_stack);
	BIND_PROPERTY_RESOURCE(RNG, rng);

	BIND_PROPERTY_RESOURCE_ARRAY(Node3D, scene_nodes);
	BIND_PROPERTY_RESOURCE_ARRAY(Sprite3D, sprite_nodes);
	BIND_PROPERTY_RESOURCE_ARRAY(SquishLabel, label_nodes);
	BIND_PROPERTY_RESOURCE_ARRAY(TextureRect, icon_nodes);
	BIND_PROPERTY_RESOURCE(Audience, audience);
	BIND_PROPERTY_RESOURCE_ARRAY(MeshInstance3D, character_nodes);
	BIND_PROPERTY_RESOURCE_ARRAY(CardGrid, card_grids);

	ADD_SIGNAL(MethodInfo("current_effect_changed"));

	ClassDB::bind_method(D_METHOD("init_sides"), &JigsawGlobal::init_sides);
}

IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, Ref<GameMode>, mode);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, Ref<VariantDef>, selected_variant);
IMPLEMENT_PROPERTY_ONCHANGE(JigsawGlobal, Ref<CardInstance>, current_card_instance, emit_signal("current_effect_changed"));
IMPLEMENT_PROPERTY_ONCHANGE(JigsawGlobal, Ref<EffectInstance>, current_effect_instance, emit_signal("current_effect_changed"));
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, TypedArray<JigsawSide>, sides);

IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, double, time_scale);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, TypedArray<JigsawContext>, context_stack);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, Ref<RNG>, rng);

IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, TypedArray<Node3D>, scene_nodes);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, TypedArray<Sprite3D>, sprite_nodes);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, TypedArray<SquishLabel>, label_nodes);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, TypedArray<TextureRect>, icon_nodes);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, Ref<Audience>, audience);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, TypedArray<MeshInstance3D>, character_nodes);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, TypedArray<CardGridNative>, card_grids);

void JigsawGlobal::init_sides() {
	ERR_FAIL_COND(_mode.is_null());
	ERR_FAIL_COND(_mode->get_base_variant().is_null());
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
