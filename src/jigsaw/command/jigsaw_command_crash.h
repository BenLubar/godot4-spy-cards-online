#ifndef JIGSAW_COMMAND_CRASH_H
#define JIGSAW_COMMAND_CRASH_H

#include "jigsaw/command/jigsaw_command.h"

class JigsawCommandCrash : public JigsawCommand {
	GDCLASS(JigsawCommandCrash, JigsawCommand);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(Ref<JigsawParameter>, message); // string
	DECLARE_PROPERTY(Ref<JigsawParameter>, params); // ordered_list

	Type get_type() const override { return CRASH; }
	bool modifies_game_state() const override { return false; }
	bool can_pause_execution() const override { return false; }
	JigsawExecutionState evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const override;

	int64_t get_num_arguments() const override;
	Ref<JigsawParameter> get_argument(int64_t i) const override;
	TypedArray<JigsawParameter> get_argument_template(int64_t i, const Ref<JigsawContext> &context) const override;
	bool is_argument_required(int64_t i) const override;
	void set_argument(int64_t i, const Ref<JigsawParameter> &arg) override;
	String get_argument_name(int64_t i) const override;

	JIGSAW_COMMAND_NO_RESULTS();
};

#endif // JIGSAW_COMMAND_CRASH_H
