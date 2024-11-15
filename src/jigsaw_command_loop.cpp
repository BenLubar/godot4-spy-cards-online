#include "jigsaw_command_loop.h"

#include "jigsaw_parameter_amount.h"

void JigsawCommandLoop::_bind_methods() {
	BIND_PROPERTY_RESOURCE(JigsawParameter, count);
	BIND_PROPERTY_RESOURCE(JigsawCommandList, body);
}

IMPLEMENT_PROPERTY(JigsawCommandLoop, Ref<JigsawParameter>, count);
IMPLEMENT_PROPERTY(JigsawCommandLoop, Ref<JigsawCommandList>, body);

JigsawExecutionState JigsawCommandLoop::evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const {
	Ref<JigsawParameterAmount> count;
	err = context->resolve_variable(_count, count, "count");
	if (unlikely(err.is_valid())) {
		return JigsawExecutionState::ERROR;
	}

	if (count->is_nan()) {
		err = context->create_error("loop count is not a number");
		return JigsawExecutionState::ERROR;
	}

	if (count->get_amount_inf() > 0) {
		// if we have infinity loops, just loop a really big number of times instead
		count = JigsawParameterAmount::make(INT64_MAX);
	}

	if (count->get_amount_inf() != 0 || count->get_amount() <= 0) {
		return JigsawExecutionState::CONTINUE;
	}

	err = context->append_stack_frame(_body, 0, Array::make(JigsawParameterAmount::make(0)));
	if (unlikely(err.is_valid())) {
		return JigsawExecutionState::ERROR;
	}

	TypedArray<JigsawStackFrame> stack = context->get_stack();
	Ref<JigsawStackFrame> top = stack.back();
	top->set_meta("loop_index", int64_t(0));
	top->set_meta("loop_count", count->get_amount());

	return JigsawExecutionState::CONTINUE;
}
Ref<JigsawError> JigsawCommandLoop::pop_stack_frame(const Ref<JigsawContext> &context, const Ref<JigsawStackFrame> &popped_frame) const {
	int64_t index = popped_frame->get_meta("loop_index");
	int64_t count = popped_frame->get_meta("loop_count");
	index++;

	if (index >= count) {
		return Ref<JigsawError>();
	}

	Ref<JigsawError> err = context->append_stack_frame(_body, 0, Array::make(JigsawParameterAmount::make(index)));
	if (unlikely(err.is_valid())) {
		return err;
	}

	TypedArray<JigsawStackFrame> stack = context->get_stack();
	Ref<JigsawStackFrame> top = stack.back();
	top->set_meta("loop_index", index);
	top->set_meta("loop_count", count);

	return Ref<JigsawError>();
}

int64_t JigsawCommandLoop::get_num_arguments() const {
	return 1;
}
Ref<JigsawParameter> JigsawCommandLoop::get_argument(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, Ref<JigsawParameter>());

	return _count;
}
TypedArray<JigsawParameter> JigsawCommandLoop::get_argument_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, 1, TypedArray<JigsawParameter>());

	return Array::make(JigsawParameterAmount::make(0));
}
void JigsawCommandLoop::set_argument(int64_t i, const Ref<JigsawParameter> &arg) {
	ERR_FAIL_INDEX(i, 1);

	_count = arg;
	emit_changed();
}
String JigsawCommandLoop::get_argument_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "count";
}

int64_t JigsawCommandLoop::get_num_branches() const {
	return 1;
}
Ref<JigsawCommandList> JigsawCommandLoop::get_branch(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, Ref<JigsawCommandList>());

	return _body;
}
void JigsawCommandLoop::set_branch(int64_t i, const Ref<JigsawCommandList> &commands) {
	ERR_FAIL_INDEX(i, 1);

	_body = commands;
	emit_changed();
}
String JigsawCommandLoop::get_branch_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "body";
}
TypedArray<JigsawParameter> JigsawCommandLoop::get_branch_argument_templates(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, TypedArray<JigsawParameter>());

	return Array::make(JigsawParameterAmount::make(0));
}
PackedStringArray JigsawCommandLoop::get_branch_argument_names(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, PackedStringArray());

	return PackedStringArray{
		"index",
	};
}
