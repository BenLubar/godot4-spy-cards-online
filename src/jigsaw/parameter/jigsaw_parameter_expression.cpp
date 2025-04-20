#include "jigsaw/parameter/jigsaw_parameter_expression.h"

// TODO: https://github.com/godotengine/godot-proposals/issues/11702

void JigsawParameterExpression::_bind_methods() {
	BIND_PROPERTY(Variant::STRING, expression);
	BIND_PROPERTY(Variant::PACKED_STRING_ARRAY, input_names);
	BIND_PROPERTY_RESOURCE_NOT_SAVED(Expression, cached_expression);
}

IMPLEMENT_PROPERTY(JigsawParameterExpression, String, expression);
IMPLEMENT_PROPERTY(JigsawParameterExpression, PackedStringArray, input_names);
IMPLEMENT_PROPERTY_SIMPLE(JigsawParameterExpression, Ref<Expression>, cached_expression);

Ref<Expression> JigsawParameterExpression::get_parsed_expression() {
	if (likely(_cached_expression.is_valid())) {
		return _cached_expression;
	}

	if (likely(_expression.is_empty())) {
		return Ref<Expression>();
	}

	Ref<Expression> expr;
	expr.instantiate();

	Error err = expr->parse(_expression, _input_names);
	ERR_FAIL_COND_V(err != OK, Ref<Expression>());

	_cached_expression = expr;
	return expr;
}

Ref<JigsawParameterExpression> JigsawParameterExpression::make(const String &expression, const PackedStringArray &input_names) {
	Ref<JigsawParameterExpression> param;
	param.instantiate();
	param->set_expression(expression);
	param->set_input_names(input_names);
	(void)param->get_parsed_expression();
	return param;
}
