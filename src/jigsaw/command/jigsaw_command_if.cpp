#include "jigsaw_command_if.h"

#include "jigsaw/parameter/jigsaw_parameter_boolean.h"
#include "util/why_isnt_this_in_godot.h"

void JigsawCommandIf::_bind_methods() {
	BIND_PROPERTY_RESOURCE(JigsawParameter, condition);
	BIND_PROPERTY_RESOURCE(JigsawCommandList, if_true);
	BIND_PROPERTY_RESOURCE(JigsawCommandList, if_false);
}

IMPLEMENT_PROPERTY(JigsawCommandIf, Ref<JigsawParameter>, condition);
IMPLEMENT_PROPERTY(JigsawCommandIf, Ref<JigsawCommandList>, if_true);
IMPLEMENT_PROPERTY(JigsawCommandIf, Ref<JigsawCommandList>, if_false);

bool JigsawCommandIf::allowed_in_mode(enums::JigsawProcedure::Mode mode, bool any_config) const {
	return true;
}
JigsawExecutionState JigsawCommandIf::evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const {
	Ref<JigsawParameterBoolean> cond_param;
	err = context->resolve_variable(_condition, cond_param, "condition");
	if (unlikely(err.is_valid())) {
		return JigsawExecutionState::ERROR;
	}

	if (cond_param->get_boolean()) {
		if (_if_true.is_valid()) {
			err = context->append_stack_frame(_if_true, 0);
		}

		return unlikely(err.is_valid()) ? JigsawExecutionState::ERROR : JigsawExecutionState::CONTINUE;
	} else {
		if (_if_false.is_valid()) {
			err = context->append_stack_frame(_if_false, 0);
		}

		return unlikely(err.is_valid()) ? JigsawExecutionState::ERROR : JigsawExecutionState::CONTINUE;
	}
}

int64_t JigsawCommandIf::get_num_arguments() const {
	return 1;
}
Ref<JigsawParameter> JigsawCommandIf::get_argument(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, Ref<JigsawParameter>());

	return _condition;
}
TypedArray<JigsawParameter> JigsawCommandIf::get_argument_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, 1, TypedArray<JigsawParameter>());

	return Array::make(JigsawParameterBoolean::make(false));
}
void JigsawCommandIf::set_argument(int64_t i, const Ref<JigsawParameter> &arg) {
	ERR_FAIL_INDEX(i, 1);

	_condition = arg;
	emit_changed();
}
String JigsawCommandIf::get_argument_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "condition";
}

int64_t JigsawCommandIf::get_num_branches() const {
	return 2;
}
Ref<JigsawCommandList> JigsawCommandIf::get_branch(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 2, Ref<JigsawCommandList>());

	if (i == 0) {
		return _if_true;
	} else if (i == 1) {
		return _if_false;
	}

	return Ref<JigsawCommandList>();
}
void JigsawCommandIf::set_branch(int64_t i, const Ref<JigsawCommandList> &commands) {
	ERR_FAIL_INDEX(i, 2);

	if (i == 0) {
		_if_true = commands;
		emit_changed();
	} else if (i == 1) {
		_if_false = commands;
		emit_changed();
	}
}
enums::JigsawProcedure::Mode JigsawCommandIf::get_branch_mode(int64_t i, enums::JigsawProcedure::Mode parent_mode) const {
	return parent_mode;
}
String JigsawCommandIf::get_branch_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 2, "");

	if (i == 0) {
		return "then";
	} else if (i == 1) {
		return "else";
	}

	return "";
}
