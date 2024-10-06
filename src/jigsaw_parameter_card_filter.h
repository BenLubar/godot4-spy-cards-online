#ifndef JIGSAW_PARAMETER_CARD_FILTER_H
#define JIGSAW_PARAMETER_CARD_FILTER_H

#include "jigsaw_parameter.h"

class JigsawParameterCardFilter;

#include "card_filter.h"

class JigsawParameterCardFilter : public JigsawParameter {
	GDCLASS(JigsawParameterCardFilter, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterCardFilter() = default;
	~JigsawParameterCardFilter() = default;

	DECLARE_PROPERTY(Ref<CardFilter>, filter);

	Type get_type() const override { return CARD_FILTER; }
};

#endif // JIGSAW_PARAMETER_CARD_FILTER_H
