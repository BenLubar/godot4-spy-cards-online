#include "jigsaw/jigsaw_input_source.h"

void JigsawInputSource::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_player_realtime_inputs", "side", "frame"), &JigsawInputSource::get_player_realtime_inputs);
	ClassDB::bind_method(D_METHOD("update_player_realtime_inputs", "frame"), &JigsawInputSource::update_player_realtime_inputs);
}
