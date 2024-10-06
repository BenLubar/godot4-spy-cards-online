#ifndef JIGSAW_PARAMETER_MODIFIER_H
#define JIGSAW_PARAMETER_MODIFIER_H

#include "jigsaw_parameter.h"

class JigsawParameterModifier;

#include "modifier_def.h"

class JigsawParameterModifier : public JigsawParameter {
	GDCLASS(JigsawParameterModifier, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterModifier() = default;
	~JigsawParameterModifier() = default;

	DECLARE_PROPERTY(ModifierDef::Modifier, modifier, = ModifierDef::Modifier::NONE);

	Type get_type() const override { return MODIFIER; }

	static JigsawParameterModifier *make(ModifierDef::Modifier modifier);
};

#endif // JIGSAW_PARAMETER_MODIFIER_H
