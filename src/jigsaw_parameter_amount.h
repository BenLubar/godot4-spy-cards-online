#ifndef JIGSAW_PARAMETER_AMOUNT_H
#define JIGSAW_PARAMETER_AMOUNT_H

#include "jigsaw_parameter.h"

class JigsawParameterAmount : public JigsawParameter {
	GDCLASS(JigsawParameterAmount, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterAmount() = default;
	~JigsawParameterAmount() = default;

	DECLARE_PROPERTY(int64_t, amount, = 0);
	DECLARE_PROPERTY(int64_t, amount_inf, = 0);
	DECLARE_PROPERTY_IS(bool, nan, = false);

	Type get_type() const override { return AMOUNT; }

	static Ref<JigsawParameterAmount> make(int64_t amount, int64_t amount_inf);
	static Ref<JigsawParameterAmount> make_nan();
};

#endif // JIGSAW_PARAMETER_AMOUNT_H
