#ifndef PLAYBACK_HANDLER_H
#define PLAYBACK_HANDLER_H

#include "jigsaw/jigsaw_input_source.h"
#include "protocol/data_container.h"

class JigsawGlobal;

class PlaybackHandler : public JigsawInputSource {
	GDCLASS(PlaybackHandler, JigsawInputSource);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(Ref<DataContainer>, recording);
	DECLARE_PROPERTY(JigsawGlobal *, global, = nullptr);

	void init_playback();

	BitField<ButtonInputHistory::InputButton> get_player_realtime_inputs(int32_t side, int64_t frame) const override;
	void update_player_realtime_inputs(int64_t frame) override {}
	void on_jigsaw_error(const Ref<JigsawError> &err) override;
};

#endif // PLAYBACK_HANDLER_H
