#ifndef JIGSAW_COMMAND_ORDERED_LIST_H
#define JIGSAW_COMMAND_ORDERED_LIST_H

#include "jigsaw_command.h"

class JigsawCommandOrderedList : public JigsawCommand {
	GDCLASS(JigsawCommandOrderedList, JigsawCommand);

protected:
	static void _bind_methods();

public:
	enum Operation {
		GET_NUM_ITEMS = 0,
		GET_ITEM_AT_INDEX = 1,
	};

	DECLARE_PROPERTY(Operation, operation, = GET_NUM_ITEMS);
	DECLARE_PROPERTY(Ref<JigsawParameter>, list);
	DECLARE_PROPERTY(Ref<JigsawParameter>, index);
	DECLARE_PROPERTY(Ref<JigsawParameterLocalVariable>, output);

	Type get_type() const override { return ORDERED_LIST; }
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
	TypedArray<JigsawParameter> get_branch_argument_templates(int64_t i) const override;
	PackedStringArray get_branch_argument_names(int64_t i) const override;
	TypedArray<Array> get_branch_result_templates(int64_t i, const Ref<JigsawContext> &context) const override;
	PackedStringArray get_branch_result_names(int64_t i) const override;
};

DECLARE_ENUM(JigsawCommandOrderedList::Operation);

#endif // JIGSAW_COMMAND_ORDERED_LIST_H
