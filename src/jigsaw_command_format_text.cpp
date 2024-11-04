#include "jigsaw_command_format_text.h"

#include "jigsaw_parameter_amount.h"
#include "jigsaw_parameter_color.h"
#include "jigsaw_parameter_float.h"
#include "jigsaw_parameter_formatted_text.h"
#include "jigsaw_parameter_icon.h"
#include "jigsaw_parameter_string.h"

void JigsawCommandFormatText::_bind_methods() {
	BIND_PROPERTY_ENUM(FormattedText::Command, command);

	BIND_PROPERTY_RESOURCE(JigsawParameter, before);
	BIND_PROPERTY_RESOURCE(JigsawParameter, text);
	BIND_PROPERTY_RESOURCE(JigsawParameter, after);

	BIND_PROPERTY_RESOURCE(JigsawParameter, table_columns);
	BIND_PROPERTY_RESOURCE(JigsawParameter, color);
	BIND_PROPERTY_RESOURCE(JigsawParameter, icon);
	BIND_PROPERTY_RESOURCE(JigsawParameter, font_size);

	BIND_PROPERTY_RESOURCE(JigsawParameterLocalVariable, formatted_text);
}

IMPLEMENT_PROPERTY(JigsawCommandFormatText, FormattedText::Command, command);

IMPLEMENT_PROPERTY(JigsawCommandFormatText, Ref<JigsawParameter>, before);
IMPLEMENT_PROPERTY(JigsawCommandFormatText, Ref<JigsawParameter>, text);
IMPLEMENT_PROPERTY(JigsawCommandFormatText, Ref<JigsawParameter>, after);

IMPLEMENT_PROPERTY(JigsawCommandFormatText, Ref<JigsawParameter>, table_columns);
IMPLEMENT_PROPERTY(JigsawCommandFormatText, Ref<JigsawParameter>, color);
IMPLEMENT_PROPERTY(JigsawCommandFormatText, Ref<JigsawParameter>, icon);
IMPLEMENT_PROPERTY(JigsawCommandFormatText, Ref<JigsawParameter>, font_size);

IMPLEMENT_PROPERTY(JigsawCommandFormatText, Ref<JigsawParameterLocalVariable>, formatted_text);

JigsawExecutionState JigsawCommandFormatText::evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const {
	err = context->create_error("internal error: TODO (format text)");

	return JigsawExecutionState::ERROR;
}

int64_t JigsawCommandFormatText::get_num_configs() const {
	return 1;
}
String JigsawCommandFormatText::get_config_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "Command";
}
String JigsawCommandFormatText::get_config_desc(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "";
}
int64_t JigsawCommandFormatText::get_config_value(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, 0);

	int64_t value = _command;

	static_assert(FormattedText::POP < FormattedText::PUSH_EFFECT_INSTANCE);
	if (value >= FormattedText::PUSH_EFFECT_INSTANCE) {
		value--;
	}
	if (value >= FormattedText::POP) {
		value--;
	}

	return value;
}
void JigsawCommandFormatText::set_config_value(int64_t i, int64_t value) {
	ERR_FAIL_INDEX(i, 1);

	static_assert(FormattedText::POP < FormattedText::PUSH_EFFECT_INSTANCE);
	if (value >= FormattedText::POP) {
		value++;
	}
	if (value >= FormattedText::PUSH_EFFECT_INSTANCE) {
		value++;
	}

	_command = static_cast<FormattedText::Command>(value);
	emit_changed();
}
PackedStringArray JigsawCommandFormatText::get_config_options(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, PackedStringArray());

	// if these asserts fail, this function needs updating (and existing custom modes are probably broken, so don't break this)
	static_assert(FormattedText::ADD_TEXT == 0);
	static_assert(FormattedText::ADD_ICON == 1);
	static_assert(FormattedText::POP == 2);
	static_assert(FormattedText::FORCE_END_OF_TEXT == 3);
	static_assert(FormattedText::PUSH_EFFECT_INSTANCE == 4);
	static_assert(FormattedText::PUSH_TEXT_COLOR == 5);
	static_assert(FormattedText::PUSH_FG_COLOR == 6);
	static_assert(FormattedText::PUSH_BG_COLOR == 7);
	static_assert(FormattedText::PUSH_FONT_NORMAL == 8);
	static_assert(FormattedText::PUSH_FONT_BOLD == 9);
	static_assert(FormattedText::PUSH_FONT_ITALIC == 10);
	static_assert(FormattedText::PUSH_FONT_BOLD_ITALIC == 11);
	static_assert(FormattedText::PUSH_FONT_MONO == 12);
	static_assert(FormattedText::PUSH_FONT_SIZE == 13);
	static_assert(FormattedText::PUSH_STRIKETHROUGH == 14);
	static_assert(FormattedText::PUSH_UNDERLINE == 15);
	static_assert(FormattedText::PUSH_ALIGN_LEFT == 16);
	static_assert(FormattedText::PUSH_ALIGN_CENTER == 17);
	static_assert(FormattedText::PUSH_ALIGN_RIGHT == 18);
	static_assert(FormattedText::PUSH_ALIGN_FILL == 19);
	static_assert(FormattedText::PUSH_TABLE == 20);
	static_assert(FormattedText::PUSH_TABLE_CELL == 21);
	static_assert(FormattedText::PUSH_LIST_NUMBERED == 22);
	static_assert(FormattedText::PUSH_LIST_LETTERED_UPPER == 23);
	static_assert(FormattedText::PUSH_LIST_LETTERED_LOWER == 24);
	static_assert(FormattedText::PUSH_LIST_ROMAN_UPPER == 25);
	static_assert(FormattedText::PUSH_LIST_ROMAN_LOWER == 26);
	static_assert(FormattedText::PUSH_LIST_BULLETED == 27);
	static_assert(FormattedText::PUSH_RAINBOW == 28);
	static_assert(FormattedText::PUSH_SHAKE == 29);
	static_assert(FormattedText::PUSH_GLITCH == 30);

	return PackedStringArray{
		"Plain text",
		"Icon",
		// POP is skipped
		"Force end of text",
		// PUSH_EFFECT_INSTANCE is skipped
		"Text color",
		"Foreground color",
		"Background color",
		"Normal font",
		"Bold font",
		"Italic font",
		"Bold italic font",
		"Monospace font",
		"Font size",
		"Strikethrough",
		"Underline",
		"Left-align",
		"Center-align",
		"Right-align",
		"Fill-align",
		"Table",
		"Table cell",
		"List (1, 2, 3)",
		"List (A, B, C)",
		"List (a, b, c)",
		"List (I, II, III)",
		"List (i, ii, iii)",
		"List (•)",
		"Animated effect: Rainbow",
		"Animated effect: Shake",
		"Animated effect: Glitch",
	};
}

int64_t JigsawCommandFormatText::get_num_arguments() const {
	switch (_command) {
	case FormattedText::ADD_TEXT:
		return 3;
	case FormattedText::ADD_ICON:
		return 3;
	case FormattedText::POP:
		// cannot happen
		return 0;
	case FormattedText::FORCE_END_OF_TEXT:
		return 1;
	case FormattedText::PUSH_EFFECT_INSTANCE:
		// cannot happen
		return 0;
	case FormattedText::PUSH_TEXT_COLOR:
	case FormattedText::PUSH_FG_COLOR:
	case FormattedText::PUSH_BG_COLOR:
		return 4;
	case FormattedText::PUSH_FONT_NORMAL:
	case FormattedText::PUSH_FONT_BOLD:
	case FormattedText::PUSH_FONT_ITALIC:
	case FormattedText::PUSH_FONT_BOLD_ITALIC:
	case FormattedText::PUSH_FONT_MONO:
		return 3;
	case FormattedText::PUSH_FONT_SIZE:
		return 4;
	case FormattedText::PUSH_STRIKETHROUGH:
	case FormattedText::PUSH_UNDERLINE:
		return 3;
	case FormattedText::PUSH_ALIGN_LEFT:
	case FormattedText::PUSH_ALIGN_CENTER:
	case FormattedText::PUSH_ALIGN_RIGHT:
	case FormattedText::PUSH_ALIGN_FILL:
		return 3;
	case FormattedText::PUSH_TABLE:
		return 4;
	case FormattedText::PUSH_TABLE_CELL:
		return 3;
	case FormattedText::PUSH_LIST_NUMBERED:
	case FormattedText::PUSH_LIST_LETTERED_UPPER:
	case FormattedText::PUSH_LIST_LETTERED_LOWER:
	case FormattedText::PUSH_LIST_ROMAN_UPPER:
	case FormattedText::PUSH_LIST_ROMAN_LOWER:
	case FormattedText::PUSH_LIST_BULLETED:
		return 3;
	case FormattedText::PUSH_RAINBOW:
	case FormattedText::PUSH_SHAKE:
	case FormattedText::PUSH_GLITCH:
		return 3;
	}

	return 0;
}
Ref<JigsawParameter> JigsawCommandFormatText::get_argument(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), Ref<JigsawParameter>());

	if (i == 0) {
		return _before;
	}

	if (i == 1) {
		if (_command == FormattedText::ADD_ICON) {
			return _icon;
		}

		return _text;
	}

	if (i == 2) {
		return _after;
	}

	if (_command == FormattedText::PUSH_FONT_SIZE) {
		return _font_size;
	}
	if (_command == FormattedText::PUSH_TABLE) {
		return _table_columns;
	}
	if (_command == FormattedText::PUSH_TEXT_COLOR || _command == FormattedText::PUSH_FG_COLOR || _command == FormattedText::PUSH_BG_COLOR) {
		return _color;
	}

	return Ref<JigsawParameter>();
}
TypedArray<JigsawParameter> JigsawCommandFormatText::get_argument_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), TypedArray<JigsawParameter>());

	if (i == 0) {
		return Array::make(JigsawParameterFormattedText::make(TypedArray<FormattedText>()));
	}

	if (i == 1) {
		if (_command == FormattedText::ADD_TEXT) {
			return Array::make(JigsawParameterString::make(""));
		}
		if (_command == FormattedText::ADD_ICON) {
			return Array::make(JigsawParameterIcon::make(enums::IconDef::NONE));
		}
		if (_command == FormattedText::PUSH_TABLE) {
			return Array::make(JigsawParameterFormattedText::make(TypedArray<FormattedText>()));
		}

		return Array::make(
			JigsawParameterFormattedText::make(TypedArray<FormattedText>()),
			JigsawParameterString::make("")
		);
	}

	if (i == 2) {
		return Array::make(JigsawParameterFormattedText::make(TypedArray<FormattedText>()));
	}

	if (_command == FormattedText::PUSH_FONT_SIZE) {
		return Array::make(JigsawParameterFloat::make(0.0));
	}
	if (_command == FormattedText::PUSH_TABLE) {
		return Array::make(JigsawParameterAmount::make(0));
	}
	if (_command == FormattedText::PUSH_TEXT_COLOR || _command == FormattedText::PUSH_FG_COLOR || _command == FormattedText::PUSH_BG_COLOR) {
		return Array::make(JigsawParameterColor::make(Color()));
	}

	return TypedArray<JigsawParameter>();
}
bool JigsawCommandFormatText::is_argument_required(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), false);

	if (i == 0) {
		// before is never required
		return false;
	}

	if (i == 1) {
		// only table cells may be implicitly empty
		return _command != FormattedText::PUSH_TABLE_CELL;
	}

	if (i == 2) {
		// after is never required
		return false;
	}

	if (i == 3) {
		// color/size is required on commands that use it
		return true;
	}

	return false;
}
void JigsawCommandFormatText::set_argument(int64_t i, const Ref<JigsawParameter> &arg) {
	ERR_FAIL_INDEX(i, get_num_arguments());

	if (i == 0) {
		_before = arg;
		emit_changed();
		return;
	}

	if (i == 1) {
		if (_command == FormattedText::ADD_ICON) {
			_icon = arg;
			emit_changed();
			return;
		}

		_text = arg;
		emit_changed();
		return;
	}

	if (i == 2) {
		_after = arg;
		emit_changed();
		return;
	}

	if (_command == FormattedText::PUSH_FONT_SIZE) {
		_font_size = arg;
		emit_changed();
		return;
	}
	if (_command == FormattedText::PUSH_TABLE) {
		_table_columns = arg;
		emit_changed();
		return;
	}
	if (_command == FormattedText::PUSH_TEXT_COLOR || _command == FormattedText::PUSH_FG_COLOR || _command == FormattedText::PUSH_BG_COLOR) {
		_color = arg;
		emit_changed();
		return;
	}
}
String JigsawCommandFormatText::get_argument_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), "");

	if (i == 0) {
		return "before";
	}

	if (i == 1) {
		if (_command == FormattedText::ADD_ICON) {
			return "icon";
		}
		if (_command == FormattedText::PUSH_TABLE) {
			return "cells";
		}

		return "text";
	}

	if (i == 2) {
		return "after";
	}

	if (_command == FormattedText::PUSH_FONT_SIZE) {
		return "font_size";
	}
	if (_command == FormattedText::PUSH_TABLE) {
		return "table_columns";
	}
	if (_command == FormattedText::PUSH_TEXT_COLOR || _command == FormattedText::PUSH_FG_COLOR || _command == FormattedText::PUSH_BG_COLOR) {
		return "color";
	}

	return "";
}

int64_t JigsawCommandFormatText::get_num_results() const {
	return 1;
}
Ref<JigsawParameterLocalVariable> JigsawCommandFormatText::get_result(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, Ref<JigsawParameterLocalVariable>());

	return _formatted_text;
}
TypedArray<JigsawParameter> JigsawCommandFormatText::get_result_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, 1, TypedArray<JigsawParameter>());

	return Array::make(JigsawParameterFormattedText::make(TypedArray<FormattedText>()));
}
void JigsawCommandFormatText::set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) {
	ERR_FAIL_INDEX(i, 1);

	_formatted_text = result;
	emit_changed();
}
String JigsawCommandFormatText::get_result_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "formatted_text";
}
