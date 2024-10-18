#ifndef STAT_VALUE_H
#define STAT_VALUE_H

#include "numeric_value.h"

#include "enums_stat_def.h"

class StatValue : public NumericValue {
	GDCLASS(StatValue, NumericValue);

protected:
	static void _bind_methods();

public:
	StatValue() = default;
	~StatValue() = default;

	DECLARE_PROPERTY(enums::StatDef::Stat, stat, = enums::StatDef::Stat::NONE);
};

#endif // STAT_VALUE_H
