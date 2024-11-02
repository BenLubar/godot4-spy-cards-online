#ifndef JIGSAW_COMMAND_IF_H
#define JIGSAW_COMMAND_IF_H

#include "jigsaw_command.h"

class JigsawCommandIf : public JigsawCommand {
	GDCLASS(JigsawCommandIf, JigsawCommand);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(Ref<JigsawParameter>, condition); // boolean
	DECLARE_PROPERTY(Ref<JigsawCommandList>, if_true);
	DECLARE_PROPERTY(Ref<JigsawCommandList>, if_false);

	Type get_type() const override { return IF; }
	bool modifies_game_state() const override { return false; }
	bool can_pause_execution() const override { return false; }

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
};

#endif // JIGSAW_COMMAND_IF_H
