#ifndef EFFECT_DEF_H
#define EFFECT_DEF_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class EffectDef : public Resource {
	GDCLASS(EffectDef, Resource);

public:
	enum Effect {
		NONE = -1,
		FLAVOR_TEXT = 0,
		STAT = 1,
		EMPOWER = 2,
		SUMMON = 3,
		HEAL = 4,
		TP = 5,
		NUMB = 6,
		RAW_STAT = 7,
		MULTIPLY_HEALING = 8,
		PREVENT_NUMB = 9,
		MODIFY_AVAILABLE_CARDS_ADD = 10,
		MODIFY_CARD_COST = 11,
		DRAW_CARD = 12,
		MODIFY_GAME_RULE = 13,
		DELAY_SETUP = 14,
		MODIFY_AVAILABLE_CARDS_REMOVE = 15,
		MODIFY_AVAILABLE_CARDS_MOVE = 16,
		MODIFY_AVAILABLE_CARDS_REPLACE = 17,
		MODIFY_AVAILABLE_CARDS_ADD_HAND = 18,
		MODIFY_AVAILABLE_CARDS_REMOVE_HAND = 19,
		MODIFY_AVAILABLE_CARDS_MOVE_HAND = 20,
		MODIFY_AVAILABLE_CARDS_REPLACE_HAND = 21,
		MODIFY_AVAILABLE_CARDS_ADD_DECK = 22,
		MODIFY_AVAILABLE_CARDS_REMOVE_DECK = 23,
		MODIFY_AVAILABLE_CARDS_MOVE_DECK = 24,
		MODIFY_AVAILABLE_CARDS_REPLACE_DECK = 25,
		MODIFY_AVAILABLE_CARDS_ADD_FIELD = 26,
		MODIFY_AVAILABLE_CARDS_REMOVE_FIELD = 27,
		MODIFY_AVAILABLE_CARDS_REPLACE_FIELD = 28,
        
		LEGACY_SET_MUSIC = 64,
		LEGACY_PLAY_SOUND = 65,
		SET_STAGE = 66,
		SET_MUSIC = 67,
		PLAY_SOUND = 68,
        
		COND_CARD = 128,
		COND_LIMIT = 129,
		COND_WINNER = 130,
		COND_APPLY = 131,
		COND_COIN = 132,
		COND_STAT = 133,
		COND_IN_HAND = 134,
		COND_LAST_EFFECT = 135,
		COND_ON_NUMB = 136,
		COND_MULTIPLE_EFFECTS = 137,
		COND_ON_DISCARD = 138,
		COND_COMPARE_TARGET_CARD = 139,
		COND_ON_EXILE = 140,
		COND_TIE = 141,
		COND_STAT_MATH = 142,
        
		FIRST_CUSTOM = 256,
	};

protected:
	static void _bind_methods();

private:

public:
	EffectDef();
	~EffectDef();
};

#endif // EFFECT_DEF_H
