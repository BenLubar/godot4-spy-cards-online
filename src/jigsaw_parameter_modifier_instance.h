#ifndef JIGSAW_PARAMETER_MODIFIER_INSTANCE_H
#define JIGSAW_PARAMETER_MODIFIER_INSTANCE_H

#include "jigsaw_parameter.h"

class JigsawParameterModifierInstance;

#include "modifier_instance.h"

class JigsawParameterModifierInstance : public JigsawParameter {
	GDCLASS(JigsawParameterModifierInstance, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterModifierInstance() = default;
	~JigsawParameterModifierInstance() = default;

	DECLARE_PROPERTY(Ref<ModifierInstance>, instance);

	Type get_type() const override { return MODIFIER_INSTANCE; }

	static Ref<JigsawParameterModifierInstance> make(const Ref<ModifierInstance> &instance);
};

#endif // JIGSAW_PARAMETER_MODIFIER_INSTANCE_H
