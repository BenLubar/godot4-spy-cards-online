#include "jigsaw_command_scene.h"

void JigsawCommandScene::_bind_methods() {
}

JigsawExecutionState JigsawCommandScene::evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const {
	return JigsawExecutionState::CONTINUE; // TODO
}

int64_t JigsawCommandScene::get_num_configs() const {
	return 0; // TODO
}
String JigsawCommandScene::get_config_name(int64_t i) const {
	return ""; // TODO
}
String JigsawCommandScene::get_config_desc(int64_t i) const {
	return ""; // TODO
}
int64_t JigsawCommandScene::get_config_value(int64_t i) const {
	return 0; // TODO
}
void JigsawCommandScene::set_config_value(int64_t i, int64_t value) {
	// TODO
}
PackedStringArray JigsawCommandScene::get_config_options(int64_t i) const {
	return PackedStringArray(); // TODO
}

int64_t JigsawCommandScene::get_num_arguments() const {
	return 0; // TODO
}
Ref<JigsawParameter> JigsawCommandScene::get_argument(int64_t i) const {
	return Ref<JigsawParameter>(); // TODO
}
TypedArray<JigsawParameter> JigsawCommandScene::get_argument_template(int64_t i, const Ref<JigsawContext> &context) const {
	return TypedArray<JigsawParameter>(); // TODO
}
void JigsawCommandScene::set_argument(int64_t i, const Ref<JigsawParameter> &arg) {
	// TODO
}
String JigsawCommandScene::get_argument_name(int64_t i) const {
	return ""; // TODO
}

int64_t JigsawCommandScene::get_num_results() const {
	return 0; // TODO
}
Ref<JigsawParameterLocalVariable> JigsawCommandScene::get_result(int64_t i) const {
	return Ref<JigsawParameterLocalVariable>(); // TODO
}
TypedArray<JigsawParameter> JigsawCommandScene::get_result_template(int64_t i, const Ref<JigsawContext> &context) const {
	return TypedArray<JigsawParameter>(); // TODO
}
void JigsawCommandScene::set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) {
	// TODO
}
String JigsawCommandScene::get_result_name(int64_t i) const {
	return ""; // TODO
}
