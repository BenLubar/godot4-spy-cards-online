#ifndef JIGSAW_PARAMETER_ORDERED_LIST_H
#define JIGSAW_PARAMETER_ORDERED_LIST_H

#include "jigsaw_parameter.h"

class JigsawParameterOrderedList : public JigsawParameter {
	GDCLASS(JigsawParameterOrderedList, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterOrderedList() = default;
	~JigsawParameterOrderedList() = default;

	DECLARE_PROPERTY(TypedArray<JigsawParameter>, list);
	DECLARE_PROPERTY_IS(bool, template, = false);

	Type get_type() const override { return ORDERED_LIST; }

	bool is_homogenous(Type type) const;

	static Ref<JigsawParameterOrderedList> make(const TypedArray<JigsawParameter> &list);
	static Ref<JigsawParameterOrderedList> make_template(const TypedArray<JigsawParameter> &list);
};

#endif // JIGSAW_PARAMETER_ORDERED_LIST_H
