#ifndef JIGSAW_PARAMETER_CARD_GRID_H
#define JIGSAW_PARAMETER_CARD_GRID_H

#include "jigsaw/parameter/jigsaw_parameter.h"

class JigsawParameterCardGrid : public JigsawParameter {
	GDCLASS(JigsawParameterCardGrid, JigsawParameter);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(int64_t, grid, = -1);
	DECLARE_PROPERTY(bool, is_3d, = true);

	Type get_type() const override { return CARD_GRID; }

	static Ref<JigsawParameterCardGrid> make(int64_t grid, bool is_3d);
};

#endif // JIGSAW_PARAMETER_CARD_GRID_H
