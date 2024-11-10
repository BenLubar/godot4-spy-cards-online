#include "jigsaw_command_audience.h"

void JigsawCommandAudience::_bind_methods() {
}

JigsawExecutionState JigsawCommandAudience::evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const {
	return JigsawExecutionState::CONTINUE;
}

int64_t JigsawCommandAudience::get_num_configs() const {
	return 0;
}
String JigsawCommandAudience::get_config_name(int64_t i) const {
	return "";
}
String JigsawCommandAudience::get_config_desc(int64_t i) const {
	return "";
}
int64_t JigsawCommandAudience::get_config_value(int64_t i) const {
	return 0;
}
void JigsawCommandAudience::set_config_value(int64_t i, int64_t value) {
}
PackedStringArray JigsawCommandAudience::get_config_options(int64_t i) const {
	return PackedStringArray();
}

int64_t JigsawCommandAudience::get_num_arguments() const {
	return 0;
}
Ref<JigsawParameter> JigsawCommandAudience::get_argument(int64_t i) const {
	return Ref<JigsawParameter>();
}
TypedArray<JigsawParameter> JigsawCommandAudience::get_argument_template(int64_t i, const Ref<JigsawContext> &context) const {
	return TypedArray<JigsawParameter>();
}
void JigsawCommandAudience::set_argument(int64_t i, const Ref<JigsawParameter> &arg) {
}
String JigsawCommandAudience::get_argument_name(int64_t i) const {
	return "";
}

int64_t JigsawCommandAudience::get_num_results() const {
	return 0;
}
Ref<JigsawParameterLocalVariable> JigsawCommandAudience::get_result(int64_t i) const {
	return Ref<JigsawParameterLocalVariable>();
}
TypedArray<JigsawParameter> JigsawCommandAudience::get_result_template(int64_t i, const Ref<JigsawContext> &context) const {
	return TypedArray<JigsawParameter>();
}
void JigsawCommandAudience::set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) {
}
String JigsawCommandAudience::get_result_name(int64_t i) const {
	return "";
}
