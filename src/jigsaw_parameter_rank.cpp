#include "jigsaw_parameter_rank.h"

void JigsawParameterRank::_bind_methods() {
	BIND_PROPERTY(Variant::INT, rank);

	ClassDB::bind_static_method("JigsawParameterRank", D_METHOD("make", "rank"), &JigsawParameterRank::make);
}

IMPLEMENT_PROPERTY(JigsawParameterRank, RankDef::Rank, rank);

Ref<JigsawParameterRank> JigsawParameterRank::make(RankDef::Rank rank) {
	Ref<JigsawParameterRank> param;
	param.instantiate();
	param->set_rank(rank);
	return param;
}
