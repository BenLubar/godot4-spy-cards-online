#ifndef JIGSAW_PARAMETER_BOOLEAN_H
#define JIGSAW_PARAMETER_BOOLEAN_H

#include "jigsaw_parameter.h"

class JigsawParameterBoolean : public JigsawParameter {
	GDCLASS(JigsawParameterBoolean, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterBoolean() = default;
	~JigsawParameterBoolean() = default;

	DECLARE_PROPERTY(bool, boolean, = false);

	Type get_type() const override { return BOOLEAN; }

	static Ref<JigsawParameterBoolean> make(bool boolean);
};

#endif // JIGSAW_PARAMETER_BOOLEAN_H
