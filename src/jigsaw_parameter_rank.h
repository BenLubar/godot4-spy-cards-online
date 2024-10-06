#ifndef JIGSAW_PARAMETER_RANK_H
#define JIGSAW_PARAMETER_RANK_H

#include "jigsaw_parameter.h"

class JigsawParameterRank;

#include "rank_def.h"

class JigsawParameterRank : public JigsawParameter {
	GDCLASS(JigsawParameterRank, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterRank() = default;
	~JigsawParameterRank() = default;

	DECLARE_PROPERTY(RankDef::Rank, rank, = RankDef::Rank::NONE);

	Type get_type() const override { return RANK; }
};

#endif // JIGSAW_PARAMETER_RANK_H
