#include "jigsaw_command_audio.h"

void JigsawCommandAudio::_bind_methods() {
}

bool JigsawCommandAudio::allowed_in_mode(enums::JigsawProcedure::Mode mode, bool any_config) const {
	using namespace enums::JigsawProcedure;

	switch (mode) {
	case FUNCTIONAL:
	case INIT:
	case MAIN:
	case SELECT:
	case CHOICE_PREVIEW:
	case REALTIME_LOGIC:
	case REALTIME_VISUAL:
		break; // TODO
	}

	ERR_FAIL_V(false);
}
JigsawExecutionState JigsawCommandAudio::evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const {
	return JigsawExecutionState::CONTINUE; // TODO
}

int64_t JigsawCommandAudio::get_num_configs() const {
	return 0; // TODO
}
String JigsawCommandAudio::get_config_name(int64_t i) const {
	return ""; // TODO
}
String JigsawCommandAudio::get_config_desc(int64_t i) const {
	return ""; // TODO
}
int64_t JigsawCommandAudio::get_config_value(int64_t i) const {
	return 0; // TODO
}
void JigsawCommandAudio::set_config_value(int64_t i, int64_t value) {
	// TODO
}
PackedStringArray JigsawCommandAudio::get_config_options(int64_t i) const {
	return PackedStringArray(); // TODO
}

int64_t JigsawCommandAudio::get_num_arguments() const {
	return 0; // TODO
}
Ref<JigsawParameter> JigsawCommandAudio::get_argument(int64_t i) const {
	return Ref<JigsawParameter>(); // TODO
}
TypedArray<JigsawParameter> JigsawCommandAudio::get_argument_template(int64_t i, const Ref<JigsawContext> &context) const {
	return TypedArray<JigsawParameter>(); // TODO
}
void JigsawCommandAudio::set_argument(int64_t i, const Ref<JigsawParameter> &arg) {
	// TODO
}
String JigsawCommandAudio::get_argument_name(int64_t i) const {
	return ""; // TODO
}

int64_t JigsawCommandAudio::get_num_results() const {
	return 0; // TODO
}
Ref<JigsawParameterLocalVariable> JigsawCommandAudio::get_result(int64_t i) const {
	return Ref<JigsawParameterLocalVariable>(); // TODO
}
TypedArray<JigsawParameter> JigsawCommandAudio::get_result_template(int64_t i, const Ref<JigsawContext> &context) const {
	return TypedArray<JigsawParameter>(); // TODO
}
void JigsawCommandAudio::set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) {
	// TODO
}
String JigsawCommandAudio::get_result_name(int64_t i) const {
	return ""; // TODO
}
