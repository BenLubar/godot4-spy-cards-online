#ifndef JIGSAW_COMMAND_MATH_H
#define JIGSAW_COMMAND_MATH_H

#include "jigsaw_command.h"

class JigsawCommandMath : public JigsawCommand {
	GDCLASS(JigsawCommandMath, JigsawCommand);

public:
	enum Operation {
		EQUALS = 0, // amount, amount -> boolean
		LESS_THAN = 1, // amount, amount -> boolean
		ADD = 2, // amount, amount -> amount
		SUBTRACT = 3, // amount, amount -> amount
		MULTIPLY = 4, // amount, amount -> amount
		DIVIDE = 5, // amount, amount -> amount
		REMAINDER = 6, // amount, amount -> amount
		AMOUNT_TO_FLOAT = 7, // amount -> float
		FLOOR = 8, // float -> amount
		CEIL = 9, // float -> amount
		FLOAT_LESS_THAN = 10, // float, float -> boolean
		FLOAT_ADD = 11, // float, float -> float
		FLOAT_SUBTRACT = 12, // float, float -> float
		FLOAT_MULTIPLY = 13, // float, float -> float
		FLOAT_DIVIDE = 14, // float, float -> float
		FLOAT_REMAINDER = 15, // float, float -> float
		POW = 16, // float, float -> float
		SIN = 17, // float -> float
		COS = 18, // float -> float
		TAN = 19, // float -> float
		ATAN = 20, // float -> float
		ATAN2 = 21, // float, float -> float
	};

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(Operation, op, = Operation::EQUALS);
	DECLARE_PROPERTY(Ref<JigsawParameter>, lhs); // see Operation enum
	DECLARE_PROPERTY(Ref<JigsawParameter>, rhs); // see Operation enum; for some operations, skipped
	DECLARE_PROPERTY(Ref<JigsawParameterLocalVariable>, output); // see Operation enum

	Type get_type() const override { return MATH; }
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
};
DECLARE_ENUM(JigsawCommandMath::Operation);

#endif // JIGSAW_COMMAND_MATH_H
