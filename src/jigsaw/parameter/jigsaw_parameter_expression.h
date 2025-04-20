#ifndef JIGSAW_PARAMETER_EXPRESSION_H
#define JIGSAW_PARAMETER_EXPRESSION_H

#include "jigsaw/parameter/jigsaw_parameter.h"

class JigsawParameterExpression;

#include <godot_cpp/classes/expression.hpp>

class JigsawParameterExpression : public JigsawParameter {
	GDCLASS(JigsawParameterExpression, JigsawParameter);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(String, expression);
	DECLARE_PROPERTY(PackedStringArray, input_names);
	DECLARE_PROPERTY(Ref<Expression>, cached_expression);

	Type get_type() const override { return EXPRESSION; }

	Ref<Expression> get_parsed_expression();

	static Ref<JigsawParameterExpression> make(const String &expression, const PackedStringArray &input_names);
};

#endif // JIGSAW_PARAMETER_EXPRESSION_H
