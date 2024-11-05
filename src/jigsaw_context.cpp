#include "jigsaw_context.h"

#include "jigsaw_global.h"
#include "jigsaw_parameter_effect_instance.h"
#include "jigsaw_parameter_effect_instance_parameter.h"
#include "jigsaw_parameter_local_variable.h"
#include "jigsaw_parameter_variable.h"
#include "why_isnt_this_in_godot.h"

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
		Ref<JigsawError> err = resolve_variable(local_templates[i], local, local_names[i]);
		if (err.is_valid()) {
			return err;
		}
		locals[i] = local;
	}

	return Ref<JigsawError>();
}
Ref<JigsawError> JigsawContext::pop_stack_frame() {
	// TODO: blocks with return values
	_stack.pop_back();

	while (!_stack.is_empty()) {
		Ref<JigsawStackFrame> frame = _stack.back();
		if (frame->get_instruction_pointer() + 1 == frame->get_commands()->get_list().size()) {
			_stack.pop_back();
		} else {
			break;
		}
	}

	return Ref<JigsawError>();
}
template<>
Ref<JigsawError> JigsawContext::resolve_variable<JigsawParameter>(const Ref<JigsawParameter> &tmpl, Ref<JigsawParameter> &ret, const String &debug_name) const {
	ERR_FAIL_COND_V(tmpl.is_null(), create_error(vformat("missing variable value for '%s'", debug_name)));

	JigsawParameter::Type type = tmpl->get_type();
	if (JigsawParameter::is_concrete_type(type)) {
		ret = tmpl;

		return Ref<JigsawError>();
	}

	if (type == JigsawParameter::VARIABLE) {
		return create_error("internal error: TODO (resolve persistent variable)");
	}

	if (type == JigsawParameter::LOCAL_VARIABLE) {
		Ref<JigsawParameterLocalVariable> variable_param = tmpl;
		int64_t frame_number = variable_param->get_frame();
		TypedArray<JigsawParameter> frame_variables;
		if (frame_number == JigsawParameterLocalVariable::FRAME_ARGUMENTS) {
			frame_variables = get_arguments();
		} else if (frame_number == JigsawParameterLocalVariable::FRAME_RESULTS) {
			frame_variables = get_results();
		} else if (unlikely(frame_number < 0 || frame_number >= get_stack().size())) {
			return create_error(vformat("cannot find stack frame number %d for '%s'", frame_number, debug_name));
		} else {
			Ref<JigsawStackFrame> frame = get_stack()[frame_number];
			frame_variables = frame->get_local_variables();
		}

		int64_t slot = variable_param->get_slot();
		if (unlikely(slot < 0 || slot >= frame_variables.size())) {
			return create_error(vformat("no such slot %d in stack frame number %d for '%s'", slot, frame_number, debug_name));
		}

		ret = frame_variables[slot];
		return Ref<JigsawError>();
	}

	if (type == JigsawParameter::EFFECT_INSTANCE_PARAMETER) {
		Ref<JigsawParameterEffectInstanceParameter> effect_instance_param_param_param = tmpl;
		int64_t index = effect_instance_param_param_param->get_index();
		if (unlikely(index < 0)) {
			return create_error(vformat("negative effect instance parameter index for '%s'", debug_name));
		}

		Ref<JigsawParameter> effect_instance_param_param;
		Ref<JigsawError> err = resolve_variable(effect_instance_param_param_param->get_effect_instance(), effect_instance_param_param, vformat("%s (effect)", debug_name));
		if (unlikely(err.is_valid())) {
			return err;
		}

		Ref<JigsawParameterEffectInstance> effect_instance_param = effect_instance_param_param;
		if (unlikely(effect_instance_param.is_null())) {
			return create_error(vformat("effect instance for '%s' was of type %s", debug_name, WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawParameter", "Type", effect_instance_param_param->get_type())));
		}

		Ref<EffectInstance> effect_instance = effect_instance_param->get_instance();
		if (unlikely(effect_instance.is_null())) {
			return create_error(vformat("effect instance for '%s' was null", debug_name));
		}

		TypedArray<JigsawParameter> params = effect_instance->get_params();
		if (index < params.size()) {
			Ref<JigsawParameter> param = params[index];
			if (likely(param.is_valid())) {
				ret = param;
				return Ref<JigsawError>();
			}
		}

		JigsawGlobal *global = get_global();
		ERR_FAIL_NULL_V(global, create_error("internal error: missing global in effect instance parameter resolve"));
		Ref<GameMode> mode = global->get_mode();
		ERR_FAIL_COND_V(mode.is_null(), create_error("internal error: missing game mode in effect instance parameter resolve"));
		Ref<EffectDef> effect = mode->get_effect(effect_instance->get_effect());
		ERR_FAIL_COND_V(effect.is_null(), create_error("internal error: missing effect in effect instance parameter resolve"));

		params = effect->get_default_parameters();
		if (index < params.size()) {
			Ref<JigsawParameter> param = params[index];
			if (likely(param.is_valid())) {
				ret = param;
				return Ref<JigsawError>();
			}
		}

		return create_error(vformat("out of range effect instance parameter for '%s'", debug_name));
	}

	return create_error(vformat("internal error: unhandled variable type %d for variable '%s'", type, debug_name));
}
Ref<JigsawError> JigsawContext::set_local_variable(const Ref<JigsawParameterLocalVariable> &var, const Ref<JigsawParameter> &value, const String &debug_name) {
	ERR_FAIL_COND_V(var.is_null(), create_error(vformat("cannot set null local variable '%s'", debug_name)));
	ERR_FAIL_COND_V(value.is_null(), create_error(vformat("cannot set local variable '%s' to null value", debug_name)));
	ERR_FAIL_COND_V(!JigsawParameter::is_concrete_type(value->get_type()), create_error(vformat("cannot set local variable '%s' to non-concrete value type %s", debug_name, WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawParameter", "Type", value->get_type()))));

	int64_t frame_number = var->get_frame();
	TypedArray<JigsawParameter> frame_variables;
	if (frame_number == JigsawParameterLocalVariable::FRAME_ARGUMENTS) {
		frame_variables = get_arguments();
	} else if (frame_number == JigsawParameterLocalVariable::FRAME_RESULTS) {
		frame_variables = get_results();
	} else if (unlikely(frame_number < 0 || frame_number >= get_stack().size())) {
		return create_error(vformat("cannot find stack frame number %d for '%s'", frame_number, debug_name));
	} else {
		Ref<JigsawStackFrame> frame = get_stack()[frame_number];
		frame_variables = frame->get_local_variables();
	}

	int64_t slot = var->get_slot();
	if (unlikely(slot < 0 || slot >= frame_variables.size())) {
		return create_error(vformat("no such slot %d in stack frame number %d for '%s'", slot, frame_number, debug_name));
	}

	frame_variables[slot] = value;

	return Ref<JigsawError>();
}

void JigsawContext::cleanup() {
	_procedure = Ref<JigsawProcedure>();
	_stack.clear();
	_arguments = TypedArray<JigsawParameter>();
	// _results is not cleared
	_step_limit_remaining = 0;
}

JigsawExecutionState JigsawContext::evaluate_next(Ref<JigsawError> &err, bool first) {
	if (_step_limit_remaining <= 0) {
		err = create_error("Procedure step count safety limit exceeded - infinite loop? If you don't think this error should have happened, let Ben know what you were doing.");

		return JigsawExecutionState::ERROR;
	}
	_step_limit_remaining--;

	if (_stack.is_empty()) {
		return JigsawExecutionState::DONE;
	}

	int64_t stack_depth = _stack.size() - 1;
	if (unlikely(stack_depth < 0)) {
		err = create_error("internal error: stack depth is negative. Let Ben know!");

		return JigsawExecutionState::ERROR;
	}

	Ref<JigsawStackFrame> frame = _stack[stack_depth];
	if (unlikely(frame.is_null())) {
		err = create_error("internal error: stack frame is null. Let Ben know!");

		return JigsawExecutionState::ERROR;
	}
	Ref<JigsawCommandList> command_list = frame->get_commands();
	if (unlikely(command_list.is_null())) {
		err = create_error("internal error: stack frame command list is null. Let Ben know!");

		return JigsawExecutionState::ERROR;
	}
	TypedArray<JigsawCommand> commands = command_list->get_list();

	int64_t ip = frame->get_instruction_pointer();
	if (unlikely((!first && ip < 0) || ip < -1)) {
		err = create_error("internal error: instruction pointer is negative. Let Ben know!");

		return JigsawExecutionState::ERROR;
	}

	if (ip == -1 && commands.is_empty()) {
		// special case: empty block

		err = pop_stack_frame();

		if (likely(err.is_null()) && _stack.is_empty()) {
			return JigsawExecutionState::DONE;
		}

		return unlikely(err.is_valid()) ? JigsawExecutionState::ERROR : JigsawExecutionState::CONTINUE;
	}

	if (first) {
		ip++;
		frame->set_instruction_pointer(ip);
	}
	if (unlikely(ip >= commands.size())) {
		err = create_error("internal error: instruction pointer is past end of block. Let Ben know!");

		return JigsawExecutionState::ERROR;
	}

	Ref<JigsawCommand> command = commands[ip];
	if (unlikely(command.is_null())) {
		err = create_error("internal error: command is null. Let Ben know!");

		return JigsawExecutionState::ERROR;
	}

	JigsawExecutionState state = command->evaluate(this, err, first);
	if (state == JigsawExecutionState::CONTINUE && ip == commands.size() - 1 && stack_depth == _stack.size() - 1) {
		err = pop_stack_frame();
		if (err.is_valid()) {
			return JigsawExecutionState::ERROR;
		}

		if (stack_depth == 0) {
			return JigsawExecutionState::DONE;
		}
	}

	return state;
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
		JigsawExecutionState state = evaluate_next(err, true);
		if (likely(state == JigsawExecutionState::CONTINUE)) {
			continue;
		}

		if (unlikely(state == JigsawExecutionState::PAUSE)) {
			err = create_error("internal error: cannot pause a procedure that returns a value");
			cleanup();
			return err;
		}

		cleanup();
		ERR_FAIL_COND_V(state != JigsawExecutionState::DONE && state != JigsawExecutionState::ERROR, err); // redundant condition for error message
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
