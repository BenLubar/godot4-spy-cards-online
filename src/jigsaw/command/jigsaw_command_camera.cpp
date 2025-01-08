#include "jigsaw_command_camera.h"

void JigsawCommandCamera::_bind_methods() {
}

bool JigsawCommandCamera::allowed_in_mode(enums::JigsawProcedure::Mode mode, bool any_config) const {
	using namespace enums::JigsawProcedure;

	switch (mode) {
	case FUNCTIONAL:
	case LOGIC:
	case VISUAL:
	case INIT:
	case MAIN:
	case CHOICE_SELECT:
	case CHOICE_PREVIEW:
	case REALTIME_LOGIC:
	case REALTIME_VISUAL:
		break; // TODO
	}

	ERR_FAIL_V(false);
}
JigsawExecutionState JigsawCommandCamera::evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const {
	return JigsawExecutionState::CONTINUE; // TODO
}

int64_t JigsawCommandCamera::get_num_configs() const {
	return 0; // TODO
}
String JigsawCommandCamera::get_config_name(int64_t i) const {
	return ""; // TODO
}
String JigsawCommandCamera::get_config_desc(int64_t i) const {
	return ""; // TODO
}
int64_t JigsawCommandCamera::get_config_value(int64_t i) const {
	return 0; // TODO
}
void JigsawCommandCamera::set_config_value(int64_t i, int64_t value) {
	// TODO
}
PackedStringArray JigsawCommandCamera::get_config_options(int64_t i) const {
	return PackedStringArray(); // TODO
}

int64_t JigsawCommandCamera::get_num_arguments() const {
	return 0; // TODO
}
Ref<JigsawParameter> JigsawCommandCamera::get_argument(int64_t i) const {
	return Ref<JigsawParameter>(); // TODO
}
TypedArray<JigsawParameter> JigsawCommandCamera::get_argument_template(int64_t i, const Ref<JigsawContext> &context) const {
	return TypedArray<JigsawParameter>(); // TODO
}
void JigsawCommandCamera::set_argument(int64_t i, const Ref<JigsawParameter> &arg) {
	// TODO
}
String JigsawCommandCamera::get_argument_name(int64_t i) const {
	return ""; // TODO
}

int64_t JigsawCommandCamera::get_num_results() const {
	return 0; // TODO
}
Ref<JigsawParameterLocalVariable> JigsawCommandCamera::get_result(int64_t i) const {
	return Ref<JigsawParameterLocalVariable>(); // TODO
}
TypedArray<JigsawParameter> JigsawCommandCamera::get_result_template(int64_t i, const Ref<JigsawContext> &context) const {
	return TypedArray<JigsawParameter>(); // TODO
}
void JigsawCommandCamera::set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) {
	// TODO
}
String JigsawCommandCamera::get_result_name(int64_t i) const {
	return ""; // TODO
}
