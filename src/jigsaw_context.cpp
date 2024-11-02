#include "jigsaw_context.h"
#include "jigsaw_global.h"

void JigsawContext::_bind_methods() {
	BIND_CONSTANT(DEFAULT_MAX_STEPS);

	BIND_PROPERTY_RESOURCE(JigsawGlobal, global);
	BIND_PROPERTY_RESOURCE(JigsawContext, parent);
	BIND_PROPERTY_RESOURCE(JigsawProcedure, procedure);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawStackFrame, stack);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawParameter, arguments);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawParameter, results);
	BIND_PROPERTY(Variant::INT, step_limit_remaining);

	ClassDB::bind_method(D_METHOD("evaluate", "procedure", "args", "results", "max_steps"), &JigsawContext::evaluate, DEFVAL(DEFAULT_MAX_STEPS));
	ClassDB::bind_method(D_METHOD("run", "procedure", "args", "max_steps"), &JigsawContext::run, DEFVAL(DEFAULT_MAX_STEPS));
	ClassDB::bind_method(D_METHOD("continue_run", "max_steps"), &JigsawContext::continue_run, DEFVAL(DEFAULT_MAX_STEPS));
	ClassDB::bind_method(D_METHOD("is_in_progress"), &JigsawContext::is_in_progress);
	ClassDB::bind_method(D_METHOD("create_error", "message", "params", "include_global_snapshot"), &JigsawContext::create_error, DEFVAL(TypedArray<JigsawParameter>()), DEFVAL(false));

	ClassDB::bind_static_method("JigsawContext", D_METHOD("make", "global", "parent"), &JigsawContext::make, DEFVAL(Ref<JigsawContext>()));
}

IMPLEMENT_PROPERTY_SIMPLE(JigsawContext, JigsawGlobal *, global);
IMPLEMENT_PROPERTY_SIMPLE(JigsawContext, Ref<JigsawContext>, parent);
IMPLEMENT_PROPERTY_SIMPLE(JigsawContext, Ref<JigsawProcedure>, procedure);
IMPLEMENT_PROPERTY_SIMPLE(JigsawContext, TypedArray<JigsawStackFrame>, stack);
IMPLEMENT_PROPERTY_SIMPLE(JigsawContext, TypedArray<JigsawParameter>, arguments);
IMPLEMENT_PROPERTY_SIMPLE(JigsawContext, TypedArray<JigsawParameter>, results);
IMPLEMENT_PROPERTY_SIMPLE(JigsawContext, int64_t, step_limit_remaining);

Ref<JigsawError> JigsawContext::append_stack_frame(const Ref<JigsawCommandList> &commands, int64_t branch) {
	ERR_FAIL_COND_V(commands.is_null(), create_error("null command list"));

	Ref<JigsawStackFrame> frame;
	frame.instantiate();
	frame->set_branch(branch);
	frame->set_commands(commands);
	_stack.append(frame);

	PackedStringArray local_names = commands->get_local_variable_names();
	TypedArray<JigsawParameter> local_templates = commands->get_local_variables();

	TypedArray<JigsawParameter> locals;
	locals.resize(local_templates.size());
	frame->set_local_variables(locals);

	for (int64_t i = 0; i < local_templates.size(); i++) {
		Ref<JigsawParameter> local;
		Ref<JigsawError> err = resolve_or_copy_variable(local_templates[i], local, local_names[i]);
		if (err.is_valid()) {
			return err;
		}
		locals[i] = local;
	}

	return Ref<JigsawError>();
}
Ref<JigsawError> JigsawContext::resolve_or_copy_variable(const Ref<JigsawParameter> &tmpl, Ref<JigsawParameter> &ret, const String &debug_name) const {
	ERR_FAIL_COND_V(tmpl.is_null(), create_error(vformat("missing variable value for '%s'", debug_name)));

	JigsawParameter::Type type = tmpl->get_type();
	if (JigsawParameter::is_concrete_type(type)) {
		ret = tmpl->duplicate();

		return Ref<JigsawError>();
	}

	if (type == JigsawParameter::VARIABLE) {
		return create_error("internal error: TODO (resolve persistent variable)");
	}

	if (type == JigsawParameter::LOCAL_VARIABLE) {
		return create_error("internal error: TODO (resolve local variable)");
	}

	if (type == JigsawParameter::EFFECT_INSTANCE_PARAMETER) {
		return create_error("internal error: TODO (resolve effect instance parameter)");
	}

	return create_error(vformat("internal error: unhandled variable type %d for variable '%s'", type, debug_name));
}

void JigsawContext::cleanup() {
	_procedure = Ref<JigsawProcedure>();
	_stack.clear();
	_arguments = TypedArray<JigsawParameter>();
	// _results is not cleared
	_step_limit_remaining = 0;
}

JigsawContext::ExecutionState JigsawContext::evaluate_next(Ref<JigsawError> &err) {
	if (_step_limit_remaining <= 0) {
		err = create_error("Procedure step count safety limit exceeded - infinite loop? If you don't think this error should have happened, let Ben know what you were doing.");

		return STATE_ERROR;
	}
	_step_limit_remaining--;

	err = create_error("internal error: TODO (evaluate_next)");

	return STATE_ERROR;
}

Ref<JigsawError> JigsawContext::evaluate(const Ref<JigsawProcedure> &procedure, const TypedArray<JigsawParameter> &args, const TypedArray<JigsawParameter> &results, int64_t max_steps) {
	ERR_FAIL_COND_V(is_in_progress(), create_error("internal error: a procedure was already running in this context"));
	ERR_FAIL_COND_V(procedure.is_null(), create_error("internal error: null procedure"));

	set_procedure(procedure);
	set_arguments(args);
	set_results(results);
	set_step_limit_remaining(max_steps);

	Ref<JigsawError> err = append_stack_frame(procedure->get_commands(), -1);
	if (err.is_valid()) {
		return err;
	}

	for (;;) {
		ExecutionState state = evaluate_next(err);
		if (likely(state == STATE_CONTINUE)) {
			continue;
		}

		if (unlikely(state == STATE_PAUSE)) {
			err = create_error("internal error: cannot pause a procedure that returns a value");
			cleanup();
			return err;
		}

		cleanup();
		ERR_FAIL_COND_V(state != STATE_DONE && state != STATE_ERROR, err); // redundant condition for error message
		return err;
	}
}
Ref<JigsawError> JigsawContext::run(const Ref<JigsawProcedure> &procedure, const TypedArray<JigsawParameter> &args, int64_t max_steps) {
	ERR_FAIL_COND_V(is_in_progress(), create_error("internal error: a procedure was already running in this context"));
	ERR_FAIL_COND_V(procedure.is_null(), create_error("internal error: null procedure"));

	return create_error("internal error: TODO (run)"); // TODO
}
Ref<JigsawError> JigsawContext::continue_run(int64_t max_steps) {
	ERR_FAIL_COND_V(!is_in_progress(), create_error("internal error: no procedure was running in this context"));

	return create_error("internal error: TODO (continue_run)"); // TODO
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
