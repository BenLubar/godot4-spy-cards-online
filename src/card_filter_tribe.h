#ifndef CARD_FILTER_TRIBE_H
#define CARD_FILTER_TRIBE_H

#include "card_filter.h"

class CardFilterTribe : public CardFilter {
	GDCLASS(CardFilterTribe, CardFilter);

protected:
	static void _bind_methods();

public:
	CardFilterTribe() = default;
	~CardFilterTribe() = default;

	DECLARE_PROPERTY(enums::TribeDef::Tribe, tribe, = enums::TribeDef::Tribe::NONE);

	Type get_type() const override { return TRIBE; }
	bool _matches_def(const Ref<JigsawContext> &ctx, enums::CardDef::Card card) const override;
	bool _matches_instance(const Ref<JigsawContext> &ctx, const Ref<CardInstance> &inst) const override;
};

#endif // CARD_FILTER_TRIBE_H
