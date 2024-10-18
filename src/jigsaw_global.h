#ifndef JIGSAW_GLOBAL_H
#define JIGSAW_GLOBAL_H

#include "dry.h"

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/sprite3d.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>

class JigsawGlobal;

#include "audience.h"
#include "card_grid_native.h"
#include "card_instance.h"
#include "effect_instance.h"
#include "game_mode.h"
#include "jigsaw_context.h"
#include "jigsaw_side.h"
#include "rng.h"
#include "squish_label.h"
#include "variant_def.h"

class JigsawGlobal : public Node {
	GDCLASS(JigsawGlobal, Node);

protected:
	static void _bind_methods();

public:
	JigsawGlobal() = default;
	~JigsawGlobal() = default;

	DECLARE_PROPERTY(Ref<GameMode>, mode);
	DECLARE_PROPERTY(Ref<VariantDef>, selected_variant);
	DECLARE_PROPERTY(Ref<CardInstance>, current_card_instance);
	DECLARE_PROPERTY(Ref<EffectInstance>, current_effect_instance);
	DECLARE_PROPERTY(TypedArray<JigsawSide>, sides);

	DECLARE_PROPERTY(double, time_scale, = 1.0);
	DECLARE_PROPERTY(TypedArray<JigsawContext>, context_stack);
	DECLARE_PROPERTY(Ref<RNG>, rng);

	DECLARE_PROPERTY(TypedArray<Node3D>, scene_nodes);
	DECLARE_PROPERTY(TypedArray<Sprite3D>, sprite_nodes);
	DECLARE_PROPERTY(TypedArray<SquishLabel>, label_nodes);
	DECLARE_PROPERTY(TypedArray<TextureRect>, icon_nodes);
	DECLARE_PROPERTY(Ref<Audience>, audience);
	DECLARE_PROPERTY(TypedArray<MeshInstance3D>, character_nodes);
	DECLARE_PROPERTY(TypedArray<CardGridNative>, card_grids);

	void init_sides();
};

#endif // JIGSAW_GLOBAL_H
