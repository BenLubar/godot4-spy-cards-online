#include "jigsaw_global.h"

void JigsawGlobal::_bind_methods() {
	BIND_PROPERTY_RESOURCE(GameMode, mode);
	BIND_PROPERTY_RESOURCE(CardInstance, current_card_instance);
	BIND_PROPERTY_RESOURCE(EffectInstance, current_effect_instance);

	BIND_PROPERTY_RESOURCE_ARRAY(JigsawContext, context_stack);

	BIND_PROPERTY_RESOURCE_ARRAY(Node3D, scene_nodes);
	BIND_PROPERTY_RESOURCE_ARRAY(Sprite3D, sprite_nodes);
	BIND_PROPERTY_RESOURCE_ARRAY(Label, label_nodes);
	BIND_PROPERTY_RESOURCE_ARRAY(TextureRect, icon_nodes);
	BIND_PROPERTY_RESOURCE(Audience, audience);
	BIND_PROPERTY_RESOURCE_ARRAY(MeshInstance3D, character_nodes);
}

IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, Ref<GameMode>, mode);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, Ref<CardInstance>, current_card_instance);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, Ref<EffectInstance>, current_effect_instance);

IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, TypedArray<JigsawContext>, context_stack);

IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, TypedArray<Node3D>, scene_nodes);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, TypedArray<Sprite3D>, sprite_nodes);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, TypedArray<Label>, label_nodes);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, TypedArray<TextureRect>, icon_nodes);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, Ref<Audience>, audience);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, TypedArray<MeshInstance3D>, character_nodes);
