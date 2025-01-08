#ifndef JIGSAW_COMMAND_AUDIENCE_H
#define JIGSAW_COMMAND_AUDIENCE_H

#include "jigsaw/command/jigsaw_command.h"

class JigsawCommandAudience : public JigsawCommand {
	GDCLASS(JigsawCommandAudience, JigsawCommand);

public:
	enum Operation {
		CREATE = 0,
		GET_BY_WEIGHT = 1,
		GET_TOTAL_WEIGHT = 2,
	};

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(Operation, operation, = CREATE);
	DECLARE_PROPERTY(Ref<JigsawParameter>, member);
	DECLARE_PROPERTY(Ref<JigsawParameter>, x);
	DECLARE_PROPERTY(Ref<JigsawParameter>, y);
	DECLARE_PROPERTY(Ref<JigsawParameter>, z);
	DECLARE_PROPERTY(Ref<JigsawParameter>, back);
	DECLARE_PROPERTY(Ref<JigsawParameter>, flip);
	DECLARE_PROPERTY(Ref<JigsawParameter>, left);
	DECLARE_PROPERTY(Ref<JigsawParameter>, color);
	DECLARE_PROPERTY(Ref<JigsawParameter>, excitement);
	DECLARE_PROPERTY(Ref<JigsawParameter>, weight);
	DECLARE_PROPERTY(Ref<JigsawParameterLocalVariable>, output);

	Type get_type() const override { return AUDIENCE; }
	bool allowed_in_mode(enums::JigsawProcedure::Mode mode, bool any_config) const override;
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
	bool is_result_required(int64_t i) const override;
	void set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) override;
	String get_result_name(int64_t i) const override;
};
DECLARE_ENUM(JigsawCommandAudience::Operation);

#endif // JIGSAW_COMMAND_AUDIENCE_H
