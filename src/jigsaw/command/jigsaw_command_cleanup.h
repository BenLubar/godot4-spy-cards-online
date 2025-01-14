#ifndef JIGSAW_COMMAND_CLEANUP_H
#define JIGSAW_COMMAND_CLEANUP_H

#include "jigsaw/command/jigsaw_command.h"

class JigsawCommandCleanup : public JigsawCommand {
	GDCLASS(JigsawCommandCleanup, JigsawCommand);

protected:
	static void _bind_methods();

public:
	enum Operation {
		GC_CARD_INSTANCES = 0,
	};

	DECLARE_PROPERTY(Operation, operation, = GC_CARD_INSTANCES);

	Type get_type() const override;
	bool allowed_in_mode(enums::JigsawProcedure::Mode mode, bool any_config) const override;
	JigsawExecutionState evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const override;

	int64_t get_num_configs() const override;
	String get_config_name(int64_t i) const override;
	String get_config_desc(int64_t i) const override;
	int64_t get_config_value(int64_t i) const override;
	void set_config_value(int64_t i, int64_t value) override;
	PackedStringArray get_config_options(int64_t i) const override;

	int64_t get_num_arguments() const override { return 0; }
	Ref<JigsawParameter> get_argument(int64_t i) const override { ERR_FAIL_V(Ref<JigsawParameter>()); }
	TypedArray<JigsawParameter> get_argument_template(int64_t i, const Ref<JigsawContext> &context) const override { ERR_FAIL_V(TypedArray<JigsawParameter>()); }
	void set_argument(int64_t i, const Ref<JigsawParameter> &arg) override { ERR_FAIL(); }
	String get_argument_name(int64_t i) const override { ERR_FAIL_V(String()); }

	JIGSAW_COMMAND_NO_RESULTS();
};
DECLARE_ENUM(JigsawCommandCleanup::Operation);

#endif // JIGSAW_COMMAND_CLEANUP_H
