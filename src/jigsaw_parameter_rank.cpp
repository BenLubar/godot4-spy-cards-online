#include "jigsaw_parameter_rank.h"

void JigsawParameterRank::_bind_methods() {
	BIND_PROPERTY(Variant::INT, rank);
}

IMPLEMENT_PROPERTY(JigsawParameterRank, RankDef::Rank, rank);
