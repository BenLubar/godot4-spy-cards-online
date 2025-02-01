#include "jigsaw/command/jigsaw_command_log.h"

#include "jigsaw/parameter/jigsaw_parameter_formatted_text.h"

void JigsawCommandLog::_bind_methods() {
	BIND_PROPERTY_RESOURCE(JigsawParameter, message);
}

IMPLEMENT_PROPERTY(JigsawCommandLog, Ref<JigsawParameter>, message);

bool JigsawCommandLog::allowed_in_mode(enums::JigsawProcedure::Mode mode, bool any_config) const {
	using namespace enums::JigsawProcedure;

	switch (mode) {
	case FUNCTIONAL:
		return false;
	case INIT:
	case MAIN:
		return true;
	case SELECT:
	case VISUAL:
	case CHOICE_PREVIEW:
	case REALTIME_LOGIC:
	case REALTIME_VISUAL:
		return false;
	}

	ERR_FAIL_V(false);
}
JigsawExecutionState JigsawCommandLog::evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const {
	err = context->create_error("internal error: TODO (log)");

	return JigsawExecutionState::ERROR;
}

int64_t JigsawCommandLog::get_num_arguments() const {
	return 1;
}
Ref<JigsawParameter> JigsawCommandLog::get_argument(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, Ref<JigsawParameter>());

	return _message;
}
TypedArray<JigsawParameter> JigsawCommandLog::get_argument_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, 1, TypedArray<JigsawParameter>());

	return Array::make(JigsawParameterFormattedText::make(TypedArray<FormattedText>()));
}
void JigsawCommandLog::set_argument(int64_t i, const Ref<JigsawParameter> &arg) {
	ERR_FAIL_INDEX(i, 1);

	_message = arg;
	emit_changed();
}
String JigsawCommandLog::get_argument_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "message";
}
