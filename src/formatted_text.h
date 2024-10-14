#ifndef FORMATTED_TEXT_H
#define FORMATTED_TEXT_H

#include "dry.h"

#include "enums_icon_def.h"

class FormattedText;
class FormattedTextWithIcon;

#include "effect_instance.h"

class FormattedText : public Resource {
	GDCLASS(FormattedText, Resource);

public:
	enum Command {
		ADD_TEXT = 0,
		ADD_ICON = 1,
		POP = 2,
		FORCE_END_OF_TEXT = 3,
		PUSH_EFFECT_INSTANCE = 4,
		PUSH_TEXT_COLOR = 5,
		PUSH_FG_COLOR = 6,
		PUSH_BG_COLOR = 7,
		PUSH_FONT_NORMAL = 8,
		PUSH_FONT_BOLD = 9,
		PUSH_FONT_ITALIC = 10,
		PUSH_FONT_BOLD_ITALIC = 11,
		PUSH_FONT_MONO = 12,
		PUSH_FONT_SIZE = 13,
		PUSH_STRIKETHROUGH = 14,
		PUSH_UNDERLINE = 15,
		PUSH_ALIGN_LEFT = 16,
		PUSH_ALIGN_CENTER = 17,
		PUSH_ALIGN_RIGHT = 18,
		PUSH_ALIGN_FILL = 19,
		PUSH_TABLE = 20,
		PUSH_TABLE_CELL = 21,
		PUSH_LIST_NUMBERED = 22,
		PUSH_LIST_LETTERED_UPPER = 23,
		PUSH_LIST_LETTERED_LOWER = 24,
		PUSH_LIST_ROMAN_UPPER = 25,
		PUSH_LIST_ROMAN_LOWER = 26,
		PUSH_LIST_BULLETED = 27,
		PUSH_RAINBOW = 28,
		PUSH_SHAKE = 29,
		PUSH_GLITCH = 30,
	};

protected:
	static void _bind_methods();

public:
	FormattedText() = default;
	~FormattedText() = default;

	DECLARE_PROPERTY(Command, command, = ADD_TEXT);
	DECLARE_PROPERTY(String, text);
	DECLARE_PROPERTY(enums::IconDef::Icon, icon, = enums::IconDef::Icon::NONE);
	DECLARE_PROPERTY(Ref<EffectInstance>, instance);
	DECLARE_PROPERTY(Color, color);
	DECLARE_PROPERTY(double, font_size, = 1.0f);
	DECLARE_PROPERTY(int64_t, table_columns, = 0);

	static TypedArray<FormattedText> make_plain(String string);
};
DECLARE_ENUM(FormattedText::Command);

class FormattedTextWithIcon : public Resource {
	GDCLASS(FormattedTextWithIcon, Resource);

protected:
	static void _bind_methods();

public:
	FormattedTextWithIcon() = default;
	~FormattedTextWithIcon() = default;

	DECLARE_PROPERTY(enums::IconDef::Icon, icon, = enums::IconDef::Icon::NONE);
	DECLARE_PROPERTY(TypedArray<FormattedText>, text);
};

#endif // FORMATTED_TEXT_H
