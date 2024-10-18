#ifndef JIGSAW_PARAMETER_CARD_GRID_H
#define JIGSAW_PARAMETER_CARD_GRID_H

#include "jigsaw_parameter.h"

class JigsawParameterCardGrid : public JigsawParameter {
	GDCLASS(JigsawParameterCardGrid, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterCardGrid() = default;
	~JigsawParameterCardGrid() = default;

	DECLARE_PROPERTY(int64_t, grid, = -1);

	Type get_type() const override { return CARD_GRID; }

	static Ref<JigsawParameterCardGrid> make(int64_t grid);
};

#endif // JIGSAW_PARAMETER_CARD_GRID_H
