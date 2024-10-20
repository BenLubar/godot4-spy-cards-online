#include "variable_def.h"

void VariableDef::_bind_methods() {
	using namespace enums::VariableDef;

	BIND_ENUM_CONSTANT(NONE);

	BIND_ENUM_CONSTANT(TARGET_CARD);
	BIND_ENUM_CONSTANT(LIMIT_COUNT);

	BIND_ENUM_CONSTANT(MAX_HP);
	BIND_ENUM_CONSTANT(HAND_MIN_SIZE);
	BIND_ENUM_CONSTANT(HAND_MAX_SIZE);
	BIND_ENUM_CONSTANT(DRAW_PER_ROUND);
	BIND_ENUM_CONSTANT(CARDS_PER_DECK);
	BIND_ENUM_CONSTANT(MIN_TP);
	BIND_ENUM_CONSTANT(MAX_TP);
	BIND_ENUM_CONSTANT(TP_PER_ROUND);
	BIND_ENUM_CONSTANT(BOSS_CARDS);
	BIND_ENUM_CONSTANT(MINI_BOSS_CARDS);
	BIND_ENUM_CONSTANT(DUPLICATE_BOSS);
	BIND_ENUM_CONSTANT(DUPLICATE_MINI_BOSS);
	BIND_ENUM_CONSTANT(DUPLICATE_EFFECT);
	BIND_ENUM_CONSTANT(DUPLICATE_ATTACKER);
	BIND_ENUM_CONSTANT(CARDS_PER_ROUND);
	BIND_ENUM_CONSTANT(FIRST_LEGACY_GAME_RULE);

	BIND_ENUM_CONSTANT(FIRST_CUSTOM);

	BIND_ENUM_CONSTANT(CARD_EFFECT_INSTANCE);
	BIND_ENUM_CONSTANT(EFFECT_QUEUE_TREE);
	BIND_ENUM_CONSTANT(SIDE);

	BIND_PROPERTY(Variant::STRING, editor_name);
	BIND_PROPERTY_ENUM(VariableDef::Uniqueness, uniqueness);
	BIND_PROPERTY_RESOURCE(JigsawParameter, default_value);
}

IMPLEMENT_PROPERTY(VariableDef, String, editor_name);
IMPLEMENT_PROPERTY(VariableDef, VariableDef::Uniqueness, uniqueness);
IMPLEMENT_PROPERTY(VariableDef, Ref<JigsawParameter>, default_value);
