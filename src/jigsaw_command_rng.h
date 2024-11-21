#ifndef JIGSAW_COMMAND_RNG_H
#define JIGSAW_COMMAND_RNG_H

#include "jigsaw_command.h"

class JigsawCommandRNG;

#include "rng.h"

class JigsawCommandRNG : public JigsawCommand {
	GDCLASS(JigsawCommandRNG, JigsawCommand);

protected:
	static void _bind_methods();

public:
	enum Operation {
		FLOAT = 0,
		AMOUNT = 1,
		SCOPE = 2,
	};

	DECLARE_PROPERTY(Operation, operation, = FLOAT);
	DECLARE_PROPERTY(Ref<JigsawParameter>, min);
	DECLARE_PROPERTY(Ref<JigsawParameter>, max);
	DECLARE_PROPERTY(Ref<JigsawParameterLocalVariable>, output);
	DECLARE_PROPERTY(Ref<JigsawCommandList>, scope);

	Type get_type() const override { return RNG; }
	bool modifies_game_state() const override { return true; }
	bool can_pause_execution() const override { return false; }
	JigsawExecutionState evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const override;
	Ref<JigsawError> pop_stack_frame(const Ref<JigsawContext> &context, const Ref<JigsawStackFrame> &popped_frame) const override;

	int64_t get_num_configs() const override;
	String get_config_name(int64_t i) const override;
	String get_config_desc(int64_t i) const override;
	int64_t get_config_value(int64_t i) const override;
	void set_config_value(int64_t i, int64_t value) override;
	PackedStringArray get_config_options(int64_t i) const override;

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

	int64_t get_num_branches() const override;
	Ref<JigsawCommandList> get_branch(int64_t i) const override;
	void set_branch(int64_t i, const Ref<JigsawCommandList> &commands) override;
	String get_branch_name(int64_t i) const override;
};
DECLARE_ENUM(JigsawCommandRNG::Operation);

#endif // JIGSAW_COMMAND_RNG_H
