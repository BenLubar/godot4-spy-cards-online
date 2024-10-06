#ifndef CARD_FILTER_H
#define CARD_FILTER_H

#include "dry.h"

class CardFilter;

#include "card_def.h"
#include "card_instance.h"

class CardFilter : public Resource {
	GDCLASS(CardFilter, Resource);

public:
	enum Type {
		AND = 0,
		OR = 1,

		CARD = 2,
		RANK = 3,
		TRIBE = 4,
		COST = 5,
		PARAM = 6,
	};

protected:
	static void _bind_methods();

public:
	CardFilter() = default;
	~CardFilter() = default;

	DECLARE_PROPERTY(bool, negate, = false);

	virtual Type get_type() const = 0;
	GDVIRTUAL2RC(bool, _matches_def, JigsawGlobal *, CardDef::Card);
	GDVIRTUAL2RC(bool, _matches_instance, JigsawGlobal *, const Ref<CardInstance> &);

	static Ref<CardFilter> make_and(const TypedArray<CardFilter> &list);
	static Ref<CardFilter> make_or(const TypedArray<CardFilter> &list);
	static Ref<CardFilter> make_card(CardDef::Card card);
	static Ref<CardFilter> make_rank(RankDef::Rank rank);
	static Ref<CardFilter> make_tribe(TribeDef::Tribe tribe);
	static Ref<CardFilter> make_cost(StatDef::Stat stat, int64_t amount);
	static Ref<CardFilter> make_param(const Ref<JigsawParameter> &param);
};
DECLARE_ENUM(CardFilter::Type);

#endif // CARD_FILTER_H
