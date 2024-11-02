#ifndef JIGSAW_COMMAND_FUNCTION_H
#define JIGSAW_COMMAND_FUNCTION_H

#include "jigsaw_command.h"

class JigsawCommandFunction;

#include "jigsaw_global.h"

class JigsawCommandFunction : public JigsawCommand {
	GDCLASS(JigsawCommandFunction, JigsawCommand);

protected:
	static void _bind_methods();

private:
	Ref<JigsawFunction> get_function() const;

public:
	DECLARE_PROPERTY(JigsawGlobal *, editor_global, = nullptr);
	DECLARE_PROPERTY(int64_t, function_index, = -1);
	DECLARE_PROPERTY(TypedArray<JigsawParameter>, args);
	DECLARE_PROPERTY(TypedArray<JigsawParameterLocalVariable>, results);

	Type get_type() const override { return FUNCTION; }
	bool modifies_game_state() const override;
	bool can_pause_execution() const override;

	int64_t get_num_configs() const override;
	String get_config_name(int64_t i) const override;
	String get_config_desc(int64_t i) const override;
	int64_t get_config_value(int64_t i) const override;
	void set_config_value(int64_t i, int64_t value) override;
	PackedStringArray get_config_options(int64_t i) const override;

	int64_t get_num_arguments() const override;
	Ref<JigsawParameter> get_argument(int64_t i) const override;
	TypedArray<JigsawParameter> get_argument_template(int64_t i, const Ref<JigsawContext> &context) const override;
	bool is_argument_required(int64_t i) const override { return false; }
	void set_argument(int64_t i, const Ref<JigsawParameter> &arg) override;
	String get_argument_name(int64_t i) const override;

	int64_t get_num_results() const override;
	Ref<JigsawParameterLocalVariable> get_result(int64_t i) const override;
	TypedArray<JigsawParameter> get_result_template(int64_t i, const Ref<JigsawContext> &context) const override;
	void set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) override;
	String get_result_name(int64_t i) const override;
};

#endif // JIGSAW_COMMAND_FUNCTION_H
