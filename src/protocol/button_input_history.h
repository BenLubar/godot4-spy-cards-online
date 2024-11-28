#ifndef BUTTON_INPUT_HISTORY_H
#define BUTTON_INPUT_HISTORY_H

#include "dry.h"

class ButtonInputHistory : public Object {
	GDCLASS(ButtonInputHistory, Object);

protected:
	static void _bind_methods();

public:
	enum InputButton {
		UP      = 0x01,
		DOWN    = 0x02,
		LEFT    = 0x04,
		RIGHT   = 0x08,
		CONFIRM = 0x10,
		CANCEL  = 0x20,
		SWITCH  = 0x40,
		TOGGLE  = 0x80,
	};

	static BitField<InputButton> get_current_inputs();
	static PackedByteArray pack_inputs(const PackedByteArray &unpacked_inputs);
	static PackedByteArray unpack_inputs(const PackedByteArray &packed_inputs);
};
VARIANT_BITFIELD_CAST(ButtonInputHistory::InputButton);

#endif // BUTTON_INPUT_HISTORY_H
