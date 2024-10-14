#ifndef JIGSAW_PARAMETER_LOCAL_VARIABLE_H
#define JIGSAW_PARAMETER_LOCAL_VARIABLE_H

#include "jigsaw_parameter.h"

class JigsawParameterLocalVariable : public JigsawParameter {
	GDCLASS(JigsawParameterLocalVariable, JigsawParameter);

public:
	constexpr static int64_t FRAME_ARGUMENTS = -1;
	constexpr static int64_t FRAME_RESULTS = -2;
	constexpr static int64_t FRAME_ENVIRONMENT = -3;

protected:
	static void _bind_methods();

public:
	JigsawParameterLocalVariable() = default;
	~JigsawParameterLocalVariable() = default;

	DECLARE_PROPERTY(int64_t, frame, = 0);
	DECLARE_PROPERTY(int64_t, slot, = -1);

	Type get_type() const override { return LOCAL_VARIABLE; }

	static Ref<JigsawParameterLocalVariable> make(int64_t frame, int64_t slot);
};

#endif // JIGSAW_PARAMETER_LOCAL_VARIABLE_H
