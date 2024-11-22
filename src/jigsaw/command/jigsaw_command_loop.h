#ifndef JIGSAW_COMMAND_LOOP_H
#define JIGSAW_COMMAND_LOOP_H

#include "jigsaw/command/jigsaw_command.h"

class JigsawCommandLoop : public JigsawCommand {
	GDCLASS(JigsawCommandLoop, JigsawCommand);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(Ref<JigsawParameter>, count); // amount
	DECLARE_PROPERTY(Ref<JigsawCommandList>, body);

	Type get_type() const override { return LOOP; }
	bool modifies_game_state() const override { return false; }
	bool can_pause_execution() const override { return false; }
	JigsawExecutionState evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const override;
	Ref<JigsawError> pop_stack_frame(const Ref<JigsawContext> &context, const Ref<JigsawStackFrame> &popped_frame) const override;

	int64_t get_num_arguments() const override;
	Ref<JigsawParameter> get_argument(int64_t i) const override;
	TypedArray<JigsawParameter> get_argument_template(int64_t i, const Ref<JigsawContext> &context) const override;
	void set_argument(int64_t i, const Ref<JigsawParameter> &arg) override;
	String get_argument_name(int64_t i) const override;

	JIGSAW_COMMAND_NO_RESULTS();

	int64_t get_num_branches() const override;
	Ref<JigsawCommandList> get_branch(int64_t i) const override;
	void set_branch(int64_t i, const Ref<JigsawCommandList> &commands) override;
	String get_branch_name(int64_t i) const override;
	TypedArray<JigsawParameter> get_branch_argument_templates(int64_t i) const override;
	PackedStringArray get_branch_argument_names(int64_t i) const override;
};

#endif // JIGSAW_COMMAND_LOOP_H
