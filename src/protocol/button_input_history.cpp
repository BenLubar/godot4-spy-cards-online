#include "protocol/button_input_history.h"

#include <godot_cpp/classes/input.hpp>

void ButtonInputHistory::_bind_methods() {
	BIND_BITFIELD_FLAG(UP);
	BIND_BITFIELD_FLAG(DOWN);
	BIND_BITFIELD_FLAG(LEFT);
	BIND_BITFIELD_FLAG(RIGHT);
	BIND_BITFIELD_FLAG(CONFIRM);
	BIND_BITFIELD_FLAG(CANCEL);
	BIND_BITFIELD_FLAG(SWITCH);
	BIND_BITFIELD_FLAG(TOGGLE);

	ClassDB::bind_static_method("ButtonInputHistory", D_METHOD("get_current_inputs"), &ButtonInputHistory::get_current_inputs);
	ClassDB::bind_static_method("ButtonInputHistory", D_METHOD("pack_inputs", "unpacked_inputs"), &ButtonInputHistory::pack_inputs);
	ClassDB::bind_static_method("ButtonInputHistory", D_METHOD("unpack_inputs", "packed_inputs"), &ButtonInputHistory::unpack_inputs);
}

BitField<ButtonInputHistory::InputButton> ButtonInputHistory::get_current_inputs() {
	BitField<ButtonInputHistory::InputButton> buttons = 0;

	Input *input = Input::get_singleton();
	if (input->is_action_pressed("button_up")) {
		buttons.set_flag(UP);
	}
	if (input->is_action_pressed("button_down")) {
		buttons.set_flag(DOWN);
	}
	if (input->is_action_pressed("button_left")) {
		buttons.set_flag(LEFT);
	}
	if (input->is_action_pressed("button_right")) {
		buttons.set_flag(RIGHT);
	}
	if (input->is_action_pressed("button_confirm")) {
		buttons.set_flag(CONFIRM);
	}
	if (input->is_action_pressed("button_cancel")) {
		buttons.set_flag(CANCEL);
	}
	if (input->is_action_pressed("button_switch")) {
		buttons.set_flag(SWITCH);
	}
	if (input->is_action_pressed("button_toggle")) {
		buttons.set_flag(TOGGLE);
	}

	return buttons;
}

// packed inputs are run-length encoded with the following 2 byte pattern:
// - 8 bits: buttons currently being held
// - 8 bits: 1 less than the number of frames this input is held for
// this means that every 2 bytes can encode between 1 and 256 frames of input.
//
// unpacked inputs are simply the 8 bit bitfield

PackedByteArray ButtonInputHistory::pack_inputs(const PackedByteArray &unpacked_inputs) {
	PackedByteArray packed_inputs;

	for (int64_t i = 0; i < unpacked_inputs.size(); ) {
		uint32_t current_run = 1;
		for (int64_t j = i + 1; j < unpacked_inputs.size() && unpacked_inputs[i] == unpacked_inputs[j] && current_run < 256; j++);

		packed_inputs.append(unpacked_inputs[i] & 0xff);
		packed_inputs.append(current_run - 1);

		i += current_run;
	}

	return packed_inputs;
}
PackedByteArray ButtonInputHistory::unpack_inputs(const PackedByteArray &packed_inputs) {
	ERR_FAIL_COND_V(packed_inputs.size() % 2 != 0, PackedByteArray());

	PackedByteArray unpacked_inputs;
	for (int64_t i = 0; i < packed_inputs.size(); i += 2) {
		uint8_t input = packed_inputs[i];

		int count = (packed_inputs[i + 1] + 1);
		for (int j = 0; j < count; j++) {
			unpacked_inputs.append(input);
		}
	}

	return unpacked_inputs;
}
