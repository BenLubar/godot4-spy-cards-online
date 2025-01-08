#ifndef JIGSAW_COMMAND_SCENE_H
#define JIGSAW_COMMAND_SCENE_H

#include "jigsaw/command/jigsaw_command.h"

class JigsawCommandScene : public JigsawCommand {
	GDCLASS(JigsawCommandScene, JigsawCommand);

public:
	enum Operation {
		GET_PROPERTY_STRING = 0,
		GET_PROPERTY_FLOAT = 1,
	};

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(Operation, operation, = GET_PROPERTY_STRING);
	DECLARE_PROPERTY(Ref<JigsawParameter>, scene);
	DECLARE_PROPERTY(Ref<JigsawParameter>, property_name);
	DECLARE_PROPERTY(Ref<JigsawParameter>, default_value);
	DECLARE_PROPERTY(Ref<JigsawParameterLocalVariable>, output);

	Type get_type() const override { return SCENE; }
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
	void set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) override;
	String get_result_name(int64_t i) const override;
};
DECLARE_ENUM(JigsawCommandScene::Operation);

#endif // JIGSAW_COMMAND_SCENE_H
