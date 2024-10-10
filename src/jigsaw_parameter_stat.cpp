#include "jigsaw_parameter_stat.h"

void JigsawParameterStat::_bind_methods() {
	BIND_PROPERTY_ENUM(StatDef::Stat, stat);

	ClassDB::bind_static_method("JigsawParameterStat", D_METHOD("make", "stat"), &JigsawParameterStat::make);
}

IMPLEMENT_PROPERTY(JigsawParameterStat, StatDef::Stat, stat);

Ref<JigsawParameterStat> JigsawParameterStat::make(StatDef::Stat stat) {
	Ref<JigsawParameterStat> param;
	param.instantiate();
	param->set_stat(stat);
	return param;
}
