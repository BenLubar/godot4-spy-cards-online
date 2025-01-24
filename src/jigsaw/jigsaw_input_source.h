#ifndef JIGSAW_INPUT_SOURCE_H
#define JIGSAW_INPUT_SOURCE_H

#include "protocol/button_input_history.h"
#include "jigsaw/jigsaw_error.h"

#include <godot_cpp/classes/node.hpp>

class JigsawInputSource : public Node {
	GDCLASS(JigsawInputSource, Node);

protected:
	static void _bind_methods();

public:
	virtual BitField<ButtonInputHistory::InputButton> get_player_realtime_inputs(int32_t side, int64_t frame) const = 0;
	virtual void update_player_realtime_inputs(int64_t frame) = 0;
	virtual void on_jigsaw_error(const Ref<JigsawError> &err) = 0;
};

#endif // JIGSAW_INPUT_SOURCE
