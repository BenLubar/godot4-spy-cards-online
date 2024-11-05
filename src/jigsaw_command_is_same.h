#ifndef JIGSAW_COMMAND_IS_SAME_H
#define JIGSAW_COMMAND_IS_SAME_H

#include "jigsaw_command.h"

class JigsawCommandIsSame : public JigsawCommand {
	GDCLASS(JigsawCommandIsSame, JigsawCommand);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(Ref<JigsawParameter>, compare_to);
	DECLARE_PROPERTY(Ref<JigsawParameter>, object);
	DECLARE_PROPERTY(Ref<JigsawParameterLocalVariable>, is_same);

	Type get_type() const override { return IS_SAME; }
	bool modifies_game_state() const override { return false; }
	bool can_pause_execution() const override { return false; }
	JigsawExecutionState evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const override;

	int64_t get_num_arguments() const override;
	Ref<JigsawParameter> get_argument(int64_t i) const override;
	TypedArray<JigsawParameter> get_argument_template(int64_t i, const Ref<JigsawContext> &context) const override;
	void set_argument(int64_t i, const Ref<JigsawParameter> &arg) override;
	String get_argument_name(int64_t i) const override;

	int64_t get_num_results() const override;
	Ref<JigsawParameterLocalVariable> get_result(int64_t i) const override;
	TypedArray<JigsawParameter> get_result_template(int64_t i, const Ref<JigsawContext> &context) const override;
	void set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) override;
	String get_result_name(int64_t i) const override;
};

#endif // JIGSAW_COMMAND_IS_SAME_H
