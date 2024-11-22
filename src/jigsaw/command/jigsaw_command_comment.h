#ifndef JIGSAW_COMMAND_COMMENT_H
#define JIGSAW_COMMAND_COMMENT_H

#include "jigsaw/command/jigsaw_command.h"

class JigsawCommandComment : public JigsawCommand {
	GDCLASS(JigsawCommandComment, JigsawCommand);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(TypedArray<JigsawParameter>, params); // any; ignored
	DECLARE_PROPERTY(Ref<JigsawCommandList>, command_list); // optional; ignored

	Type get_type() const override { return COMMENT; }
	bool modifies_game_state() const override { return false; }
	bool can_pause_execution() const override { return false; }
	JigsawExecutionState evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const override;

	int64_t get_num_arguments() const override;
	Ref<JigsawParameter> get_argument(int64_t i) const override;
	TypedArray<JigsawParameter> get_argument_template(int64_t i, const Ref<JigsawContext> &context) const override;
	bool is_argument_required(int64_t i) const override { return false; }
	void set_argument(int64_t i, const Ref<JigsawParameter> &arg) override;
	String get_argument_name(int64_t i) const override;

	JIGSAW_COMMAND_NO_RESULTS();

	int64_t get_num_branches() const override { return 1; }
	Ref<JigsawCommandList> get_branch(int64_t i) const override {
		ERR_FAIL_INDEX_V(i, 1, Ref<JigsawCommandList>());

		return get_command_list();
	}
	void set_branch(int64_t i, const Ref<JigsawCommandList> &commands) override {
		ERR_FAIL_INDEX(i, 1);

		set_command_list(commands);
	}
};

#endif // JIGSAW_COMMAND_COMMENT_H
