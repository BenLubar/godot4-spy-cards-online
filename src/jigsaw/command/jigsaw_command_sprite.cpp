#include "jigsaw_command_sprite.h"

void JigsawCommandSprite::_bind_methods() {
}

bool JigsawCommandSprite::allowed_in_mode(enums::JigsawProcedure::Mode mode, bool any_config) const {
	using namespace enums::JigsawProcedure;

	switch (mode) {
	case FUNCTIONAL:
	case INIT:
	case MAIN:
	case SELECT:
	case VISUAL:
	case CHOICE_PREVIEW:
	case REALTIME_LOGIC:
	case REALTIME_VISUAL:
		break; // TODO
	}

	ERR_FAIL_V(false);
}
JigsawExecutionState JigsawCommandSprite::evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const {
	return JigsawExecutionState::CONTINUE; // TODO
}

int64_t JigsawCommandSprite::get_num_configs() const {
	return 0; // TODO
}
String JigsawCommandSprite::get_config_name(int64_t i) const {
	return ""; // TODO
}
String JigsawCommandSprite::get_config_desc(int64_t i) const {
	return ""; // TODO
}
int64_t JigsawCommandSprite::get_config_value(int64_t i) const {
	return 0; // TODO
}
void JigsawCommandSprite::set_config_value(int64_t i, int64_t value) {
	// TODO
}
PackedStringArray JigsawCommandSprite::get_config_options(int64_t i) const {
	return PackedStringArray(); // TODO
}

int64_t JigsawCommandSprite::get_num_arguments() const {
	return 0; // TODO
}
Ref<JigsawParameter> JigsawCommandSprite::get_argument(int64_t i) const {
	return Ref<JigsawParameter>(); // TODO
}
TypedArray<JigsawParameter> JigsawCommandSprite::get_argument_template(int64_t i, const Ref<JigsawContext> &context) const {
	return TypedArray<JigsawParameter>(); // TODO
}
void JigsawCommandSprite::set_argument(int64_t i, const Ref<JigsawParameter> &arg) {
	// TODO
}
String JigsawCommandSprite::get_argument_name(int64_t i) const {
	return ""; // TODO
}

int64_t JigsawCommandSprite::get_num_results() const {
	return 0; // TODO
}
Ref<JigsawParameterLocalVariable> JigsawCommandSprite::get_result(int64_t i) const {
	return Ref<JigsawParameterLocalVariable>(); // TODO
}
TypedArray<JigsawParameter> JigsawCommandSprite::get_result_template(int64_t i, const Ref<JigsawContext> &context) const {
	return TypedArray<JigsawParameter>(); // TODO
}
void JigsawCommandSprite::set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) {
	// TODO
}
String JigsawCommandSprite::get_result_name(int64_t i) const {
	return ""; // TODO
}
