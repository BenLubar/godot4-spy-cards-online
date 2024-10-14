#ifndef JIGSAW_PARAMETER_MODIFIER_H
#define JIGSAW_PARAMETER_MODIFIER_H

#include "jigsaw_parameter.h"

#include "enums_modifier_def.h"

class JigsawParameterModifier : public JigsawParameter {
	GDCLASS(JigsawParameterModifier, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterModifier() = default;
	~JigsawParameterModifier() = default;

	DECLARE_PROPERTY(enums::ModifierDef::Modifier, modifier, = enums::ModifierDef::Modifier::NONE);

	Type get_type() const override { return MODIFIER; }

	static Ref<JigsawParameterModifier> make(enums::ModifierDef::Modifier modifier);
};

#endif // JIGSAW_PARAMETER_MODIFIER_H
