#include "protocol/button_input_history.h"

#include "util/format_helper.h"

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
	BIND_BITFIELD_FLAG(PAUSE);
	BIND_BITFIELD_FLAG(HELP);

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
	if (input->is_action_pressed("button_pause")) {
		buttons.set_flag(PAUSE);
	}
	if (input->is_action_pressed("button_help")) {
		buttons.set_flag(HELP);
	}

	return buttons;
}

// packed inputs are run-length encoded with the following pattern:
// - uvarint: bitfield of input buttons being held
// - uvarint: one fewer than the number of frames this input is held for
//
// unpacked inputs are simply the 10 bit bitfield (bits 11-31 reserved, zero)

PackedByteArray ButtonInputHistory::pack_inputs(const PackedInt32Array &unpacked_inputs) {
	Ref<FormatHelper> packed_inputs = FormatHelper::write("inputs");

	for (int64_t i = 0; i < unpacked_inputs.size(); ) {
		int64_t current_run = 1;
		for (int64_t j = i + 1; j < unpacked_inputs.size() && unpacked_inputs[i] == unpacked_inputs[j]; j++);

		packed_inputs->write_uvarint(unpacked_inputs[i]);
		packed_inputs->write_uvarint(current_run - 1);

		i += current_run;
	}

	return packed_inputs->get_buffer();
}
PackedInt32Array ButtonInputHistory::unpack_inputs(const PackedByteArray &packed_inputs) {
	Ref<FormatHelper> fh = FormatHelper::read("inputs", packed_inputs);

	PackedInt32Array unpacked_inputs;
	while (!fh->is_eof()) {
		int32_t input = fh->read_uvarint();
		int64_t count = fh->read_uvarint() + 1;

		for (int64_t i = 0; i < count; i++) {
			unpacked_inputs.append(input);
		}
	}

	return unpacked_inputs;
}
