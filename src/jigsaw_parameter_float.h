#ifndef JIGSAW_PARAMETER_FLOAT_H
#define JIGSAW_PARAMETER_FLOAT_H

#include "jigsaw_parameter.h"

class JigsawParameterFloat : public JigsawParameter {
	GDCLASS(JigsawParameterFloat, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterFloat() = default;
	~JigsawParameterFloat() = default;

	DECLARE_PROPERTY(double, value, = 0.0);

	Type get_type() const override { return FLOAT; }

	static Ref<JigsawParameterFloat> make(double value);
};

#endif // JIGSAW_PARAMETER_FLOAT_H
