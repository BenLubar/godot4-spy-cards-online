#ifndef NUMERIC_VALUE_H
#define NUMERIC_VALUE_H

#include "dry.h"

class NumericValue : public Resource {
	GDCLASS(NumericValue, Resource);

protected:
	static void _bind_methods();

public:
	NumericValue() = default;
	~NumericValue() = default;

	DECLARE_PROPERTY(int64_t, amount, = 0);
	DECLARE_PROPERTY(int64_t, amount_inf, = 0);
	DECLARE_PROPERTY_IS(bool, nan, = false);
};

#endif // NUMERIC_VALUE_H
