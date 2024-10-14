#include "jigsaw_parameter_rank.h"

void JigsawParameterRank::_bind_methods() {
	BIND_PROPERTY_ENUM(enums::RankDef::Rank, rank);

	ClassDB::bind_static_method("JigsawParameterRank", D_METHOD("make", "rank"), &JigsawParameterRank::make);
}

IMPLEMENT_PROPERTY(JigsawParameterRank, enums::RankDef::Rank, rank);

Ref<JigsawParameterRank> JigsawParameterRank::make(enums::RankDef::Rank rank) {
	Ref<JigsawParameterRank> param;
	param.instantiate();
	param->set_rank(rank);
	return param;
}
