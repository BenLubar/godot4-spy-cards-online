#include "formatted_text.h"

void FormattedText::_bind_methods() {
	BIND_ENUM_CONSTANT(ADD_TEXT);
	BIND_ENUM_CONSTANT(ADD_ICON);
	BIND_ENUM_CONSTANT(POP);
	BIND_ENUM_CONSTANT(FORCE_END_OF_TEXT);
	BIND_ENUM_CONSTANT(PUSH_EFFECT_INSTANCE);
	BIND_ENUM_CONSTANT(PUSH_TEXT_COLOR);
	BIND_ENUM_CONSTANT(PUSH_FG_COLOR);
	BIND_ENUM_CONSTANT(PUSH_BG_COLOR);
	BIND_ENUM_CONSTANT(PUSH_FONT_NORMAL);
	BIND_ENUM_CONSTANT(PUSH_FONT_BOLD);
	BIND_ENUM_CONSTANT(PUSH_FONT_ITALIC);
	BIND_ENUM_CONSTANT(PUSH_FONT_BOLD_ITALIC);
	BIND_ENUM_CONSTANT(PUSH_FONT_MONO);
	BIND_ENUM_CONSTANT(PUSH_FONT_SIZE);
	BIND_ENUM_CONSTANT(PUSH_STRIKETHROUGH);
	BIND_ENUM_CONSTANT(PUSH_UNDERLINE);
	BIND_ENUM_CONSTANT(PUSH_ALIGN_LEFT);
	BIND_ENUM_CONSTANT(PUSH_ALIGN_CENTER);
	BIND_ENUM_CONSTANT(PUSH_ALIGN_RIGHT);
	BIND_ENUM_CONSTANT(PUSH_ALIGN_FILL);
	BIND_ENUM_CONSTANT(PUSH_TABLE);
	BIND_ENUM_CONSTANT(PUSH_TABLE_CELL);
	BIND_ENUM_CONSTANT(PUSH_LIST_NUMBERED);
	BIND_ENUM_CONSTANT(PUSH_LIST_LETTERED_UPPER);
	BIND_ENUM_CONSTANT(PUSH_LIST_LETTERED_LOWER);
	BIND_ENUM_CONSTANT(PUSH_LIST_ROMAN_UPPER);
	BIND_ENUM_CONSTANT(PUSH_LIST_ROMAN_LOWER);
	BIND_ENUM_CONSTANT(PUSH_LIST_BULLETED);
	BIND_ENUM_CONSTANT(PUSH_RAINBOW);
	BIND_ENUM_CONSTANT(PUSH_SHAKE);
	BIND_ENUM_CONSTANT(PUSH_GLITCH);

	BIND_PROPERTY(Variant::INT, command);
	BIND_PROPERTY(Variant::STRING, text);
	BIND_PROPERTY(Variant::INT, icon);
	BIND_PROPERTY(Variant::OBJECT, instance, PROPERTY_HINT_RESOURCE_TYPE, "EffectInstance");
	BIND_PROPERTY(Variant::COLOR, color);
	BIND_PROPERTY(Variant::FLOAT, font_size);
	BIND_PROPERTY(Variant::INT, table_columns);

	ClassDB::bind_static_method("FormattedText", D_METHOD("make_plain", "string"), &FormattedText::make_plain);
}
FormattedText::FormattedText() {}
FormattedText::~FormattedText() {}

IMPLEMENT_PROPERTY(FormattedText, FormattedText::Command, command);
IMPLEMENT_PROPERTY(FormattedText, String, text);
IMPLEMENT_PROPERTY(FormattedText, IconDef::Icon, icon);
IMPLEMENT_PROPERTY(FormattedText, Ref<EffectInstance>, instance);
IMPLEMENT_PROPERTY(FormattedText, Color, color);
IMPLEMENT_PROPERTY(FormattedText, double, font_size);
IMPLEMENT_PROPERTY(FormattedText, int64_t, table_columns);

TypedArray<FormattedText> FormattedText::make_plain(String string) {
	FormattedText *ft = memnew(FormattedText);
	ft->set_command(ADD_TEXT);
	ft->set_text(string);
	return Array::make(ft);
}
