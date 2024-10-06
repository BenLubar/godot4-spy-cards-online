#include "jigsaw_global.h"

void JigsawGlobal::_bind_methods() {
	BIND_PROPERTY(Variant::OBJECT, mode, PROPERTY_HINT_RESOURCE_TYPE, "GameMode");
	BIND_PROPERTY(Variant::OBJECT, current_card_instance);
	BIND_PROPERTY(Variant::OBJECT, current_effect_instance, PROPERTY_HINT_RESOURCE_TYPE, "EffectInstance");
	BIND_PROPERTY(Variant::ARRAY, scene_nodes);
	BIND_PROPERTY(Variant::ARRAY, sprite_nodes);
}

IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, Ref<GameMode>, mode);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, Ref<CardInstance>, current_card_instance);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, Ref<EffectInstance>, current_effect_instance);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, TypedArray<Node3D>, scene_nodes);
IMPLEMENT_PROPERTY_SIMPLE(JigsawGlobal, TypedArray<Sprite3D>, sprite_nodes);
