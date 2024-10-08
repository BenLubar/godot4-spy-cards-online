#ifndef JIGSAW_PARAMETER_STRING_H
#define JIGSAW_PARAMETER_STRING_H

#include "jigsaw_parameter.h"

class JigsawParameterString : public JigsawParameter {
	GDCLASS(JigsawParameterString, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterString() = default;
	~JigsawParameterString() = default;

	DECLARE_PROPERTY(String, string);

	Type get_type() const override { return STRING; }

	static Ref<JigsawParameterString> make(const String &string);
};

#endif // JIGSAW_PARAMETER_STRING_H
