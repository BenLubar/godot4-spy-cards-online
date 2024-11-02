#ifndef JIGSAW_COMMAND_SET_VARIABLE_H
#define JIGSAW_COMMAND_SET_VARIABLE_H

#include "jigsaw_command.h"

class JigsawCommandSetVariable : public JigsawCommand {
	GDCLASS(JigsawCommandSetVariable, JigsawCommand);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY_IS(bool, persistent, = true);
	DECLARE_PROPERTY(Ref<JigsawParameter>, value);
	DECLARE_PROPERTY(Ref<JigsawParameter>, variable); // persistent ? variable : local_variable

	Type get_type() const override { return SET_VARIABLE; }
	bool modifies_game_state() const override { return _persistent; }
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
	bool is_argument_unresolved(int64_t i) const override;
	void set_argument(int64_t i, const Ref<JigsawParameter> &arg) override;
	String get_argument_name(int64_t i) const override;

	JIGSAW_COMMAND_NO_RESULTS();
};

#endif // JIGSAW_COMMAND_SET_VARIABLE_H
