#ifndef CARD_DEF_H
#define CARD_DEF_H

#include "dry.h"

#include "enums/enums_card_def.h"
#include "enums/enums_icon_def.h"
#include "enums/enums_rank_def.h"
#include "enums/enums_tribe_def.h"

class CardDef;

#include "defs/effect_instance.h"
#include "defs/stat_value.h"

class CardDef : public Resource {
	GDCLASS(CardDef, Resource);

public:
	using Card = enums::CardDef::Card;

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(Card, id, = Card::NONE);
	DECLARE_PROPERTY(enums::RankDef::Rank, rank, = enums::RankDef::Rank::NONE);
	DECLARE_PROPERTY(TypedArray<StatValue>, costs);
	DECLARE_PROPERTY(String, name);
	DECLARE_PROPERTY(enums::IconDef::Icon, portrait, = enums::IconDef::Icon::NONE);
	DECLARE_PROPERTY(PackedArray<enums::TribeDef::Tribe>, tribes);
	DECLARE_PROPERTY(TypedArray<EffectInstance>, effects);
	DECLARE_PROPERTY(float, curve_adjust, = 0.0f);

	DEFAULT_TO_STRING();
};

#endif // CARD_DEF_H
