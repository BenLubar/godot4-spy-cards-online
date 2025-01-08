#include "jigsaw/command/jigsaw_command.h"

void JigsawCommand::_bind_methods() {
	BIND_ENUM_CONSTANT(NONE);
	BIND_ENUM_CONSTANT(CRASH);
	BIND_ENUM_CONSTANT(COMMENT);
	BIND_ENUM_CONSTANT(FUNCTION);
	BIND_ENUM_CONSTANT(MATH);
	BIND_ENUM_CONSTANT(LOG);
	BIND_ENUM_CONSTANT(FORMAT_TEXT);
	BIND_ENUM_CONSTANT(IF);
	BIND_ENUM_CONSTANT(SET_VARIABLE);
	BIND_ENUM_CONSTANT(LOOKUP_DEFINITION_PROPERTY);
	BIND_ENUM_CONSTANT(IS_SAME);
	BIND_ENUM_CONSTANT(ORDERED_LIST);
	BIND_ENUM_CONSTANT(RNG);
	BIND_ENUM_CONSTANT(AUDIENCE);
	BIND_ENUM_CONSTANT(CHARACTER);
	BIND_ENUM_CONSTANT(AUDIO);
	BIND_ENUM_CONSTANT(SCENE);
	BIND_ENUM_CONSTANT(SPRITE);
	BIND_ENUM_CONSTANT(CAMERA);
	BIND_ENUM_CONSTANT(UI);
	BIND_ENUM_CONSTANT(LOOP);
	BIND_ENUM_CONSTANT(COLOR);

	ClassDB::bind_method(D_METHOD("get_type"), &JigsawCommand::get_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "type"), "", "get_type");
	ClassDB::bind_method(D_METHOD("allowed_in_mode", "mode", "any_config"), &JigsawCommand::allowed_in_mode, DEFVAL(false));

	ClassDB::bind_method(D_METHOD("get_num_configs"), &JigsawCommand::get_num_configs);
	ClassDB::bind_method(D_METHOD("get_config_name", "i"), &JigsawCommand::get_config_name);
	ClassDB::bind_method(D_METHOD("get_config_desc", "i"), &JigsawCommand::get_config_desc);
	ClassDB::bind_method(D_METHOD("get_config_value", "i"), &JigsawCommand::get_config_value);
	ClassDB::bind_method(D_METHOD("set_config_value", "i", "value"), &JigsawCommand::set_config_value);
	ClassDB::bind_method(D_METHOD("get_config_options", "i"), &JigsawCommand::get_config_options);

	ClassDB::bind_method(D_METHOD("get_num_arguments"), &JigsawCommand::get_num_arguments);
	ClassDB::bind_method(D_METHOD("get_argument", "i"), &JigsawCommand::get_argument);
	ClassDB::bind_method(D_METHOD("get_argument_template", "i"), &JigsawCommand::get_argument_template);
	ClassDB::bind_method(D_METHOD("is_argument_required", "i"), &JigsawCommand::is_argument_required);
	ClassDB::bind_method(D_METHOD("set_argument", "i", "arg"), &JigsawCommand::set_argument);
	ClassDB::bind_method(D_METHOD("get_argument_name", "i"), &JigsawCommand::get_argument_name);

	ClassDB::bind_method(D_METHOD("get_num_results"), &JigsawCommand::get_num_results);
	ClassDB::bind_method(D_METHOD("get_result", "i"), &JigsawCommand::get_result);
	ClassDB::bind_method(D_METHOD("get_result_template", "i"), &JigsawCommand::get_result_template);
	ClassDB::bind_method(D_METHOD("is_result_required", "i"), &JigsawCommand::is_result_required);
	ClassDB::bind_method(D_METHOD("set_result", "i", "result"), &JigsawCommand::set_result);
	ClassDB::bind_method(D_METHOD("get_result_name", "i"), &JigsawCommand::get_result_name);

	ClassDB::bind_method(D_METHOD("get_num_branches"), &JigsawCommand::get_num_branches);
	ClassDB::bind_method(D_METHOD("get_branch", "i"), &JigsawCommand::get_branch);
	ClassDB::bind_method(D_METHOD("set_branch", "i", "commands"), &JigsawCommand::set_branch);
	ClassDB::bind_method(D_METHOD("get_branch_mode", "i", "parent_mode"), &JigsawCommand::get_branch_mode);
	ClassDB::bind_method(D_METHOD("get_branch_name", "i"), &JigsawCommand::get_branch_name);
	ClassDB::bind_method(D_METHOD("get_branch_argument_templates", "i"), &JigsawCommand::get_branch_argument_templates);
	ClassDB::bind_method(D_METHOD("get_branch_argument_names", "i"), &JigsawCommand::get_branch_argument_names);
	ClassDB::bind_method(D_METHOD("get_branch_result_templates", "i"), &JigsawCommand::get_branch_result_templates);
	ClassDB::bind_method(D_METHOD("get_branch_result_names", "i"), &JigsawCommand::get_branch_result_names);
}

JigsawExecutionState JigsawCommand::set_command_result(const Ref<JigsawContext> &context, Ref<JigsawError> &err, int64_t i, const Ref<JigsawParameter> &value) const {
	String result_name = get_result_name(i);

	Ref<JigsawParameterLocalVariable> var = get_result(i);
	if (var.is_null()) {
		if (is_result_required(i)) {
			err = context->create_error(vformat("missing required result variable %s", result_name));
			return JigsawExecutionState::ERROR;
		}

		return JigsawExecutionState::CONTINUE;
	}

	err = context->set_local_variable(var, value, result_name);
	return unlikely(err.is_valid()) ? JigsawExecutionState::ERROR : JigsawExecutionState::CONTINUE;
}
