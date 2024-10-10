#include "jigsaw_parameter_audience.h"

void JigsawParameterAudience::_bind_methods() {
	BIND_PROPERTY(Variant::INT, audience);
}

IMPLEMENT_PROPERTY(JigsawParameterAudience, int64_t, audience);
