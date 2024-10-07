#include "squish_label.h"

void SquishLabel::_bind_methods() {
	BIND_PROPERTY(Variant::VECTOR2, max_size);
	BIND_PROPERTY(Variant::VECTOR2, text_scale);
	BIND_PROPERTY_IS(Variant::BOOL, centered);

	ClassDB::bind_method(D_METHOD("set_formatted_text", "formatted_text", "card"), &SquishLabel::set_formatted_text);
}
SquishLabel::SquishLabel() {
	set_use_bbcode(true);
	set_fit_content(true);
	set_scroll_active(false);
	set_clip_contents(false);

	_outline_current_effect.instantiate();
	install_effect(_outline_current_effect);
	_rainbow_effect.instantiate();
	install_effect(_rainbow_effect);
	_shake_effect.instantiate();
	install_effect(_shake_effect);
	_glitch_effect.instantiate();
	install_effect(_glitch_effect);

	Variant resize_callable = callable_mp(this, &SquishLabel::_resize);
	connect("resized", resize_callable, CONNECT_DEFERRED);
	connect("finished", resize_callable, CONNECT_DEFERRED);
}
SquishLabel::~SquishLabel() {}

IMPLEMENT_PROPERTY_ONCHANGE(SquishLabel, Vector2, max_size, _resize());
IMPLEMENT_PROPERTY_ONCHANGE(SquishLabel, Vector2, text_scale, _resize());
IMPLEMENT_PROPERTY_ONCHANGE_IS(SquishLabel, bool, centered, _resize());

void SquishLabel::_resize() {
	Vector2 new_scale = get_scale();

	if (max_size.x != 0.0f && text_scale.x != 0.0f) {
		new_scale.x = Math::min(1.0f, max_size.x / text_scale.x / Math::max(get_content_width(), 1)) * text_scale.x;
	} else {
		new_scale.x = text_scale.x;
	}

	if (max_size.y != 0.0f && text_scale.y != 0.0f) {
		new_scale.y = Math::min(1.0f, max_size.y / text_scale.y / Math::max(get_content_height(), 1)) * text_scale.y;
		set_autowrap_mode(TextServer::AUTOWRAP_WORD_SMART);
	} else {
		new_scale.y = text_scale.y;
		set_autowrap_mode(TextServer::AUTOWRAP_OFF);
	}

	set_scale(new_scale);

	if (centered) {
		set_anchors_and_offsets_preset(PRESET_CENTER, PRESET_MODE_KEEP_SIZE);
	}
}

void SquishLabel::set_formatted_text(const TypedArray<FormattedText> &formatted_text, const Ref<CardInstance> &card) {
	clear();

	Ref<GameMode> mode;

	bool uses_outline_current = false;
	if (card.is_valid()) {
		JigsawGlobal *global = card->get_global();
		ERR_FAIL_NULL(global);
		mode = global->get_mode();
		ERR_FAIL_COND(mode.is_null());

		for (int64_t i = 0; i < formatted_text.size(); i++) {
			Ref<FormattedText> command = formatted_text[i];
			if (command.is_valid() && command->get_command() == FormattedText::PUSH_EFFECT_INSTANCE) {
				uses_outline_current = true;
				break;
			}
		}

		if (uses_outline_current) {
			push_outline_color(mode->get_visual_description_background_color());
			push_outline_size(mode->get_visual_effect_highlight_width());
		}
	}

	int32_t default_font_size = get_theme_font_size("normal_font_size");
	PackedInt32Array font_size_stack;
	font_size_stack.append(default_font_size);
	PackedInt32Array list_level_stack;
	list_level_stack.append(0);

	for (int64_t i = 0; i < formatted_text.size(); i++) {
		Ref<FormattedText> command = formatted_text[i];
		bool copy_font_size = true;
		bool copy_list_level = true;
		switch (command->get_command()) {
			case FormattedText::ADD_TEXT:
				copy_font_size = false;
				copy_list_level = false;
				{
					PackedStringArray lines = command->get_text().split("\n");
					for (int64_t j = 0; j < lines.size(); j++) {
						if (j) {
							newline();
						}
						add_text(lines[j]);
					}
				}
				break;
			case FormattedText::ADD_ICON:
				copy_font_size = false;
				copy_list_level = false;
				add_image(mode.is_valid() ? mode->get_icon_texture(command->get_icon()) : get_predefined<Texture2D>(command->get_icon()), 0, font_size_stack[font_size_stack.size() - 1]);
				break;
			case FormattedText::POP:
				copy_font_size = false;
				copy_list_level = false;
				font_size_stack.remove_at(font_size_stack.size() - 1);
				list_level_stack.remove_at(list_level_stack.size() - 1);
				pop();
				break;
			case FormattedText::FORCE_END_OF_TEXT:
				return;
			case FormattedText::PUSH_EFFECT_INSTANCE:
				{
					Dictionary env;
					env[String("card")] = card;
					env[String("effect")] = command->get_instance();
					push_customfx(_outline_current_effect, env);
				}
				break;
			case FormattedText::PUSH_TEXT_COLOR:
				push_color(command->get_color());
				break;
			case FormattedText::PUSH_FG_COLOR:
				push_fgcolor(command->get_color());
				break;
			case FormattedText::PUSH_BG_COLOR:
				push_bgcolor(command->get_color());
				break;
			case FormattedText::PUSH_FONT_NORMAL:
				push_normal();
				break;
			case FormattedText::PUSH_FONT_BOLD:
				push_bold();
				break;
			case FormattedText::PUSH_FONT_ITALIC:
				push_italics();
				break;
			case FormattedText::PUSH_FONT_BOLD_ITALIC:
				push_bold_italics();
				break;
			case FormattedText::PUSH_FONT_MONO:
				push_mono();
				break;
			case FormattedText::PUSH_FONT_SIZE:
				copy_font_size = false;
				font_size_stack.append(int32_t(Math::round(font_size_stack[font_size_stack.size() - 1] * command->get_font_size())));
				push_font_size(font_size_stack[font_size_stack.size() - 1]);
				break;
			case FormattedText::PUSH_STRIKETHROUGH:
				push_strikethrough();
				break;
			case FormattedText::PUSH_UNDERLINE:
				push_underline();
				break;
			case FormattedText::PUSH_ALIGN_LEFT:
				push_paragraph(HORIZONTAL_ALIGNMENT_LEFT);
				break;
			case FormattedText::PUSH_ALIGN_CENTER:
				push_paragraph(HORIZONTAL_ALIGNMENT_CENTER);
				break;
			case FormattedText::PUSH_ALIGN_RIGHT:
				push_paragraph(HORIZONTAL_ALIGNMENT_RIGHT);
				break;
			case FormattedText::PUSH_ALIGN_FILL:
				push_paragraph(HORIZONTAL_ALIGNMENT_FILL);
				break;
			case FormattedText::PUSH_TABLE:
				push_table(command->get_table_columns());
				break;
			case FormattedText::PUSH_TABLE_CELL:
				push_cell();
				break;
			case FormattedText::PUSH_LIST_NUMBERED:
				copy_list_level = false;
				list_level_stack.append(list_level_stack[list_level_stack.size() - 1] + 1);
				push_list(list_level_stack[list_level_stack.size() - 1], LIST_NUMBERS, false);
				break;
			case FormattedText::PUSH_LIST_LETTERED_UPPER:
				copy_list_level = false;
				list_level_stack.append(list_level_stack[list_level_stack.size() - 1] + 1);
				push_list(list_level_stack[list_level_stack.size() - 1], LIST_LETTERS, true);
				break;
			case FormattedText::PUSH_LIST_LETTERED_LOWER:
				copy_list_level = false;
				list_level_stack.append(list_level_stack[list_level_stack.size() - 1] + 1);
				push_list(list_level_stack[list_level_stack.size() - 1], LIST_LETTERS, false);
				break;
			case FormattedText::PUSH_LIST_ROMAN_UPPER:
				copy_list_level = false;
				list_level_stack.append(list_level_stack[list_level_stack.size() - 1] + 1);
				push_list(list_level_stack[list_level_stack.size() - 1], LIST_ROMAN, true);
				break;
			case FormattedText::PUSH_LIST_ROMAN_LOWER:
				copy_list_level = false;
				list_level_stack.append(list_level_stack[list_level_stack.size() - 1] + 1);
				push_list(list_level_stack[list_level_stack.size() - 1], LIST_ROMAN, false);
				break;
			case FormattedText::PUSH_LIST_BULLETED:
				copy_list_level = false;
				list_level_stack.append(list_level_stack[list_level_stack.size() - 1] + 1);
				push_list(list_level_stack[list_level_stack.size() - 1], LIST_DOTS, false);
				break;
			case FormattedText::PUSH_RAINBOW:
				push_customfx(_rainbow_effect, Dictionary());
				break;
			case FormattedText::PUSH_SHAKE:
				push_customfx(_shake_effect, Dictionary());
				break;
			case FormattedText::PUSH_GLITCH:
				push_customfx(_glitch_effect, Dictionary());
				break;
			default:
				ERR_FAIL_MSG(String("Unhandled formatting command ") + String::num(command->get_command()));
				break;
		}

		if (copy_font_size) {
			font_size_stack.append(font_size_stack[font_size_stack.size() - 1]);
		}
		if (copy_list_level) {
			list_level_stack.append(list_level_stack[list_level_stack.size() - 1]);
		}
	}

	if (uses_outline_current) {
		pop();
		pop();
	}
}
