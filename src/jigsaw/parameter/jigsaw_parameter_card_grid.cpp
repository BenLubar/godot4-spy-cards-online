#include "jigsaw/parameter/jigsaw_parameter_card_grid.h"

void JigsawParameterCardGrid::_bind_methods() {
	BIND_PROPERTY(Variant::INT, grid);
	BIND_PROPERTY(Variant::BOOL, is_3d);
}

IMPLEMENT_PROPERTY(JigsawParameterCardGrid, int64_t, grid);
IMPLEMENT_PROPERTY(JigsawParameterCardGrid, bool, is_3d);
