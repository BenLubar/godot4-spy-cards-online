#ifndef JIGSAW_PARAMETER_MODIFIER_INSTANCE_H
#define JIGSAW_PARAMETER_MODIFIER_INSTANCE_H

#include "jigsaw/parameter/jigsaw_parameter.h"

class JigsawParameterModifierInstance;

#include "active/modifier_instance.h"

class JigsawParameterModifierInstance : public JigsawParameter {
	GDCLASS(JigsawParameterModifierInstance, JigsawParameter);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(int64_t, card_instance, = -1);
	DECLARE_PROPERTY(int64_t, modifier_index, = -1);

	Type get_type() const override { return MODIFIER_INSTANCE; }

	static Ref<JigsawParameterModifierInstance> make(int64_t card_instance, int64_t modifier_index);
};

#endif // JIGSAW_PARAMETER_MODIFIER_INSTANCE_H
