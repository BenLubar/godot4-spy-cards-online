#include "jigsaw_command_comment.h"

void JigsawCommandComment::_bind_methods() {
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawParameter, params);
	BIND_PROPERTY_RESOURCE(JigsawCommandList, command_list);
}

IMPLEMENT_PROPERTY(JigsawCommandComment, TypedArray<JigsawParameter>, params);
IMPLEMENT_PROPERTY(JigsawCommandComment, Ref<JigsawCommandList>, command_list);

JigsawExecutionState JigsawCommandComment::evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const {
	return JigsawExecutionState::CONTINUE;
}

int64_t JigsawCommandComment::get_num_arguments() const {
	return _params.size() + 1;
}
Ref<JigsawParameter> JigsawCommandComment::get_argument(int64_t i) const {
	if (_params.size() <= i) {
		return Ref<JigsawParameter>();
	}

	return _params[i];
}
TypedArray<JigsawParameter> JigsawCommandComment::get_argument_template(int64_t i, const Ref<JigsawContext> &context) const {
	return TypedArray<JigsawParameter>();
}
void JigsawCommandComment::set_argument(int64_t i, const Ref<JigsawParameter> &arg) {
	if (_params.size() <= i) {
		_params.resize(i + 1);
	}

	_params[i] = arg;
	emit_changed();
}
String JigsawCommandComment::get_argument_name(int64_t i) const {
	return vformat("comment_%d", i + 1);
}
