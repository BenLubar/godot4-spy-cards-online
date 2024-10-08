#ifndef JIGSAW_PARAMETER_CARD_INSTANCE_H
#define JIGSAW_PARAMETER_CARD_INSTANCE_H

#include "jigsaw_parameter.h"

class JigsawParameterCardInstance;

#include "card_instance.h"

class JigsawParameterCardInstance : public JigsawParameter {
	GDCLASS(JigsawParameterCardInstance, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterCardInstance() = default;
	~JigsawParameterCardInstance() = default;

	DECLARE_PROPERTY(Ref<CardInstance>, instance);

	Type get_type() const override { return CARD_INSTANCE; }

	static Ref<JigsawParameterCardInstance> make(const Ref<CardInstance> &instance);
};

#endif // JIGSAW_PARAMETER_CARD_INSTANCE_H
