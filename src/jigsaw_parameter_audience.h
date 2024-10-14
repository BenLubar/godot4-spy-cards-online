#ifndef JIGSAW_PARAMETER_AUDIENCE_H
#define JIGSAW_PARAMETER_AUDIENCE_H

#include "jigsaw_parameter.h"

#include "enums_audience_def.h"

class JigsawParameterAudience : public JigsawParameter {
	GDCLASS(JigsawParameterAudience, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterAudience() = default;
	~JigsawParameterAudience() = default;

	DECLARE_PROPERTY(enums::AudienceDef::Audience, audience, = enums::AudienceDef::Audience::NONE);

	Type get_type() const override { return AUDIENCE; }

	static Ref<JigsawParameterAudience> make(enums::AudienceDef::Audience audience);
};

#endif // JIGSAW_PARAMETER_AUDIENCE_H
