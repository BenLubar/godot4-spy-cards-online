#ifndef JIGSAW_PARAMETER_STAT_VALUE_H
#define JIGSAW_PARAMETER_STAT_VALUE_H

#include "jigsaw/parameter/jigsaw_parameter.h"

#include "enums/enums_stat_def.h"

class JigsawParameterStatValue;

#include "defs/stat_value.h"

class JigsawParameterStatValue : public JigsawParameter {
	GDCLASS(JigsawParameterStatValue, JigsawParameter);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(enums::StatDef::Stat, stat, = enums::StatDef::Stat::NONE);
	DECLARE_PROPERTY(int64_t, amount, = 0);
	DECLARE_PROPERTY(int64_t, amount_inf, = 0);
	DECLARE_PROPERTY_IS(bool, nan, = false);

	Type get_type() const override { return STAT_VALUE; }

	static Ref<JigsawParameterStatValue> make(enums::StatDef::Stat stat, int64_t amount, int64_t amount_inf = 0);
	static Ref<JigsawParameterStatValue> make_nan(enums::StatDef::Stat stat);
	static Ref<JigsawParameterStatValue> convert(const Ref<StatValue> &stat_value);
};

#endif // JIGSAW_PARAMETER_STAT_VALUE_H
