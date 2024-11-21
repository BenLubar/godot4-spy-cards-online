#ifndef JIGSAW_GLOBAL_H
#define JIGSAW_GLOBAL_H

#include "dry.h"

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/sprite3d.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/templates/hash_map.hpp>

class JigsawGlobal;
class JigsawVisual;

#include "audience.h"
#include "card_grid.h"
#include "card_instance.h"
#include "effect_instance.h"
#include "game_mode.h"
#include "jigsaw_context.h"
#include "jigsaw_side.h"
#include "queued_effect.h"
#include "rng.h"
#include "squish_label.h"
#include "variant_def.h"

class JigsawGlobal : public Node {
	GDCLASS(JigsawGlobal, Node);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(JigsawVisual *, visual, = nullptr);
	DECLARE_PROPERTY(Ref<GameMode>, mode);
	DECLARE_PROPERTY(Ref<VariantDef>, selected_variant);
	DECLARE_PROPERTY(Ref<CardInstance>, current_card_instance);
	DECLARE_PROPERTY(Ref<EffectInstance>, current_effect_instance);
	DECLARE_PROPERTY(Ref<QueuedEffect>, current_queued_effect);
	DECLARE_PROPERTY(int32_t, current_side, = 0);
	DECLARE_PROPERTY(TypedArray<JigsawSide>, sides);

	DECLARE_PROPERTY(double, time_scale, = 1.0);
	DECLARE_PROPERTY(double, pause_time, = 0.0);
	DECLARE_PROPERTY(TypedArray<JigsawContext>, context_stack);

	DECLARE_PROPERTY(TypedArray<Node3D>, scene_nodes);
	DECLARE_PROPERTY(TypedArray<Sprite3D>, sprite_nodes);
	DECLARE_PROPERTY(TypedArray<SquishLabel>, label_nodes);
	DECLARE_PROPERTY(TypedArray<TextureRect>, icon_nodes);
	DECLARE_PROPERTY(Ref<Audience>, audience);
	DECLARE_PROPERTY(TypedArray<MeshInstance3D>, character_nodes);
	DECLARE_PROPERTY(TypedArray<CardGrid2D>, card_grids_2d);
	DECLARE_PROPERTY(TypedArray<CardGrid3D>, card_grids_3d);

	HashMap<enums::VariableDef::Variable, Ref<JigsawParameter>> _variables;

	~JigsawGlobal();

	void init_sides();
	Ref<JigsawError> run_variant_triggers(JigsawTriggerVariant::Type type, const TypedArray<JigsawParameter> &args, const Ref<RNG> &rng, bool copy_rng);

	DEFAULT_TO_STRING();
};

#endif // JIGSAW_GLOBAL_H
