#ifndef JIGSAW_COMMAND_MATH_H
#define JIGSAW_COMMAND_MATH_H

#include "jigsaw_command.h"

class JigsawCommandMath : public JigsawCommand {
	GDCLASS(JigsawCommandMath, JigsawCommand);

public:
	enum Operation {
		EQUALS = 0, // result is boolean
		LESS_THAN = 1, // result is boolean
	};

protected:
	static void _bind_methods();

private:
	TypedArray<JigsawParameter> get_numeric_template(int64_t i, const Ref<JigsawContext> &context) const;

public:
	DECLARE_PROPERTY(Operation, op, = Operation::EQUALS);
	DECLARE_PROPERTY(Ref<JigsawParameter>, lhs); // amount or float
	DECLARE_PROPERTY(Ref<JigsawParameter>, rhs); // same type as lhs; for some operations, skipped
	DECLARE_PROPERTY(Ref<JigsawParameterLocalVariable>, output); // see Operation enum

	Type get_type() const override { return MATH; }
	bool modifies_game_state() const override { return false; }
	bool can_pause_execution() const override { return false; }

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
};
DECLARE_ENUM(JigsawCommandMath::Operation);

#endif // JIGSAW_COMMAND_MATH_H
