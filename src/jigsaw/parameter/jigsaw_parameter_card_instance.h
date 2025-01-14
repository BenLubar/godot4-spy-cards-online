#ifndef JIGSAW_PARAMETER_CARD_INSTANCE_H
#define JIGSAW_PARAMETER_CARD_INSTANCE_H

#include "jigsaw/parameter/jigsaw_parameter.h"

class JigsawParameterCardInstance;

#include "active/card_instance.h"

class JigsawParameterCardInstance : public JigsawParameter {
	GDCLASS(JigsawParameterCardInstance, JigsawParameter);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(int64_t, instance, = -1);

	Type get_type() const override { return CARD_INSTANCE; }

	Ref<CardInstance> resolve(JigsawGlobal *global) const;

	static Ref<JigsawParameterCardInstance> make(int64_t instance);
};

#endif // JIGSAW_PARAMETER_CARD_INSTANCE_H
