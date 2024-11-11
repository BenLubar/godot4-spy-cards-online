#include "jigsaw_command_character.h"

void JigsawCommandCharacter::_bind_methods() {
}

JigsawExecutionState JigsawCommandCharacter::evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const {
	return JigsawExecutionState::CONTINUE; // TODO
}

int64_t JigsawCommandCharacter::get_num_configs() const {
	return 0; // TODO
}
String JigsawCommandCharacter::get_config_name(int64_t i) const {
	return ""; // TODO
}
String JigsawCommandCharacter::get_config_desc(int64_t i) const {
	return ""; // TODO
}
int64_t JigsawCommandCharacter::get_config_value(int64_t i) const {
	return 0; // TODO
}
void JigsawCommandCharacter::set_config_value(int64_t i, int64_t value) {
	// TODO
}
PackedStringArray JigsawCommandCharacter::get_config_options(int64_t i) const {
	return PackedStringArray(); // TODO
}

int64_t JigsawCommandCharacter::get_num_arguments() const {
	return 0; // TODO
}
Ref<JigsawParameter> JigsawCommandCharacter::get_argument(int64_t i) const {
	return Ref<JigsawParameter>(); // TODO
}
TypedArray<JigsawParameter> JigsawCommandCharacter::get_argument_template(int64_t i, const Ref<JigsawContext> &context) const {
	return TypedArray<JigsawParameter>(); // TODO
}
void JigsawCommandCharacter::set_argument(int64_t i, const Ref<JigsawParameter> &arg) {
	// TODO
}
String JigsawCommandCharacter::get_argument_name(int64_t i) const {
	return ""; // TODO
}

int64_t JigsawCommandCharacter::get_num_results() const {
	return 0; // TODO
}
Ref<JigsawParameterLocalVariable> JigsawCommandCharacter::get_result(int64_t i) const {
	return Ref<JigsawParameterLocalVariable>(); // TODO
}
TypedArray<JigsawParameter> JigsawCommandCharacter::get_result_template(int64_t i, const Ref<JigsawContext> &context) const {
	return TypedArray<JigsawParameter>(); // TODO
}
void JigsawCommandCharacter::set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) {
	// TODO
}
String JigsawCommandCharacter::get_result_name(int64_t i) const {
	return ""; // TODO
}
