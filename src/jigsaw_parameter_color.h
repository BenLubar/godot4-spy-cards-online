#ifndef JIGSAW_PARAMETER_COLOR_H
#define JIGSAW_PARAMETER_COLOR_H

#include "jigsaw_parameter.h"

class JigsawParameterColor : public JigsawParameter {
	GDCLASS(JigsawParameterColor, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterColor() = default;
	~JigsawParameterColor() = default;

	DECLARE_PROPERTY(Color, color);

	Type get_type() const override { return COLOR; }

	static Ref<JigsawParameterColor> make(Color color);
};

#endif // JIGSAW_PARAMETER_COLOR_H
