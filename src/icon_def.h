#ifndef ICON_DEF_H
#define ICON_DEF_H

#include "dry.h"

#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/image_texture.hpp>

class IconDef : public Resource {
	GDCLASS(IconDef, Resource);

public:
	enum Icon {
		NONE = -1,

		PORTRAIT_ZOMBIANT = 0,
		PORTRAIT_JELLYSHROOM = 1,
		PORTRAIT_SPIDER = 2,
		PORTRAIT_ZASP = 3,
		PORTRAIT_CACTILING = 4,
		PORTRAIT_PSICORP = 5,
		PORTRAIT_THIEF = 6,
		PORTRAIT_BANDIT = 7,
		PORTRAIT_INICHAS = 8,
		PORTRAIT_SEEDLING = 9,
		PORTRAIT_BLANK = 10,
		PORTRAIT_ACORNLING = 11,
		PORTRAIT_WEEVIL = 12,
		PORTRAIT_VENUS_BUD = 13,
		PORTRAIT_NUMBNAIL = 14,
		PORTRAIT_MOTHIVA = 15,
		PORTRAIT_CHOMPER = 16,
		PORTRAIT_KABBU = 17,
		PORTRAIT_ACOLYTE_ARIA = 18,
		PORTRAIT_WASP_TROOPER = 19,
		PORTRAIT_MIDGE = 20,
		PORTRAIT_VENUS_GUARDIAN = 21,
		PORTRAIT_UNDERLING = 22,
		PORTRAIT_MONSIEUR_SCARLET = 23,
		PORTRAIT_GOLDEN_SEEDLING = 24,
		PORTRAIT_MOTHER_CHOMPER = 25,
		PORTRAIT_BURGLAR = 26,
		PORTRAIT_BEE_BOOP = 27,
		PORTRAIT_ABOMIHONEY = 28,
		PORTRAIT_CARMINA = 29,
		PORTRAIT_SECURITY_TURRET = 30,
		PORTRAIT_DENMUKI = 31,
		PORTRAIT_HEAVY_DRONE_B33 = 32,
		PORTRAIT_WASP_SCOUT = 33,
		PORTRAIT_ARROW_WORM = 34,
		PORTRAIT_AHONEYNATION = 35,
		PORTRAIT_MENDER = 36,
		PORTRAIT_DUNE_SCORPION = 37,
		PORTRAIT_ASTOTHELES = 38,
		PORTRAIT_KALI = 39,
		PORTRAIT_KRAWLER = 40,
		PORTRAIT_HAUNTED_CLOTH = 41,
		PORTRAIT_WARDEN = 42,
		PORTRAIT_THE_WATCHER = 43,
		PORTRAIT_SEEDLING_KING = 44,
		PORTRAIT_FLOWERLING = 45,
		PORTRAIT_PLUMPLING = 46,
		PORTRAIT_JUMPING_SPIDER = 47,
		PORTRAIT_LEAFBUG_NINJA = 48,
		PORTRAIT_LEAFBUG_ARCHER = 49,
		PORTRAIT_LEAFBUG_CLUBBER = 50,
		PORTRAIT_MADESPHY = 51,
		PORTRAIT_MIMIC_SPIDER = 52,
		PORTRAIT_THE_BEAST = 53,
		PORTRAIT_CHOMPER_BRUTE = 54,
		PORTRAIT_WASP_DRILLER = 55,
		PORTRAIT_GENERAL_ULTIMAX = 56,
		PORTRAIT_WASP_BOMBER = 57,
		PORTRAIT_WILD_CHOMPER = 58,
		PORTRAIT_ZOMBEE = 59,
		PORTRAIT_ZOMBEETLE = 60,
		PORTRAIT_BLOATSHROOM = 61,
		PORTRAIT_PRIMAL_WEEVIL = 62,
		PORTRAIT_CROSS = 63,
		PORTRAIT_POI = 64,
		PORTRAIT_MANTIDFLY = 65,
		PORTRAIT_FALSE_MONARCH = 66,
		PORTRAIT_MOTHFLY = 67,
		PORTRAIT_MOTHFLY_CLUSTER = 68,
		PORTRAIT_BROODMOTHER = 69,
		PORTRAIT_MAKI = 70,
		PORTRAIT_KINA = 71,
		PORTRAIT_WASP_KING = 72,
		PORTRAIT_IRONNAIL = 73,
		PORTRAIT_BELOSTOSS = 74,
		PORTRAIT_WATER_STRIDER = 75,
		PORTRAIT_DIVING_SPIDER = 76,
		PORTRAIT_CENN = 77,
		PORTRAIT_PISCI = 78,
		PORTRAIT_RUFFIAN = 79,
		PORTRAIT_INNKEEPER = 80,
		PORTRAIT_TOD = 81,
		PORTRAIT_LEBY = 82,
		PORTRAIT_YATANTA = 83,
		PORTRAIT_CHUCK = 84,
		PORTRAIT_LAYNA = 85,
		PORTRAIT_MM = 86,
		PORTRAIT_LEVI = 87,
		PORTRAIT_TANJERIN = 88,
		PORTRAIT_PROFESSOR_NEOLITH = 89,
		PORTRAIT_ISAU = 90,
		PORTRAIT_GS_TECHNICIAN = 91,
		PORTRAIT_CRISBEE = 92,
		PORTRAIT_KUT = 93,
		PORTRAIT_FRY = 94,
		PORTRAIT_LEIF = 95,
		PORTRAIT_BOOK = 96,
		PORTRAIT_PROFESSOR_HONEYCOMB = 97,
		PORTRAIT_DOCTOR_HB = 98,
		PORTRAIT_ZASP_DISGUISED = 99,
		PORTRAIT_ACOLYTE_ARIA_QUEST = 100,
		PORTRAIT_MADELEINE = 101,
		PORTRAIT_VI = 102,
		PORTRAIT_ALI = 103,
		PORTRAIT_BEE_KINGDOM = 104,
		PORTRAIT_VIVI = 105,
		PORTRAIT_MALBEE = 106,
		PORTRAIT_SPY_CARDS = 107,
		PORTRAIT_TAKKUN = 108,
		PORTRAIT_BOMBY = 109,
		PORTRAIT_BUTOMU = 110,
		PORTRAIT_MAYOR = 111,
		PORTRAIT_EETL = 112,
		PORTRAIT_VEN = 113,
		PORTRAIT_GEN_AND_ERI = 114,
		PORTRAIT_ARTIA = 115,
		PORTRAIT_MUN = 116,
		PORTRAIT_ALEX = 117,
		PORTRAIT_MAKI_AND_KINA = 118,
		PORTRAIT_FALSE_MONARCH_BOUNTY = 119,
		PORTRAIT_SEEDLING_KING_BOUNTY = 120,
		PORTRAIT_FABRI = 121,
		PORTRAIT_TIDAL_WYRM = 122,
		PORTRAIT_TIDAL_WYRM_BOUNTY = 123,
		PORTRAIT_RIZ = 124,
		PORTRAIT_RIZZA = 125,
		PORTRAIT_ZOMMOTH = 126,
		PORTRAIT_PEACOCK_SPIDER = 127,
		PORTRAIT_UNKNOWN = 128,
		PORTRAIT_SNAKEMOUTH_DEN = 129,
		PORTRAIT_SNAKEMOUTH_DEPTHS = 130,
		PORTRAIT_EXPLORERS_MESSAGE = 131,
		PORTRAIT_THE_ROACHES_VILLAGE = 132,
		PORTRAIT_INN_FOREMOTHERS = 133,
		PORTRAIT_THE_SETTLER_STATUE = 134,
		PORTRAIT_THE_ANTS = 135,
		PORTRAIT_THE_BEES = 136,
		PORTRAIT_THE_TERMITES = 137,
		PORTRAIT_THE_WASPS = 138,
		PORTRAIT_QUEEN_ELIZANT_I = 139,
		PORTRAIT_THE_ANTHILL_PALACE = 140,
		PORTRAIT_APHIDS_AND_COCHNEALS = 141,
		PORTRAIT_THE_GOLDEN_FESTIVAL = 142,
		PORTRAIT_THE_GODDESS_STATUE = 143,
		PORTRAIT_PEACOCK_SPIDER_BOUNTY = 144,
		PORTRAIT_THE_WIZARD = 145,
		PORTRAIT_ULTIMAX_TANK = 146,
		PORTRAIT_ELOM = 147,
		PORTRAIT_SAND_CASTLE_MURAL = 148,
		PORTRAIT_EREMI = 149,
		PORTRAIT_VI_DEED = 150,
		PORTRAIT_LEIF_DEED = 151,
		PORTRAIT_MASK = 152,
		PORTRAIT_TABLET = 153,
		PORTRAIT_KEY_1 = 154,
		PORTRAIT_KEY_2 = 155,
		PORTRAIT_QUEEN_ELIZANT_II = 156,
		PORTRAIT_BROACH = 157,
		PORTRAIT_WASP_KING_CROWN = 158,
		PORTRAIT_GREEN_BUG_RANGER = 159,
		PORTRAIT_DEAD_LANDER_ALPHA = 160,
		PORTRAIT_DEAD_LANDER_BETA = 161,
		PORTRAIT_DEAD_LANDER_GAMMA = 162,
		PORTRAIT_COMPLETIONIST = 163,
		PORTRAIT_QUESTS = 164,
		PORTRAIT_DISCOVERIES = 165,
		PORTRAIT_MEDALS = 166,
		PORTRAIT_CHOMPY = 167,
		PORTRAIT_RANK = 168,
		PORTRAIT_MUSIC = 169,
		PORTRAIT_CRYSTAL = 170,
		PORTRAIT_HIGHSCORES = 171,
		PORTRAIT_RECIPES = 172,
		PORTRAIT_BESTIARY = 173,
		PORTRAIT_THE_EVERLASTING_KING = 174,
		PORTRAIT_THE_EVERLASTING_KING_HARD = 175,
		PORTRAIT_SPIDER_HARD = 176,
		PORTRAIT_VENUS_GUARDIAN_HARD = 177,
		PORTRAIT_HEAVY_DRONE_B33_HARD = 178,
		PORTRAIT_THE_WATCHER_HARD = 179,
		PORTRAIT_THE_BEAST_HARD = 180,
		PORTRAIT_ULTIMAX_TANK_HARD = 181,
		PORTRAIT_BOUNTIES = 182,
		PORTRAIT_KABBU_DEED = 183,
		PORTRAIT_YIN = 184,
		PORTRAIT_DEVOURER = 185,
		PORTRAIT_DEVOURER_BOUNTY = 186,
		PORTRAIT_TARAR = 187,
		PORTRAIT_HONORED_EMPLOYEE = 188,
		PORTRAIT_SNAKEMOUTHS_LAB = 189,
		PORTRAIT_HEALING_SOPHIE = 190,
		PORTRAIT_SEEDLING_HAVEN = 191,
		PORTRAIT_UNDERGROUND_TAVERN = 192,
		PORTRAIT_THE_ANT_MINES = 193,
		PORTRAIT_CHOMPER_CAVERN = 194,
		PORTRAIT_THE_POWER_PLANT = 195,
		PORTRAIT_THE_BEE_KINGDOM = 196,
		PORTRAIT_BALCONY_TELESCOPE = 197,
		PORTRAIT_B_O_S_S = 198,
		PORTRAIT_DEFIANT_ROOT = 199,
		PORTRAIT_THE_LOST_SANDS = 200,
		PORTRAIT_TARDIGRADE_IDOL = 201,
		PORTRAIT_BANDIT_HIDEOUT = 202,
		PORTRAIT_STREAM_MOUNTAIN = 203,
		PORTRAIT_FAR_GRASSLANDS = 204,
		PORTRAIT_WIZARDS_TOWER = 205,
		PORTRAIT_TERMITE_KINGDOM = 206,
		PORTRAIT_TERMITE_COLOSSEUM = 207,
		PORTRAIT_METAL_ISLAND = 208,
		PORTRAIT_RUBBER_PRISON = 209,
		PORTRAIT_BUMBLE = 210,
		PORTRAIT_ENGIRA = 211,
		PORTRAIT_KABBU_QUEST = 212,
		PORTRAIT_REEVES = 213,
		PORTRAIT_RELIC_MUSEUM = 214,
		PORTRAIT_TERMITE_STATUE = 215,
		PORTRAIT_GIANTS_LAIR = 216,
		PORTRAIT_PEACOCK_ISLAND = 217,
		PORTRAIT_WILD_SWAMPLANDS = 218,
		PORTRAIT_WASP_KINGDOM = 219,
		PORTRAIT_FISHING_VILLAGE = 220,
		PORTRAIT_FORSAKEN_LANDS = 221,
		PORTRAIT_SAILORS_STATUE = 222,
		PORTRAIT_ANCIENT_CITY = 223,
		PORTRAIT_TERMACADE = 224,
		PORTRAIT_HAUNTED_CLOTH_FIRE = 225,
		PORTRAIT_KRAWLER_FIRE = 226,
		PORTRAIT_WARDEN_FIRE = 227,
		PORTRAIT_REBECCA = 228,
		PORTRAIT_ROY = 229,
		PORTRAIT_STRATOS_AND_DELILAH = 230,
		PORTRAIT_STRATOS = 231,
		PORTRAIT_DELILAH = 232,
		//UNUSED = 233,
		//UNUSED = 234,
		//UNUSED = 235,
		//UNUSED = 236,
		//UNUSED = 237,
		//UNUSED = 238,
		//UNUSED = 239,
		//UNUSED = 240,
		//UNUSED = 241,
		//UNUSED = 242,
		//UNUSED = 243,
		//UNUSED = 244,
		//UNUSED = 245,
		//UNUSED = 246,
		PORTRAIT_DIGIT_1 = 247,
		PORTRAIT_DIGIT_2 = 248,
		PORTRAIT_DIGIT_3 = 249,
		PORTRAIT_DIGIT_4 = 250,
		PORTRAIT_DIGIT_5 = 251,
		PORTRAIT_DIGIT_6 = 252,
		PORTRAIT_DIGIT_7 = 253,
		LEGACY_PORTRAIT_EMBEDDED = 254,
		LEGACY_PORTRAIT_EXTERNAL = 255,

		STAT_ATK = 256,
		STAT_DEF = 257,
		STAT_HP = 258,
		STAT_TP = 259,
		STAT_TP_INF = 260,
		STAT_BERRIES = 261,
		STAT_CRYSTAL = 262,
		STAT_EXP = 263,
		STAT_MP = 264,

		STATUS_FREEZE = 265,
		STATUS_POISON = 266,
		STATUS_NUMB = 267,
		STATUS_SLEEP = 268,
		STATUS_UNSTEADY = 269,
		STATUS_FLIPPED = 270,
		STATUS_DEF_UP = 271,
		STATUS_DEF_DOWN = 272,
		STATUS_ATK_UP = 273,
		STATUS_ATK_DOWN = 274,
		STATUS_TAUNT = 275,
		STATUS_DANGER = 276,
		STATUS_TP_REGEN = 277,
		STATUS_HP_REGEN = 278,
		STATUS_MOTHFLY = 279,
		STATUS_CHARGE = 280,
		STATUS_BURN = 281,
		STATUS_INK = 282,
		STATUS_STICKY = 283,
		STATUS_TALK = 284,
		STATUS_THINK = 285,
		STATUS_JANET = 286,

		BACK_ATTACKER = 287,
		BACK_EFFECT = 288,
		BACK_MINI_BOSS = 289,
		BACK_BOSS = 290,
		BACK_TOKEN = 291,

		FRONT_ATTACKER = 292,
		FRONT_EFFECT = 293,
		FRONT_MINI_BOSS = 294,
		FRONT_BOSS = 295,
		FRONT_TOKEN = 296,

		COIN = 297,
		COIN_HAPPY = 298,
		COIN_SAD = 299,

		DIGIT_0 = 300,
		DIGIT_1 = 301,
		DIGIT_2 = 302,
		DIGIT_3 = 303,
		DIGIT_4 = 304,
		DIGIT_5 = 305,
		DIGIT_6 = 306,
		DIGIT_7 = 307,
		DIGIT_8 = 308,
		DIGIT_9 = 309,

		CARD_FRONT = 310,
		CARD_FRONT_WINDOW = 311,
		CARD_DESC = 312,
		CARD_TRIBE_BUBBLE = 313,
		CARD_TRIBE_BUBBLE_WIDE = 314,
		WHITE_BAR = 315,

		AUDIENCE_MOTH_FRONT = 316,
		AUDIENCE_MOTH_BACK = 317,
		AUDIENCE_MOTH_FRONT_CHEER = 318,
		AUDIENCE_MOTH_BACK_CHEER = 319,
		AUDIENCE_ANT_FRONT = 320,
		AUDIENCE_ANT_BACK = 321,
		AUDIENCE_ANT_FRONT_CHEER = 322,
		AUDIENCE_ANT_BACK_CHEER = 323,
		AUDIENCE_BEETLE_FRONT = 324,
		AUDIENCE_BEETLE_BACK = 325,
		AUDIENCE_BEETLE_FRONT_CHEER = 326,
		AUDIENCE_BEE_FRONT = 327,
		AUDIENCE_BEE_BACK = 328,
		AUDIENCE_BEE_FRONT_CHEER = 329,
		AUDIENCE_BEE_BACK_CHEER = 330,
		AUDIENCE_TERMITE1_FRONT = 331,
		AUDIENCE_TERMITE1_BACK = 332,
		AUDIENCE_TERMITE1_FRONT_CHEER = 333,
		AUDIENCE_TERMITE1_BACK_CHEER = 334,
		AUDIENCE_TERMITE2_FRONT = 335,
		AUDIENCE_TERMITE2_BACK = 336,
		AUDIENCE_TERMITE2_FRONT_CHEER = 337,
		AUDIENCE_TERMITE2_BACK_CHEER = 338,

		CHARACTER_AMBER_IDLE0 = 339,
		CHARACTER_AMBER_IDLE1 = 340,
		CHARACTER_AMBER_ANGRY = 341,
		CHARACTER_ARIA_IDLE0 = 342,
		CHARACTER_ARIA_IDLE1 = 343,
		CHARACTER_ARIA_ANGRY = 344,
		CHARACTER_ARIE_IDLE0 = 345,
		CHARACTER_ARIE_IDLE1 = 346,
		CHARACTER_ARIE_ANGRY = 347,
		CHARACTER_ASTOTHELES_IDLE0 = 348,
		CHARACTER_ASTOTHELES_IDLE1 = 349,
		CHARACTER_ASTOTHELES_ANGRY = 350,
		CHARACTER_BOMBY_IDLE0 = 351,
		CHARACTER_BOMBY_IDLE1 = 352,
		CHARACTER_BOMBY_ANGRY = 353,
		CHARACTER_BUGI_IDLE0 = 354,
		CHARACTER_BUGI_IDLE1 = 355,
		CHARACTER_BUGI_ANGRY = 356,
		CHARACTER_CARMINA_IDLE0 = 357,
		CHARACTER_CARMINA_IDLE1 = 358,
		CHARACTER_CARMINA_ANGRY = 359,
		CHARACTER_CELIA_IDLE0 = 360,
		CHARACTER_CELIA_IDLE1 = 361,
		CHARACTER_CELIA_ANGRY = 362,
		CHARACTER_CENN_IDLE0 = 363,
		CHARACTER_CENN_IDLE1 = 364,
		CHARACTER_CENN_ANGRY = 365,
		CHARACTER_CERISE_IDLE0 = 366,
		CHARACTER_CERISE_IDLE1 = 367,
		CHARACTER_CERISE_ANGRY = 368,
		CHARACTER_CHOMPY_IDLE0 = 369,
		CHARACTER_CHOMPY_IDLE1 = 370,
		CHARACTER_CHOMPY_ANGRY = 371,
		CHARACTER_CHUBEE_IDLE0 = 372,
		CHARACTER_CHUBEE_IDLE1 = 373,
		CHARACTER_CHUBEE_ANGRY = 374,
		CHARACTER_CHUCK_IDLE0 = 375,
		CHARACTER_CHUCK_IDLE1 = 376,
		CHARACTER_CHUCK_ANGRY = 377,
		CHARACTER_CRISBEE_IDLE0 = 378,
		CHARACTER_CRISBEE_IDLE1 = 379,
		CHARACTER_CRISBEE_ANGRY = 380,
		CHARACTER_CROW_IDLE0 = 381,
		CHARACTER_CROW_IDLE1 = 382,
		CHARACTER_CROW_ANGRY = 383,
		CHARACTER_DIANA_IDLE0 = 384,
		CHARACTER_DIANA_IDLE1 = 385,
		CHARACTER_DIANA_ANGRY = 386,
		CHARACTER_EREMI_IDLE0 = 387,
		CHARACTER_EREMI_IDLE1 = 388,
		CHARACTER_EREMI_ANGRY = 389,
		CHARACTER_FUTES_IDLE0 = 390,
		CHARACTER_FUTES_IDLE1 = 391,
		CHARACTER_FUTES_ANGRY = 392,
		CHARACTER_GENOW_IDLE0 = 393,
		CHARACTER_GENOW_IDLE1 = 394,
		CHARACTER_GENOW_ANGRY = 395,
		CHARACTER_HB_IDLE0 = 396,
		CHARACTER_HB_IDLE1 = 397,
		CHARACTER_HB_ANGRY = 398,
		CHARACTER_HONEYCOMB_IDLE0 = 399,
		CHARACTER_HONEYCOMB_IDLE1 = 400,
		CHARACTER_HONEYCOMB_ANGRY = 401,
		CHARACTER_JANET_IDLE0 = 402,
		CHARACTER_JANET_IDLE1 = 403,
		CHARACTER_JANET_ANGRY = 404,
		CHARACTER_JAUNE_IDLE0 = 405,
		CHARACTER_JAUNE_IDLE1 = 406,
		CHARACTER_JAUNE_ANGRY = 407,
		CHARACTER_JAYDE_IDLE0 = 408,
		CHARACTER_JAYDE_IDLE1 = 409,
		CHARACTER_JAYDE_ANGRY = 410,
		CHARACTER_JOHNNY_IDLE0 = 411,
		CHARACTER_JOHNNY_IDLE1 = 412,
		CHARACTER_JOHNNY_ANGRY = 413,
		CHARACTER_KABBU_IDLE0 = 414,
		CHARACTER_KABBU_IDLE1 = 415,
		CHARACTER_KABBU_ANGRY = 416,
		CHARACTER_KAGE_IDLE0 = 417,
		CHARACTER_KAGE_IDLE1 = 418,
		CHARACTER_KAGE_ANGRY = 419,
		CHARACTER_KALI_IDLE0 = 420,
		CHARACTER_KALI_IDLE1 = 421,
		CHARACTER_KALI_ANGRY = 422,
		CHARACTER_KENNY_IDLE0 = 423,
		CHARACTER_KENNY_IDLE1 = 424,
		CHARACTER_KENNY_ANGRY = 425,
		CHARACTER_KINA_IDLE0 = 426,
		CHARACTER_KINA_IDLE1 = 427,
		CHARACTER_KINA_ANGRY = 428,
		CHARACTER_LAYNA_IDLE0 = 429,
		CHARACTER_LAYNA_IDLE1 = 430,
		CHARACTER_LAYNA_ANGRY = 431,
		CHARACTER_LEIF_IDLE0 = 432,
		CHARACTER_LEIF_IDLE1 = 433,
		CHARACTER_LEIF_ANGRY = 434,
		CHARACTER_LEVI_IDLE0 = 435,
		CHARACTER_LEVI_IDLE1 = 436,
		CHARACTER_LEVI_ANGRY = 437,
		CHARACTER_MAKI_IDLE0 = 438,
		CHARACTER_MAKI_IDLE1 = 439,
		CHARACTER_MAKI_ANGRY = 440,
		CHARACTER_MALBEE_IDLE0 = 441,
		CHARACTER_MALBEE_IDLE1 = 442,
		CHARACTER_MALBEE_ANGRY = 443,
		CHARACTER_MAR_IDLE0 = 444,
		CHARACTER_MAR_IDLE1 = 445,
		CHARACTER_MAR_ANGRY = 446,
		CHARACTER_MOTHIVA_IDLE0 = 447,
		CHARACTER_MOTHIVA_IDLE1 = 448,
		CHARACTER_MOTHIVA_ANGRY = 449,
		CHARACTER_NEOLITH_IDLE0 = 450,
		CHARACTER_NEOLITH_IDLE1 = 451,
		CHARACTER_NEOLITH_ANGRY = 452,
		CHARACTER_NERO_IDLE0 = 453,
		CHARACTER_NERO_IDLE1 = 454,
		CHARACTER_NERO_ANGRY = 455,
		CHARACTER_PIBU_IDLE0 = 456,
		CHARACTER_PIBU_IDLE1 = 457,
		CHARACTER_PIBU_ANGRY = 458,
		CHARACTER_PISCI_IDLE0 = 459,
		CHARACTER_PISCI_IDLE1 = 460,
		CHARACTER_PISCI_ANGRY = 461,
		CHARACTER_RITCHEE_IDLE0 = 462,
		CHARACTER_RITCHEE_IDLE1 = 463,
		CHARACTER_RITCHEE_ANGRY = 464,
		CHARACTER_RIZ_IDLE0 = 465,
		CHARACTER_RIZ_IDLE1 = 466,
		CHARACTER_RIZ_ANGRY = 467,
		CHARACTER_SAMIRA_IDLE0 = 468,
		CHARACTER_SAMIRA_IDLE1 = 469,
		CHARACTER_SAMIRA_ANGRY = 470,
		CHARACTER_SCARLET_IDLE0 = 471,
		CHARACTER_SCARLET_IDLE1 = 472,
		CHARACTER_SCARLET_ANGRY = 473,
		CHARACTER_SERENE_IDLE0 = 474,
		CHARACTER_SERENE_IDLE1 = 475,
		CHARACTER_SERENE_ANGRY = 476,
		CHARACTER_SHAY_IDLE0 = 477,
		CHARACTER_SHAY_IDLE1 = 478,
		CHARACTER_SHAY_ANGRY = 479,
		CHARACTER_SKIRBY_IDLE0 = 480,
		CHARACTER_SKIRBY_IDLE1 = 481,
		CHARACTER_SKIRBY_ANGRY = 482,
		CHARACTER_TANJERIN_IDLE0 = 483,
		CHARACTER_TANJERIN_IDLE1 = 484,
		CHARACTER_TANJERIN_ANGRY = 485,
		CHARACTER_ULTIMAX_IDLE0 = 486,
		CHARACTER_ULTIMAX_IDLE1 = 487,
		CHARACTER_ULTIMAX_ANGRY = 488,
		CHARACTER_VANESSA_IDLE0 = 489,
		CHARACTER_VANESSA_IDLE1 = 490,
		CHARACTER_VANESSA_ANGRY = 491,
		CHARACTER_VI_IDLE0 = 492,
		CHARACTER_VI_IDLE1 = 493,
		CHARACTER_VI_ANGRY = 494,
		CHARACTER_YIN_IDLE0 = 495,
		CHARACTER_YIN_IDLE1 = 496,
		CHARACTER_YIN_ANGRY = 497,
		CHARACTER_ZARYANT_IDLE0 = 498,
		CHARACTER_ZARYANT_IDLE1 = 499,
		CHARACTER_ZARYANT_ANGRY = 500,
		CHARACTER_ZASP_IDLE0 = 501,
		CHARACTER_ZASP_IDLE1 = 502,
		CHARACTER_ZASP_ANGRY = 503,
		AUDIENCE_KABBU_FRONT = 504,
		AUDIENCE_KABBU_BACK = 505,
		AUDIENCE_KABBU_FRONT_CHEER = 506,
		AUDIENCE_TANJERIN_BACK = 507,
		AUDIENCE_TANJERIN_FRONT_CHEER = 508,
		CARD_BATTLE_BACKGROUND = 509,
		ARTIFACT_MASK = 510,
		ARTIFACT_TABLET = 511,
		ARTIFACT_TABLET_CRACK_1 = 512,
		ARTIFACT_TABLET_CRACK_2 = 513,
		ARTIFACT_KEY_HANDLE = 514,
		ARTIFACT_KEY_TEETH = 515,
		ARTIFACT_KEY_FULL = 516,
		ARTIFACT_ELIZANT = 517,
		ARTIFACT_BROACH = 518,
		ARTIFACT_CROWN = 519,

		FIRST_CUSTOM = 2048,
	};

protected:
	static void _bind_methods();

private:
	PackedByteArray file_id;
	Ref<Image> image;
	Ref<ImageTexture> texture;

public:
	IconDef();
	~IconDef();

	PackedByteArray get_file_id() const;
	void set_file_id(PackedByteArray new_file_id);
	Ref<Image> get_image() const;
	void set_image(Ref<Image> new_image);
	Ref<ImageTexture> get_texture() const;

	static IconDef *convert_legacy_portrait(Icon portrait, PackedByteArray data);
};
DECLARE_ENUM(IconDef::Icon);
DECLARE_PREDEFINED_KEY(Texture2D, ICON);

#endif // ICON_DEF_H
