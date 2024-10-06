#ifndef CARD_FILTER_PARAM_H
#define CARD_FILTER_PARAM_H

#include "dry.h"

#include "card_filter.h"

class CardFilterParam : public CardFilter {
	GDCLASS(CardFilterParam, CardFilter);

protected:
	static void _bind_methods();

public:
	CardFilterParam() = default;
	~CardFilterParam() = default;

	DECLARE_PROPERTY(Ref<JigsawParameter>, param);

	Type get_type() const override { return PARAM; }
};

#endif // CARD_FILTER_PARAM_H
