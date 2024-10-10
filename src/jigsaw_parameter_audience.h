#ifndef JIGSAW_PARAMETER_AUDIENCE_H
#define JIGSAW_PARAMETER_AUDIENCE_H

#include "jigsaw_parameter.h"

class JigsawParameterAudience : public JigsawParameter {
	GDCLASS(JigsawParameterAudience, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterAudience() = default;
	~JigsawParameterAudience() = default;

	DECLARE_PROPERTY(int64_t, audience, = -1);

	Type get_type() const override { return AUDIENCE; }

	static Ref<JigsawParameterAudience> make(int64_t audience);
};

#endif // JIGSAW_PARAMETER_AUDIENCE_H
