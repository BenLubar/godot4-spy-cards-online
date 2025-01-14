#include "protocol/deck.h"

#include "util/format_helper.h"

void Deck::_bind_methods() {
	ClassDB::bind_static_method("Deck", D_METHOD("encode", "cards"), &Deck::encode);
	ClassDB::bind_static_method("Deck", D_METHOD("decode", "buf"), &Deck::decode);
}

static constexpr uint32_t is_vanilla[4] = {0xffbbc3ff, 0xa7ffffff, 0xffffffff, 0x00018007};
static constexpr enums::CardDef::Card vanilla_enemy[] = {
	enums::CardDef::SEEDLING,
	enums::CardDef::ACORNLING,
	enums::CardDef::UNDERLING,
	enums::CardDef::CACTILING,
	enums::CardDef::FLOWERLING,
	enums::CardDef::PLUMPLING,
	enums::CardDef::GOLDEN_SEEDLING,
	enums::CardDef::ZOMBIANT,
	enums::CardDef::ZOMBEE,
	enums::CardDef::ZOMBEETLE,
	enums::CardDef::JELLYSHROOM,
	enums::CardDef::BLOATSHROOM,
	enums::CardDef::INICHAS,
	enums::CardDef::DENMUKI,
	enums::CardDef::MADESPHY,
	enums::CardDef::NUMBNAIL,
	enums::CardDef::IRONNAIL,
	enums::CardDef::MIDGE,
	enums::CardDef::CHOMPER,
	enums::CardDef::CHOMPER_BRUTE,
	enums::CardDef::WILD_CHOMPER,
	enums::CardDef::WEEVIL,
	enums::CardDef::PSICORP,
	enums::CardDef::ARROW_WORM,
	enums::CardDef::THIEF,
	enums::CardDef::BANDIT,
	enums::CardDef::BURGLAR,
	enums::CardDef::RUFFIAN,
	enums::CardDef::BEE_BOOP,
	enums::CardDef::SECURITY_TURRET,
	enums::CardDef::MENDER,
	enums::CardDef::ABOMIHONEY,
	enums::CardDef::KRAWLER,
	enums::CardDef::WARDEN,
	enums::CardDef::HAUNTED_CLOTH,
	enums::CardDef::LEAFBUG_NINJA,
	enums::CardDef::LEAFBUG_ARCHER,
	enums::CardDef::LEAFBUG_CLUBBER,
	enums::CardDef::MANTIDFLY,
	enums::CardDef::JUMPING_SPIDER,
	enums::CardDef::MIMIC_SPIDER,
	enums::CardDef::DIVING_SPIDER,
	enums::CardDef::WATER_STRIDER,
	enums::CardDef::BELOSTOSS,
	enums::CardDef::MOTHFLY,
	enums::CardDef::MOTHFLY_CLUSTER,
	enums::CardDef::WASP_SCOUT,
	enums::CardDef::WASP_TROOPER,
	enums::CardDef::WASP_BOMBER,
	enums::CardDef::WASP_DRILLER,
	enums::CardDef::VENUS_BUD,
};
static constexpr enums::CardDef::Card vanilla_mini_boss[] = {
	enums::CardDef::ACOLYTE_ARIA,
	enums::CardDef::MOTHIVA,
	enums::CardDef::ZASP,
	enums::CardDef::AHONEYNATION,
	enums::CardDef::ASTOTHELES,
	enums::CardDef::DUNE_SCORPION,
	enums::CardDef::PRIMAL_WEEVIL,
	enums::CardDef::CROSS,
	enums::CardDef::POI,
	enums::CardDef::GENERAL_ULTIMAX,
	enums::CardDef::CENN,
	enums::CardDef::PISCI,
	enums::CardDef::MONSIEUR_SCARLET,
	enums::CardDef::KABBU,
	enums::CardDef::KALI,
	enums::CardDef::CARMINA,
	enums::CardDef::RIZ,
	enums::CardDef::KINA,
	enums::CardDef::YIN,
	enums::CardDef::STRATOS,
	enums::CardDef::DELILAH,
	enums::CardDef::DEAD_LANDER_ALPHA,
	enums::CardDef::DEAD_LANDER_BETA,
	enums::CardDef::DEAD_LANDER_GAMMA,
};
static constexpr enums::CardDef::Card vanilla_boss[] = {
	enums::CardDef::SPIDER,
	enums::CardDef::VENUS_GUARDIAN,
	enums::CardDef::HEAVY_DRONE_B33,
	enums::CardDef::THE_WATCHER,
	enums::CardDef::THE_BEAST,
	enums::CardDef::ULTIMAX_TANK,
	enums::CardDef::MOTHER_CHOMPER,
	enums::CardDef::BROODMOTHER,
	enums::CardDef::ZOMMOTH,
	enums::CardDef::SEEDLING_KING,
	enums::CardDef::TIDAL_WYRM,
	enums::CardDef::PEACOCK_SPIDER,
	enums::CardDef::DEVOURER,
	enums::CardDef::FALSE_MONARCH,
	enums::CardDef::MAKI,
	enums::CardDef::WASP_KING,
	enums::CardDef::THE_EVERLASTING_KING,
};

template<typename T, size_t N>
constexpr static bool array_has(const T (&array)[N], const T &value) {
	for (size_t i = 0; i < N; i++) {
		if (array[i] == value) {
			return true;
		}
	}

	return false;
}

template<typename T, size_t N>
constexpr static size_t array_index(const T (&array)[N], const T &value) {
	for (size_t i = 0; i < N; i++) {
		if (array[i] == value) {
			return i;
		}
	}

	return N;
}

PackedByteArray Deck::encode(const PackedArray<enums::CardDef::Card> &cards) {
	if (cards.size() == 0) {
		return PackedByteArray();
	}

	bool any_non_basic = false;
	bool any_above_255 = false;
	bool any_below_128 = false;

	for (int64_t i = 0; i < cards.size(); i++) {
		enums::CardDef::Card card = cards[i];
		if (card < 128) {
			any_below_128 = true;
		}

		if (card > 255) {
			any_above_255 = true;
		}

		if (card >= enums::CardDef::FIRST_CUSTOM || !(is_vanilla[card / 32] & (1 << (card % 32)))) {
			any_non_basic = true;
		}

		switch (i) {
		case 0:
			if (!array_has(vanilla_boss, card)) {
				any_non_basic = true;
			}
			break;
		case 1:
		case 2:
			if (!array_has(vanilla_mini_boss, card)) {
				any_non_basic = true;
			}
			break;
		default:
			if (!array_has(vanilla_enemy, card)) {
				any_non_basic = true;
			}
			break;
		}
	}

	if (cards.size() <= 3) {
		any_non_basic = true;
	}

	Ref<FormatHelper> fh = FormatHelper::write("deck");

	if (!any_below_128) {
		fh->write_uint8(0x82);

		for (int64_t i = 0; i < cards.size(); i++) {
			fh->write_uvarint(cards[i] - 128);
		}
	} else if (any_above_255) {
		fh->write_uint8(0x81);

		for (int64_t i = 0; i < cards.size(); i++) {
			fh->write_uvarint(cards[i]);
		}
	} else if (any_non_basic) {
		fh->write_uint8(0x80);

		for (int64_t i = 0; i < cards.size(); i++) {
			fh->write_uint8(cards[i]);
		}
	} else {
		uint8_t c0 = array_index(vanilla_boss, cards[0]);
		uint8_t c1 = array_index(vanilla_mini_boss, cards[1]);
		uint8_t c2 = array_index(vanilla_mini_boss, cards[2]);
		uint8_t c3 = 0;

		fh->write_uint8((c0 << 2) | (c1 >> 3));
		fh->write_uint8((c1 << 5) | c2);

		PackedArray<enums::CardDef::Card> remaining_cards = cards.slice(3);

		while (remaining_cards.size() >= 4) {
			c0 = array_index(vanilla_enemy, remaining_cards[0]);
			c1 = array_index(vanilla_enemy, remaining_cards[1]);
			c2 = array_index(vanilla_enemy, remaining_cards[2]);
			c3 = array_index(vanilla_enemy, remaining_cards[3]);

			fh->write_uint8((c0 << 2) | (c1 >> 4));
			fh->write_uint8((c1 << 4) | (c2 >> 2));
			fh->write_uint8((c2 << 6) | c3);

			remaining_cards = remaining_cards.slice(4);
		}

		switch (remaining_cards.size()) {
		case 0:
			break;
		case 1:
			c0 = array_index(vanilla_enemy, remaining_cards[0]);
			fh->write_uint8(c0 << 2);
			break;
		case 2:
			c0 = array_index(vanilla_enemy, remaining_cards[0]);
			c1 = array_index(vanilla_enemy, remaining_cards[1]);
			fh->write_uint8((c0 << 2) | (c1 >> 4));
			fh->write_uint8(c1 << 4);
			break;
		case 3:
			c0 = array_index(vanilla_enemy, remaining_cards[0]);
			c1 = array_index(vanilla_enemy, remaining_cards[1]);
			c2 = array_index(vanilla_enemy, remaining_cards[2]);
			fh->write_uint8((c0 << 2) | (c1 >> 4));
			fh->write_uint8((c1 << 4) | (c2 >> 2));
			fh->write_uint8((c2 << 6) | 63);
			break;
		}
	}

	return fh->get_buffer();
}
PackedArray<enums::CardDef::Card> Deck::decode(const PackedByteArray &buf) {
	PackedArray<enums::CardDef::Card> deck;
	if (buf.is_empty()) {
		return deck;
	}

	if (buf[0] < 0x80) {
		ERR_FAIL_COND_V(buf.size() < 2, PackedArray<enums::CardDef::Card>());

#define CARD_INDEX(m_array, m_index) \
		ERR_FAIL_COND_V(uint8_t((m_index)) >= (sizeof((m_array)) / sizeof((m_array)[0])), PackedArray<enums::CardDef::Card>()); \
		deck.append((m_array)[uint8_t((m_index))])

		CARD_INDEX(vanilla_boss, buf[0] >> 2);
		CARD_INDEX(vanilla_mini_boss, ((buf[0] & 3) << 3) | (buf[1] >> 5));
		CARD_INDEX(vanilla_mini_boss, buf[1] & 31);

		PackedByteArray b = buf.slice(2);
		while (b.size() > 3) {
			CARD_INDEX(vanilla_enemy, b[0] >> 2);
			CARD_INDEX(vanilla_enemy, ((b[0] & 3) << 4) | (b[1] >> 4));
			CARD_INDEX(vanilla_enemy, ((b[1] & 15) << 2) | (b[2] >> 6));
			CARD_INDEX(vanilla_enemy, b[2] & 63);
			b = b.slice(3);
		}

		CARD_INDEX(vanilla_enemy, b[0] >> 2);

		if (b.size() >= 2) {
			CARD_INDEX(vanilla_enemy, ((b[0] & 3) << 4) | (b[1] >> 4));

			if (b.size() >= 3) {
				CARD_INDEX(vanilla_enemy, ((b[1] & 15) << 2) | (b[2] >> 6));

				if ((b[2] & 63) != 63) {
					CARD_INDEX(vanilla_enemy, b[2] & 63);
				}
			}
		}

		return deck;
	}

	Ref<FormatHelper> fh = FormatHelper::read("deck", buf);
	switch (fh->read_uint8()) {
	case 0x80:
		while (!fh->is_eof()) {
			deck.append(fh->read_uint8());
		}
		break;
	case 0x81:
		while (!fh->is_eof()) {
			deck.append(fh->read_uvarint());
		}
		break;
	case 0x82:
		while (!fh->is_eof()) {
			deck.append(fh->read_uvarint() + 128);
		}
		break;
	default:
		ERR_FAIL_V(PackedArray<enums::CardDef::Card>());
	}

	ERR_FAIL_COND_V(!fh->is_valid_eof(), PackedArray<enums::CardDef::Card>());

	return deck;
}
