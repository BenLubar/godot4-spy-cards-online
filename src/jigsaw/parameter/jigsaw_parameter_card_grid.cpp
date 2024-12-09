#include "jigsaw/parameter/jigsaw_parameter_card_grid.h"

void JigsawParameterCardGrid::_bind_methods() {
	BIND_PROPERTY(Variant::INT, grid);
	BIND_PROPERTY(Variant::BOOL, is_3d);

	ClassDB::bind_static_method("JigsawParameterCardGrid", D_METHOD("make", "grid", "is_3d"), &JigsawParameterCardGrid::make);
}

IMPLEMENT_PROPERTY(JigsawParameterCardGrid, int64_t, grid);
IMPLEMENT_PROPERTY(JigsawParameterCardGrid, bool, is_3d);

Ref<JigsawParameterCardGrid> JigsawParameterCardGrid::make(int64_t grid, bool is_3d) {
	Ref<JigsawParameterCardGrid> param;
	param.instantiate();
	param->set_grid(grid);
	param->set_is_3d(is_3d);
	return param;
}
