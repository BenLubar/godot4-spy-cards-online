#include "jigsaw_parameter_stat.h"

void JigsawParameterStat::_bind_methods() {
	BIND_PROPERTY(Variant::INT, stat);

	ClassDB::bind_static_method("JigsawParameterStat", D_METHOD("make", "stat"), &JigsawParameterStat::make);
}

IMPLEMENT_PROPERTY(JigsawParameterStat, StatDef::Stat, stat);

JigsawParameterStat *JigsawParameterStat::make(StatDef::Stat stat) {
	JigsawParameterStat *param = memnew(JigsawParameterStat);
	param->set_stat(stat);
	return param;
}
