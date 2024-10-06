#ifndef JIGSAW_PARAMETER_FORMATTED_TEXT_H
#define JIGSAW_PARAMETER_FORMATTED_TEXT_H

#include "jigsaw_parameter.h"

class JigsawParameterFormattedText;

#include "formatted_text.h"

class JigsawParameterFormattedText : public JigsawParameter {
	GDCLASS(JigsawParameterFormattedText, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterFormattedText() = default;
	~JigsawParameterFormattedText() = default;

	DECLARE_PROPERTY(TypedArray<FormattedText>, text);

	Type get_type() const override { return FORMATTED_TEXT; }

	static JigsawParameterFormattedText *make(const TypedArray<FormattedText> &text);
};

#endif // JIGSAW_PARAMETER_FORMATTED_TEXT_H
