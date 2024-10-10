#ifndef JIGSAW_PARAMETER_AUDIENCE_INSTANCE_H
#define JIGSAW_PARAMETER_AUDIENCE_INSTANCE_H

#include "jigsaw_parameter.h"

class JigsawParameterAudienceInstance : public JigsawParameter {
	GDCLASS(JigsawParameterAudienceInstance, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterAudienceInstance() = default;
	~JigsawParameterAudienceInstance() = default;

	DECLARE_PROPERTY(int64_t, group, = -1);
	DECLARE_PROPERTY(int64_t, member, = -1);

	Type get_type() const override { return AUDIENCE_INSTANCE; }

	static Ref<JigsawParameterAudienceInstance> make(int64_t group, int64_t member);
};

#endif // JIGSAW_PARAMETER_AUDIENCE_INSTANCE_H
