#ifndef JIGSAW_COMMAND_LOG_H
#define JIGSAW_COMMAND_LOG_H

#include "jigsaw/command/jigsaw_command.h"

class JigsawCommandLog : public JigsawCommand {
	GDCLASS(JigsawCommandLog, JigsawCommand);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(Ref<JigsawParameter>, message); // formatted_text

	Type get_type() const override { return LOG; }
	bool allowed_in_mode(enums::JigsawProcedure::Mode mode, bool any_config) const override;
	JigsawExecutionState evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const override;

	int64_t get_num_arguments() const override;
	Ref<JigsawParameter> get_argument(int64_t i) const override;
	TypedArray<JigsawParameter> get_argument_template(int64_t i, const Ref<JigsawContext> &context) const override;
	void set_argument(int64_t i, const Ref<JigsawParameter> &arg) override;
	String get_argument_name(int64_t i) const override;

	JIGSAW_COMMAND_NO_RESULTS();
};

#endif // JIGSAW_COMMAND_LOG_H
