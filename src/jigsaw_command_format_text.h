#ifndef JIGSAW_COMMAND_FORMAT_TEXT_H
#define JIGSAW_COMMAND_FORMAT_TEXT_H

#include "jigsaw_command.h"

class JigsawCommandFormatText;

#include "formatted_text.h"

class JigsawCommandFormatText : public JigsawCommand {
	GDCLASS(JigsawCommandFormatText, JigsawCommand);

protected:
	static void _bind_methods();

public:
	JigsawCommandFormatText() = default;
	~JigsawCommandFormatText() = default;

	// POP and PUSH_EFFECT_INSTANCE are disallowed
	DECLARE_PROPERTY(FormattedText::Command, command, = FormattedText::Command::ADD_TEXT);

	DECLARE_PROPERTY(Ref<JigsawParameter>, before); // formatted_text (optional)
	DECLARE_PROPERTY(Ref<JigsawParameter>, text); // string or formatted_text or amount (skipped for ADD_ICON and FORCE_END_OF_TEXT)
	DECLARE_PROPERTY(Ref<JigsawParameter>, after); // formatted_text (optional)

	// only for relevant commands
	DECLARE_PROPERTY(int64_t, table_columns, = 1);
	DECLARE_PROPERTY(Ref<JigsawParameter>, color); // color
	DECLARE_PROPERTY(Ref<JigsawParameter>, icon); // icon
	DECLARE_PROPERTY(Ref<JigsawParameter>, font_size); // float

	DECLARE_PROPERTY(Ref<JigsawParameterLocalVariable>, result); // formatted_text

	Type get_type() const override { return FORMAT_TEXT; }
};

#endif // JIGSAW_COMMAND_FORMAT_TEXT_H
