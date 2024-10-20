#ifndef JIGSAW_PARAMETER_RANK_H
#define JIGSAW_PARAMETER_RANK_H

#include "jigsaw_parameter.h"

#include "enums_rank_def.h"

class JigsawParameterRank : public JigsawParameter {
	GDCLASS(JigsawParameterRank, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterRank() = default;
	~JigsawParameterRank() = default;

	DECLARE_PROPERTY(enums::RankDef::Rank, rank, = enums::RankDef::Rank::NONE);

	Type get_type() const override { return RANK; }

	static Ref<JigsawParameterRank> make(enums::RankDef::Rank rank);
};

#endif // JIGSAW_PARAMETER_RANK_H
