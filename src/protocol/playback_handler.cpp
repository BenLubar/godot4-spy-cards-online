#include "protocol/playback_handler.h"

void PlaybackHandler::_bind_methods() {
	BIND_PROPERTY_RESOURCE(DataContainer, recording);
}

IMPLEMENT_PROPERTY_SIMPLE(PlaybackHandler, Ref<DataContainer>, recording);

BitField<ButtonInputHistory::InputButton> PlaybackHandler::get_player_realtime_inputs(int32_t side, int64_t frame) const {
	ERR_FAIL_V_MSG(0, "TODO: PlaybackHandler::get_player_realtime_inputs");
}
