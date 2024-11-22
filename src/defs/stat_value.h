#ifndef STAT_VALUE_H
#define STAT_VALUE_H

#include "defs/numeric_value.h"

#include "enums/enums_stat_def.h"

class StatValue : public NumericValue {
	GDCLASS(StatValue, NumericValue);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(enums::StatDef::Stat, stat, = enums::StatDef::Stat::NONE);

	DEFAULT_TO_STRING();
};

#endif // STAT_VALUE_H
