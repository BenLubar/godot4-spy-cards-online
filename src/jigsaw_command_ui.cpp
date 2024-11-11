#include "jigsaw_command_ui.h"

void JigsawCommandUI::_bind_methods() {
}

JigsawExecutionState JigsawCommandUI::evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const {
	return JigsawExecutionState::CONTINUE; // TODO
}

int64_t JigsawCommandUI::get_num_configs() const {
	return 0; // TODO
}
String JigsawCommandUI::get_config_name(int64_t i) const {
	return ""; // TODO
}
String JigsawCommandUI::get_config_desc(int64_t i) const {
	return ""; // TODO
}
int64_t JigsawCommandUI::get_config_value(int64_t i) const {
	return 0; // TODO
}
void JigsawCommandUI::set_config_value(int64_t i, int64_t value) {
	// TODO
}
PackedStringArray JigsawCommandUI::get_config_options(int64_t i) const {
	return PackedStringArray(); // TODO
}

int64_t JigsawCommandUI::get_num_arguments() const {
	return 0; // TODO
}
Ref<JigsawParameter> JigsawCommandUI::get_argument(int64_t i) const {
	return Ref<JigsawParameter>(); // TODO
}
TypedArray<JigsawParameter> JigsawCommandUI::get_argument_template(int64_t i, const Ref<JigsawContext> &context) const {
	return TypedArray<JigsawParameter>(); // TODO
}
void JigsawCommandUI::set_argument(int64_t i, const Ref<JigsawParameter> &arg) {
	// TODO
}
String JigsawCommandUI::get_argument_name(int64_t i) const {
	return ""; // TODO
}

int64_t JigsawCommandUI::get_num_results() const {
	return 0; // TODO
}
Ref<JigsawParameterLocalVariable> JigsawCommandUI::get_result(int64_t i) const {
	return Ref<JigsawParameterLocalVariable>(); // TODO
}
TypedArray<JigsawParameter> JigsawCommandUI::get_result_template(int64_t i, const Ref<JigsawContext> &context) const {
	return TypedArray<JigsawParameter>(); // TODO
}
void JigsawCommandUI::set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) {
	// TODO
}
String JigsawCommandUI::get_result_name(int64_t i) const {
	return ""; // TODO
}
