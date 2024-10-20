#include "card_def.h"

#include "effect_instance.h"
#include "stat_value.h"

void CardDef::_bind_methods() {
	using namespace enums::CardDef;

	BIND_ENUM_CONSTANT(NONE);

	BIND_ENUM_CONSTANT(ZOMBIANT);
	BIND_ENUM_CONSTANT(JELLYSHROOM);
	BIND_ENUM_CONSTANT(SPIDER);
	BIND_ENUM_CONSTANT(ZASP);
	BIND_ENUM_CONSTANT(CACTILING);
	BIND_ENUM_CONSTANT(PSICORP);
	BIND_ENUM_CONSTANT(THIEF);
	BIND_ENUM_CONSTANT(BANDIT);
	BIND_ENUM_CONSTANT(INICHAS);
	BIND_ENUM_CONSTANT(SEEDLING);
	BIND_ENUM_CONSTANT(NUMBNAIL);
	BIND_ENUM_CONSTANT(MOTHIVA);
	BIND_ENUM_CONSTANT(ACORNLING);
	BIND_ENUM_CONSTANT(WEEVIL);
	BIND_ENUM_CONSTANT(VENUS_BUD);
	BIND_ENUM_CONSTANT(CHOMPER);
	BIND_ENUM_CONSTANT(ACOLYTE_ARIA);
	BIND_ENUM_CONSTANT(KABBU);
	BIND_ENUM_CONSTANT(VENUS_GUARDIAN);
	BIND_ENUM_CONSTANT(WASP_TROOPER);
	BIND_ENUM_CONSTANT(WASP_BOMBER);
	BIND_ENUM_CONSTANT(WASP_DRILLER);
	BIND_ENUM_CONSTANT(WASP_SCOUT);
	BIND_ENUM_CONSTANT(MIDGE);
	BIND_ENUM_CONSTANT(UNDERLING);
	BIND_ENUM_CONSTANT(MONSIEUR_SCARLET);
	BIND_ENUM_CONSTANT(GOLDEN_SEEDLING);
	BIND_ENUM_CONSTANT(ARROW_WORM);
	BIND_ENUM_CONSTANT(CARMINA);
	BIND_ENUM_CONSTANT(SEEDLING_KING);
	BIND_ENUM_CONSTANT(BROODMOTHER);
	BIND_ENUM_CONSTANT(PLUMPLING);
	BIND_ENUM_CONSTANT(FLOWERLING);
	BIND_ENUM_CONSTANT(BURGLAR);
	BIND_ENUM_CONSTANT(ASTOTHELES);
	BIND_ENUM_CONSTANT(MOTHER_CHOMPER);
	BIND_ENUM_CONSTANT(AHONEYNATION);
	BIND_ENUM_CONSTANT(BEE_BOOP);
	BIND_ENUM_CONSTANT(SECURITY_TURRET);
	BIND_ENUM_CONSTANT(DENMUKI);
	BIND_ENUM_CONSTANT(HEAVY_DRONE_B33);
	BIND_ENUM_CONSTANT(MENDER);
	BIND_ENUM_CONSTANT(ABOMIHONEY);
	BIND_ENUM_CONSTANT(DUNE_SCORPION);
	BIND_ENUM_CONSTANT(TIDAL_WYRM);
	BIND_ENUM_CONSTANT(KALI);
	BIND_ENUM_CONSTANT(ZOMBEE);
	BIND_ENUM_CONSTANT(ZOMBEETLE);
	BIND_ENUM_CONSTANT(THE_WATCHER);
	BIND_ENUM_CONSTANT(PEACOCK_SPIDER);
	BIND_ENUM_CONSTANT(BLOATSHROOM);
	BIND_ENUM_CONSTANT(KRAWLER);
	BIND_ENUM_CONSTANT(HAUNTED_CLOTH);
	BIND_ENUM_CONSTANT(WARDEN);
	BIND_ENUM_CONSTANT(JUMPING_SPIDER);
	BIND_ENUM_CONSTANT(MIMIC_SPIDER);
	BIND_ENUM_CONSTANT(LEAFBUG_NINJA);
	BIND_ENUM_CONSTANT(LEAFBUG_ARCHER);
	BIND_ENUM_CONSTANT(LEAFBUG_CLUBBER);
	BIND_ENUM_CONSTANT(MADESPHY);
	BIND_ENUM_CONSTANT(THE_BEAST);
	BIND_ENUM_CONSTANT(CHOMPER_BRUTE);
	BIND_ENUM_CONSTANT(MANTIDFLY);
	BIND_ENUM_CONSTANT(GENERAL_ULTIMAX);
	BIND_ENUM_CONSTANT(WILD_CHOMPER);
	BIND_ENUM_CONSTANT(CROSS);
	BIND_ENUM_CONSTANT(POI);
	BIND_ENUM_CONSTANT(PRIMAL_WEEVIL);
	BIND_ENUM_CONSTANT(FALSE_MONARCH);
	BIND_ENUM_CONSTANT(MOTHFLY);
	BIND_ENUM_CONSTANT(MOTHFLY_CLUSTER);
	BIND_ENUM_CONSTANT(IRONNAIL);
	BIND_ENUM_CONSTANT(BELOSTOSS);
	BIND_ENUM_CONSTANT(RUFFIAN);
	BIND_ENUM_CONSTANT(WATER_STRIDER);
	BIND_ENUM_CONSTANT(DIVING_SPIDER);
	BIND_ENUM_CONSTANT(CENN);
	BIND_ENUM_CONSTANT(PISCI);
	BIND_ENUM_CONSTANT(DEAD_LANDER_ALPHA);
	BIND_ENUM_CONSTANT(DEAD_LANDER_BETA);
	BIND_ENUM_CONSTANT(DEAD_LANDER_GAMMA);
	BIND_ENUM_CONSTANT(WASP_KING);
	BIND_ENUM_CONSTANT(THE_EVERLASTING_KING);
	BIND_ENUM_CONSTANT(MAKI);
	BIND_ENUM_CONSTANT(KINA);
	BIND_ENUM_CONSTANT(YIN);
	BIND_ENUM_CONSTANT(ULTIMAX_TANK);
	BIND_ENUM_CONSTANT(ZOMMOTH);
	BIND_ENUM_CONSTANT(RIZ);
	BIND_ENUM_CONSTANT(DEVOURER);
	BIND_ENUM_CONSTANT(STRATOS);
	BIND_ENUM_CONSTANT(DELILAH);

	BIND_ENUM_CONSTANT(FIRST_CUSTOM);

	BIND_PROPERTY_ENUM(enums::CardDef::Card, id);
	BIND_PROPERTY_ENUM(enums::RankDef::Rank, rank);
	BIND_PROPERTY_RESOURCE_ARRAY(StatValue, costs);
	BIND_PROPERTY(Variant::STRING, name);
	BIND_PROPERTY_ENUM(enums::IconDef::Icon, portrait);
	BIND_PROPERTY_ENUM_ARRAY(enums::TribeDef::Tribe, tribes);
	BIND_PROPERTY_RESOURCE_ARRAY(EffectInstance, effects);
	BIND_PROPERTY(Variant::FLOAT, curve_adjust);
}

IMPLEMENT_PROPERTY(CardDef, enums::CardDef::Card, id);
IMPLEMENT_PROPERTY(CardDef, enums::RankDef::Rank, rank);
IMPLEMENT_PROPERTY(CardDef, TypedArray<StatValue>, costs);
IMPLEMENT_PROPERTY(CardDef, String, name);
IMPLEMENT_PROPERTY(CardDef, enums::IconDef::Icon, portrait);
IMPLEMENT_PROPERTY(CardDef, TypedArray<enums::TribeDef::Tribe>, tribes);
IMPLEMENT_PROPERTY(CardDef, TypedArray<EffectInstance>, effects);
IMPLEMENT_PROPERTY(CardDef, double, curve_adjust);
