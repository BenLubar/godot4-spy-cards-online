#ifndef JIGSAW_COMMAND_H
#define JIGSAW_COMMAND_H

#include "dry.h"

class JigsawCommand;

#include "jigsaw/procedure/jigsaw_command_list.h"
#include "jigsaw/jigsaw_context.h"
#include "jigsaw/jigsaw_error.h"
#include "jigsaw/parameter/jigsaw_parameter.h"
#include "jigsaw/parameter/jigsaw_parameter_local_variable.h"
#include "jigsaw/procedure/jigsaw_procedure.h"

class JigsawCommand : public Resource {
	GDCLASS(JigsawCommand, Resource);

public:
	enum Type {
		CRASH = 0,
		COMMENT = 1,
		FUNCTION = 2,
		MATH = 3,
		LOG = 4,
		FORMAT_TEXT = 5,
		IF = 6,
		SET_VARIABLE = 7,
		LOOKUP_DEFINITION_PROPERTY = 8,
		IS_SAME = 9,
		ORDERED_LIST = 10,
		RNG = 11,
		AUDIENCE = 12,
		CHARACTER = 13,
		AUDIO = 14,
		SCENE = 15,
		SPRITE = 16,
		CAMERA = 17,
		UI = 18,
		LOOP = 19,
		COLOR = 20,
	};

protected:
	static void _bind_methods();

	JigsawExecutionState set_command_result(const Ref<JigsawContext> &context, Ref<JigsawError> &err, int64_t i, const Ref<JigsawParameter> &value) const;

public:
	virtual Type get_type() const = 0;
	virtual bool modifies_game_state() const = 0;
	virtual bool can_pause_execution() const = 0;
	virtual JigsawExecutionState evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const = 0;
	virtual Ref<JigsawError> pop_stack_frame(const Ref<JigsawContext> &context, const Ref<JigsawStackFrame> &popped_frame) const { return Ref<JigsawError>(); }

	virtual int64_t get_num_configs() const { return 0; }
	virtual String get_config_name(int64_t i) const { return ""; }
	virtual String get_config_desc(int64_t i) const { return ""; }
	virtual int64_t get_config_value(int64_t i) const { return 0; }
	virtual void set_config_value(int64_t i, int64_t value) {}
	virtual PackedStringArray get_config_options(int64_t i) const { return PackedStringArray(); }

	virtual int64_t get_num_arguments() const = 0;
	virtual Ref<JigsawParameter> get_argument(int64_t i) const = 0;
	virtual TypedArray<JigsawParameter> get_argument_template(int64_t i, const Ref<JigsawContext> &context) const = 0;
	virtual bool is_argument_required(int64_t i) const { return true; }
	virtual void set_argument(int64_t i, const Ref<JigsawParameter> &arg) = 0;
	virtual String get_argument_name(int64_t i) const = 0;

	virtual int64_t get_num_results() const = 0;
	virtual Ref<JigsawParameterLocalVariable> get_result(int64_t i) const = 0;
	virtual TypedArray<JigsawParameter> get_result_template(int64_t i, const Ref<JigsawContext> &context) const = 0;
	virtual bool is_result_required(int64_t i) const { return true; }
	virtual void set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) = 0;
	virtual String get_result_name(int64_t i) const = 0;

	virtual int64_t get_num_branches() const { return 0; }
	virtual Ref<JigsawCommandList> get_branch(int64_t i) const { return Ref<JigsawCommandList>(); }
	virtual void set_branch(int64_t i, const Ref<JigsawCommandList> &commands) {}
	virtual String get_branch_name(int64_t i) const { return ""; }
	virtual TypedArray<JigsawParameter> get_branch_argument_templates(int64_t i) const { return TypedArray<JigsawParameter>(); }
	virtual PackedStringArray get_branch_argument_names(int64_t i) const { return PackedStringArray(); }
	virtual TypedArray<Array> get_branch_result_templates(int64_t i, const Ref<JigsawContext> &context) const { return TypedArray<Array>(); }
	virtual PackedStringArray get_branch_result_names(int64_t i) const { return PackedStringArray(); }

	DEFAULT_TO_STRING();
};
DECLARE_ENUM(JigsawCommand::Type);

#define JIGSAW_COMMAND_NO_RESULTS() \
	int64_t get_num_results() const override { return 0; } \
	Ref<JigsawParameterLocalVariable> get_result(int64_t i) const override { return Ref<JigsawParameterLocalVariable>(); } \
	TypedArray<JigsawParameter> get_result_template(int64_t i, const Ref<JigsawContext> &context) const override { return TypedArray<JigsawParameter>(); } \
	void set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) override {} \
	String get_result_name(int64_t i) const override { return ""; }

#endif // JIGSAW_COMMAND_H
