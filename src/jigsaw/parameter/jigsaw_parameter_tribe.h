#ifndef JIGSAW_PARAMETER_TRIBE_H
#define JIGSAW_PARAMETER_TRIBE_H

#include "jigsaw/parameter/jigsaw_parameter.h"

#include "enums/enums_tribe_def.h"

class JigsawParameterTribe : public JigsawParameter {
	GDCLASS(JigsawParameterTribe, JigsawParameter);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(enums::TribeDef::Tribe, tribe, = enums::TribeDef::Tribe::NONE);

	Type get_type() const override { return TRIBE; }

	static Ref<JigsawParameterTribe> make(enums::TribeDef::Tribe tribe);
};

#endif // JIGSAW_PARAMETER_TRIBE_H
