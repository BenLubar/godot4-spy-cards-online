#include "jigsaw_parameter_card_grid.h"

void JigsawParameterCardGrid::_bind_methods() {
	BIND_PROPERTY(Variant::INT, grid);
}

IMPLEMENT_PROPERTY(JigsawParameterCardGrid, int64_t, grid);
