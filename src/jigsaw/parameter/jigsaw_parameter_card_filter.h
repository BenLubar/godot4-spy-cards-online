#ifndef JIGSAW_PARAMETER_CARD_FILTER_H
#define JIGSAW_PARAMETER_CARD_FILTER_H

#include "jigsaw/parameter/jigsaw_parameter.h"

class JigsawParameterCardFilter;

#include "defs/card_filter.h"

class JigsawParameterCardFilter : public JigsawParameter {
	GDCLASS(JigsawParameterCardFilter, JigsawParameter);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(Ref<CardFilter>, filter);

	Type get_type() const override { return CARD_FILTER; }

	static Ref<JigsawParameterCardFilter> make(const Ref<CardFilter> &filter);
};

#endif // JIGSAW_PARAMETER_CARD_FILTER_H
