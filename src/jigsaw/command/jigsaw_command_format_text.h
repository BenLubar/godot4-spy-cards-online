#ifndef JIGSAW_COMMAND_FORMAT_TEXT_H
#define JIGSAW_COMMAND_FORMAT_TEXT_H

#include "jigsaw/command/jigsaw_command.h"

class JigsawCommandFormatText;

#include "defs/formatted_text.h"

class JigsawCommandFormatText : public JigsawCommand {
	GDCLASS(JigsawCommandFormatText, JigsawCommand);

protected:
	static void _bind_methods();

public:
	// POP and PUSH_EFFECT_INSTANCE are disallowed
	DECLARE_PROPERTY(FormattedText::Command, command, = FormattedText::Command::ADD_TEXT);

	DECLARE_PROPERTY(Ref<JigsawParameter>, before); // formatted_text (optional)
	DECLARE_PROPERTY(Ref<JigsawParameter>, text); // string or formatted_text or amount (skipped for ADD_ICON and FORCE_END_OF_TEXT)
	DECLARE_PROPERTY(Ref<JigsawParameter>, after); // formatted_text (optional)

	// only for relevant commands
	DECLARE_PROPERTY(Ref<JigsawParameter>, table_columns); // amount
	DECLARE_PROPERTY(Ref<JigsawParameter>, color); // color
	DECLARE_PROPERTY(Ref<JigsawParameter>, icon); // icon
	DECLARE_PROPERTY(Ref<JigsawParameter>, font_size); // float

	DECLARE_PROPERTY(Ref<JigsawParameterLocalVariable>, formatted_text); // formatted_text

	Type get_type() const override { return FORMAT_TEXT; }
	bool modifies_game_state() const override { return false; }
	bool can_pause_execution() const override { return false; }
	JigsawExecutionState evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const override;

	int64_t get_num_configs() const override;
	String get_config_name(int64_t i) const override;
	String get_config_desc(int64_t i) const override;
	int64_t get_config_value(int64_t i) const override;
	void set_config_value(int64_t i, int64_t value) override;
	PackedStringArray get_config_options(int64_t i) const override;

	int64_t get_num_arguments() const override;
	Ref<JigsawParameter> get_argument(int64_t i) const override;
	TypedArray<JigsawParameter> get_argument_template(int64_t i, const Ref<JigsawContext> &context) const override;
	bool is_argument_required(int64_t i) const override;
	void set_argument(int64_t i, const Ref<JigsawParameter> &arg) override;
	String get_argument_name(int64_t i) const override;

	int64_t get_num_results() const override;
	Ref<JigsawParameterLocalVariable> get_result(int64_t i) const override;
	TypedArray<JigsawParameter> get_result_template(int64_t i, const Ref<JigsawContext> &context) const override;
	void set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) override;
	String get_result_name(int64_t i) const override;
};

#endif // JIGSAW_COMMAND_FORMAT_TEXT_H
