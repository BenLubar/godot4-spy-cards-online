#ifndef CARD_FILTER_H
#define CARD_FILTER_H

#include "dry.h"

#include "enums_card_def.h"
#include "enums_rank_def.h"
#include "enums_tribe_def.h"
#include "enums_stat_def.h"

class CardFilter;

#include "card_instance.h"
#include "jigsaw_context.h"
#include "jigsaw_parameter.h"

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
	bool matches_def(const Ref<JigsawContext> &ctx, enums::CardDef::Card card) const;
	bool matches_instance(const Ref<JigsawContext> &ctx, const Ref<CardInstance> &inst) const;
	virtual bool _matches_def(const Ref<JigsawContext> &ctx, enums::CardDef::Card card) const = 0;
	virtual bool _matches_instance(const Ref<JigsawContext> &ctx, const Ref<CardInstance> &inst) const = 0;

	static Ref<CardFilter> make_and(const TypedArray<CardFilter> &list);
	static Ref<CardFilter> make_or(const TypedArray<CardFilter> &list);
	static Ref<CardFilter> make_card(enums::CardDef::Card card);
	static Ref<CardFilter> make_rank(enums::RankDef::Rank rank);
	static Ref<CardFilter> make_tribe(enums::TribeDef::Tribe tribe);
	static Ref<CardFilter> make_cost(enums::StatDef::Stat stat, int64_t amount, int64_t amount_inf);
	static Ref<CardFilter> make_cost_nan(enums::StatDef::Stat stat);
	static Ref<CardFilter> make_param(const Ref<JigsawParameter> &param);
};
DECLARE_ENUM(CardFilter::Type);

#endif // CARD_FILTER_H
