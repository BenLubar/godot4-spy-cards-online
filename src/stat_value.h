#ifndef STAT_VALUE_H
#define STAT_VALUE_H

#include "dry.h"

class StatValue;

#include "stat_def.h"

class StatValue : public Resource {
	GDCLASS(StatValue, Resource);

protected:
	static void _bind_methods();

public:
	StatValue() = default;
	~StatValue() = default;

	DECLARE_PROPERTY(StatDef::Stat, stat, = StatDef::Stat::NONE);
	DECLARE_PROPERTY(int64_t, amount, = 0);
	DECLARE_PROPERTY(int64_t, amount_inf, = 0);
	DECLARE_PROPERTY_IS(bool, nan, = false);
};

#endif // STAT_VALUE_H
