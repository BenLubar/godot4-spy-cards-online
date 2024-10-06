#include "jigsaw_parameter_tribe.h"

void JigsawParameterTribe::_bind_methods() {
	BIND_PROPERTY(Variant::INT, tribe);
}
	
IMPLEMENT_PROPERTY(JigsawParameterTribe, TribeDef::Tribe, tribe);
