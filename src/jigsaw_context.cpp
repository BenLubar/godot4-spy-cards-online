#include "jigsaw_context.h"
#include "jigsaw_global.h"

void JigsawContext::_bind_methods() {
	BIND_PROPERTY_RESOURCE(JigsawGlobal, global);
	BIND_PROPERTY_RESOURCE(JigsawContext, parent);
	BIND_PROPERTY_RESOURCE(JigsawProcedure, procedure);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawStackFrame, stack);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawCommandList, command_stack);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawParameter, arguments);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawParameter, results);

	ClassDB::bind_method(D_METHOD("evaluate", "procedure", "args", "results"), &JigsawContext::evaluate);
	ClassDB::bind_method(D_METHOD("run", "procedure", "args"), &JigsawContext::run);
	ClassDB::bind_method(D_METHOD("continue_run"), &JigsawContext::continue_run);
	ClassDB::bind_method(D_METHOD("is_in_progress"), &JigsawContext::is_in_progress);
	ClassDB::bind_method(D_METHOD("create_error", "message", "params", "include_global_snapshot"), &JigsawContext::create_error, DEFVAL(TypedArray<JigsawParameter>()), DEFVAL(false));

	ClassDB::bind_static_method("JigsawContext", D_METHOD("make", "global", "parent"), &JigsawContext::make, DEFVAL(Ref<JigsawContext>()));
}

IMPLEMENT_PROPERTY_SIMPLE(JigsawContext, JigsawGlobal *, global);
IMPLEMENT_PROPERTY_SIMPLE(JigsawContext, Ref<JigsawContext>, parent);
IMPLEMENT_PROPERTY_SIMPLE(JigsawContext, Ref<JigsawProcedure>, procedure);
IMPLEMENT_PROPERTY_SIMPLE(JigsawContext, TypedArray<JigsawStackFrame>, stack);
IMPLEMENT_PROPERTY_SIMPLE(JigsawContext, TypedArray<JigsawCommandList>, command_stack);
IMPLEMENT_PROPERTY_SIMPLE(JigsawContext, TypedArray<JigsawParameter>, arguments);
IMPLEMENT_PROPERTY_SIMPLE(JigsawContext, TypedArray<JigsawParameter>, results);

void JigsawContext::cleanup() {
	_procedure = Ref<JigsawProcedure>();
	_stack.clear();
	_command_stack.clear();
	_arguments = TypedArray<JigsawParameter>();
	// _results is not cleared
}

void JigsawContext::append_stack_frame(const Ref<JigsawCommandList> &commands) {
}

JigsawContext::ExecutionState JigsawContext::evaluate_next(Ref<JigsawError> &err) {
	err = create_error("TODO");

	return STATE_ERROR;
}

Ref<JigsawError> JigsawContext::evaluate(const Ref<JigsawProcedure> &procedure, const TypedArray<JigsawParameter> &args, const TypedArray<JigsawParameter> &results) {
	ERR_FAIL_COND_V(is_in_progress(), create_error("context was not ready"));
	ERR_FAIL_COND_V(procedure.is_null(), create_error("null procedure"));

	set_procedure(procedure);
	set_arguments(args);
	set_results(results);
	append_stack_frame(procedure->get_commands());

	Ref<JigsawError> err;
	for (;;) {
		ExecutionState state = evaluate_next(err);
		if (likely(state == STATE_CONTINUE)) {
			continue;
		}

		if (unlikely(state == STATE_PAUSE)) {
			err = create_error("cannot pause a procedure that returns a value");
			cleanup();
			return err;
		}

		cleanup();
		ERR_FAIL_COND_V(state != STATE_DONE && state != STATE_ERROR, err);
		return err;
	}
}
Ref<JigsawError> JigsawContext::run(const Ref<JigsawProcedure> &procedure, const TypedArray<JigsawParameter> &args) {
	ERR_FAIL_COND_V(is_in_progress(), create_error("context was not ready"));
	ERR_FAIL_COND_V(procedure.is_null(), create_error("null procedure"));

	return create_error("TODO"); // TODO
}
Ref<JigsawError> JigsawContext::continue_run() {
	ERR_FAIL_COND_V(!is_in_progress(), create_error("context was ready"));

	return create_error("TODO"); // TODO
}
bool JigsawContext::is_in_progress() const {
	return !_stack.is_empty();
}
Ref<JigsawError> JigsawContext::create_error(const String &message, const TypedArray<JigsawParameter> &params, bool include_global_snapshot) const {
	Ref<JigsawError> error;
	error.instantiate();

	if (include_global_snapshot) {
		error->_flags1.set_flag(JigsawError::CONTAINS_GLOBAL_SNAPSHOT);
	}

	error->set_message(message);
	error->set_params(params.duplicate(true));

	if (include_global_snapshot) {
		// TODO: store global snapshot
	} else {
		// TODO: store local context snapshot
	}

	return error;
}

Ref<JigsawContext> JigsawContext::make(JigsawGlobal *global, const Ref<JigsawContext> &parent) {
	ERR_FAIL_NULL_V(global, Ref<JigsawContext>());

	Ref<JigsawContext> context;
	context.instantiate();
	context->set_global(global);
	context->set_parent(parent);
	return context;
}
