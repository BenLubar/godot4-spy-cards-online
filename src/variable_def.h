#ifndef VARIABLE_DEF_H
#define VARIABLE_DEF_H

#include "dry.h"

class VariableDef;

#include "jigsaw_parameter.h"

class VariableDef : public Resource {
	GDCLASS(VariableDef, Resource);

public:
	enum Variable {
		NONE = -1,
		TARGET_CARD = 0,
		LIMIT_COUNT = 1,
        
		MAX_HP = 241,
		HAND_MIN_SIZE = 242,
		HAND_MAX_SIZE = 243,
		DRAW_PER_ROUND = 244,
		CARDS_PER_DECK = 245,
		MIN_TP = 246,
		MAX_TP = 247,
		TP_PER_ROUND = 248,
		BOSS_CARDS = 249,
		MINI_BOSS_CARDS = 250,
		DUPLICATE_BOSS = 251,
		DUPLICATE_MINI_BOSS = 252,
		DUPLICATE_EFFECT = 253,
		DUPLICATE_ATTACKER = 254,
		CARDS_PER_ROUND = 255,
		FIRST_LEGACY_GAME_RULE = MAX_HP,
        
		FIRST_CUSTOM = 256,
	};
	enum Uniqueness {
		CARD_EFFECT_INSTANCE = 0,
		EFFECT_QUEUE_TREE = 1,
		SIDE = 2,
	};

protected:
	static void _bind_methods();

public:
	VariableDef() = default;
	~VariableDef() = default;

	DECLARE_PROPERTY(String, editor_name);
	DECLARE_PROPERTY(Uniqueness, uniqueness, = Uniqueness::CARD_EFFECT_INSTANCE);
	DECLARE_PROPERTY(Ref<JigsawParameter>, default_value);
};
DECLARE_ENUM(VariableDef::Variable);
DECLARE_ENUM(VariableDef::Uniqueness);

#endif // VARIABLE_DEF_H
