#ifndef JIGSAW_PARAMETER_STAT_H
#define JIGSAW_PARAMETER_STAT_H

#include "jigsaw_parameter.h"

#include "enums_stat_def.h"

class JigsawParameterStat : public JigsawParameter {
	GDCLASS(JigsawParameterStat, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterStat() = default;
	~JigsawParameterStat() = default;

	DECLARE_PROPERTY(enums::StatDef::Stat, stat, = enums::StatDef::Stat::NONE);

	Type get_type() const override { return STAT; }

	static Ref<JigsawParameterStat> make(enums::StatDef::Stat stat);
};

#endif // JIGSAW_PARAMETER_STAT_H
