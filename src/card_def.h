#ifndef CARD_DEF_H
#define CARD_DEF_H

#include "dry.h"

#include "rank_def.h"
#include "tribe_def.h"
#include "stat_value.h"
#include "effect_instance.h"

class CardDef : public Resource {
	GDCLASS(CardDef, Resource);

public:
	enum Card {
		NONE                 = -1,
		ZOMBIANT             = 0,
		JELLYSHROOM          = 1,
		SPIDER               = 2,
		ZASP                 = 3,
		CACTILING            = 4,
		PSICORP              = 5,
		THIEF                = 6,
		BANDIT               = 7,
		INICHAS              = 8,
		SEEDLING             = 9,
		NUMBNAIL             = 14,
		MOTHIVA              = 15,
		ACORNLING            = 16,
		WEEVIL               = 17,
		VENUS_BUD            = 19,
		CHOMPER              = 20,
		ACOLYTE_ARIA         = 21,
		KABBU                = 23,
		VENUS_GUARDIAN       = 24,
		WASP_TROOPER         = 25,
		WASP_BOMBER          = 26,
		WASP_DRILLER         = 27,
		WASP_SCOUT           = 28,
		MIDGE                = 29,
		UNDERLING            = 30,
		MONSIEUR_SCARLET     = 31,
		GOLDEN_SEEDLING      = 32,
		ARROW_WORM           = 33,
		CARMINA              = 34,
		SEEDLING_KING        = 35,
		BROODMOTHER          = 36,
		PLUMPLING            = 37,
		FLOWERLING           = 38,
		BURGLAR              = 39,
		ASTOTHELES           = 40,
		MOTHER_CHOMPER       = 41,
		AHONEYNATION         = 42,
		BEE_BOOP             = 43,
		SECURITY_TURRET      = 44,
		DENMUKI              = 45,
		HEAVY_DRONE_B33      = 46,
		MENDER               = 47,
		ABOMIHONEY           = 48,
		DUNE_SCORPION        = 49,
		TIDAL_WYRM           = 50,
		KALI                 = 51,
		ZOMBEE               = 52,
		ZOMBEETLE            = 53,
		THE_WATCHER          = 54,
		PEACOCK_SPIDER       = 55,
		BLOATSHROOM          = 56,
		KRAWLER              = 57,
		HAUNTED_CLOTH        = 58,
		WARDEN               = 61,
		JUMPING_SPIDER       = 63,
		MIMIC_SPIDER         = 64,
		LEAFBUG_NINJA        = 65,
		LEAFBUG_ARCHER       = 66,
		LEAFBUG_CLUBBER      = 67,
		MADESPHY             = 68,
		THE_BEAST            = 69,
		CHOMPER_BRUTE        = 70,
		MANTIDFLY            = 71,
		GENERAL_ULTIMAX      = 72,
		WILD_CHOMPER         = 73,
		CROSS                = 74,
		POI                  = 75,
		PRIMAL_WEEVIL        = 76,
		FALSE_MONARCH        = 77,
		MOTHFLY              = 78,
		MOTHFLY_CLUSTER      = 79,
		IRONNAIL             = 80,
		BELOSTOSS            = 81,
		RUFFIAN              = 82,
		WATER_STRIDER        = 83,
		DIVING_SPIDER        = 84,
		CENN                 = 85,
		PISCI                = 86,
		DEAD_LANDER_Α        = 87,
		DEAD_LANDER_Β        = 88,
		DEAD_LANDER_Γ        = 89,
		WASP_KING            = 90,
		THE_EVERLASTING_KING = 91,
		MAKI                 = 92,
		KINA                 = 93,
		YIN                  = 94,
		ULTIMAX_TANK         = 95,
		ZOMMOTH              = 96,
		RIZ                  = 97,
		DEVOURER             = 98,
		STRATOS              = 111,
		DELILAH              = 112,

		FIRST_CUSTOM         = 128,
	};

protected:
	static void _bind_methods();

public:
	CardDef() = default;
	~CardDef() = default;

	DECLARE_PROPERTY(Card, id, = Card::NONE);
	DECLARE_PROPERTY(RankDef::Rank, rank, = RankDef::Rank::NONE);
	DECLARE_PROPERTY(TypedArray<StatValue>, costs);
	DECLARE_PROPERTY(String, name);
	DECLARE_PROPERTY(IconDef::Icon, portrait, = IconDef::Icon::NONE);
	DECLARE_PROPERTY(TypedArray<TribeDef::Tribe>, tribes);
	DECLARE_PROPERTY(TypedArray<EffectInstance>, effects);
	DECLARE_PROPERTY(double, curve_adjust, = 0.0);
};
DECLARE_ENUM(CardDef::Card);

#endif // CARD_DEF_H
