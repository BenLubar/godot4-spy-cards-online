#ifndef JIGSAW_PARAMETER_MODIFIER_INSTANCE_H
#define JIGSAW_PARAMETER_MODIFIER_INSTANCE_H

#include "jigsaw_parameter.h"

class JigsawParameterModifierInstance : public JigsawParameter {
	GDCLASS(JigsawParameterModifierInstance, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterModifierInstance() = default;
	~JigsawParameterModifierInstance() = default;

	Type get_type() const override { return MODIFIER_INSTANCE; }
};

#endif // JIGSAW_PARAMETER_MODIFIER_INSTANCE_H
