#ifndef JIGSAW_PARAMETER_TRIBE_H
#define JIGSAW_PARAMETER_TRIBE_H

#include "jigsaw_parameter.h"

class JigsawParameterTribe;

#include "tribe_def.h"

class JigsawParameterTribe : public JigsawParameter {
	GDCLASS(JigsawParameterTribe, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterTribe() = default;
	~JigsawParameterTribe() = default;

	DECLARE_PROPERTY(TribeDef::Tribe, tribe, = TribeDef::Tribe::NONE);

	Type get_type() const override { return TRIBE; }

	static JigsawParameterTribe *make(TribeDef::Tribe tribe);
};

#endif // JIGSAW_PARAMETER_TRIBE_H
