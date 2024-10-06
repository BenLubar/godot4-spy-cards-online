#ifndef JIGSAW_GLOBAL_H
#define JIGSAW_GLOBAL_H

#include "dry.h"

class JigsawGlobal;

#include "game_mode.h"
#include "card_instance.h"
#include "effect_instance.h"

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/sprite3d.hpp>

class JigsawGlobal : public Node {
	GDCLASS(JigsawGlobal, Node);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(Ref<GameMode>, mode);
	DECLARE_PROPERTY(Ref<CardInstance>, current_card_instance);
	DECLARE_PROPERTY(Ref<EffectInstance>, current_effect_instance);
	DECLARE_PROPERTY(TypedArray<Node3D>, scene_nodes);
	DECLARE_PROPERTY(TypedArray<Sprite3D>, sprite_nodes);
};

#endif // JIGSAW_GLOBAL_H
