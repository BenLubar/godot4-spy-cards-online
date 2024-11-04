#include "jigsaw_command_log.h"

#include "jigsaw_parameter_formatted_text.h"

void JigsawCommandLog::_bind_methods() {
	BIND_PROPERTY_RESOURCE(JigsawParameter, message);
}

IMPLEMENT_PROPERTY(JigsawCommandLog, Ref<JigsawParameter>, message);

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
