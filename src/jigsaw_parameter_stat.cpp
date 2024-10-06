#include "jigsaw_parameter_stat.h"

void JigsawParameterStat::_bind_methods() {
	BIND_PROPERTY(Variant::INT, stat);
}

IMPLEMENT_PROPERTY(JigsawParameterStat, StatDef::Stat, stat);
