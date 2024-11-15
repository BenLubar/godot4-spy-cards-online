#ifndef JIGSAW_CONTEXT_H
#define JIGSAW_CONTEXT_H

#include "dry.h"

class JigsawContext;
enum class JigsawExecutionState;
class JigsawGlobal;

#include "jigsaw_command_list.h"
#include "jigsaw_error.h"
#include "jigsaw_parameter.h"
#include "jigsaw_parameter_local_variable.h"
#include "jigsaw_parameter_variable.h"
#include "jigsaw_procedure.h"
#include "jigsaw_stack_frame.h"
#include "why_isnt_this_in_godot.h"

enum class JigsawExecutionState {
	ERROR = 0,
	DONE = 1,
	CONTINUE = 2,
	PAUSE = 3,
};

class JigsawContext : public RefCounted {
	GDCLASS(JigsawContext, RefCounted);

protected:
	static void _bind_methods();

public:
	// high to avoid restricting creativity but finite to allow breaking out of infinite loops
	static constexpr int64_t DEFAULT_MAX_STEPS = 10000;

	DECLARE_PROPERTY(JigsawGlobal *, global, = nullptr);
	DECLARE_PROPERTY(Ref<JigsawContext>, parent);
	DECLARE_PROPERTY(Ref<JigsawProcedure>, procedure);
	DECLARE_PROPERTY(TypedArray<JigsawStackFrame>, stack);
	DECLARE_PROPERTY(TypedArray<JigsawParameter>, arguments);
	DECLARE_PROPERTY(TypedArray<JigsawParameter>, results);
	DECLARE_PROPERTY(int64_t, step_limit_remaining, = 0);

public:
	// public to C++ code, private to GDScript
	Ref<JigsawError> append_stack_frame(const Ref<JigsawCommandList> &commands, int64_t branch, const TypedArray<JigsawParameter> &args = TypedArray<JigsawParameter>());
	Ref<JigsawError> pop_stack_frame();
	template<typename T>
	_FORCE_INLINE_ Ref<JigsawError> resolve_variable(const Ref<JigsawParameter> &tmpl, Ref<T> &ret, const String &debug_name) const;
	Ref<JigsawError> set_local_variable(const Ref<JigsawParameterLocalVariable> &var, const Ref<JigsawParameter> &value, const String &debug_name);
	Ref<JigsawError> set_persistent_variable(const Ref<JigsawParameterVariable> &var, const Ref<JigsawParameter> &value, const String &debug_name);

private:
	void cleanup();
	JigsawExecutionState evaluate_next(Ref<JigsawError> &err, bool first);

public:
	Ref<JigsawError> evaluate(const Ref<JigsawProcedure> &procedure, const TypedArray<JigsawParameter> &args, const TypedArray<JigsawParameter> &results, int64_t max_steps = DEFAULT_MAX_STEPS);
	Ref<JigsawError> run(const Ref<JigsawProcedure> &procedure, const TypedArray<JigsawParameter> &args, int64_t max_steps = DEFAULT_MAX_STEPS);
	Ref<JigsawError> continue_run(int64_t max_steps = DEFAULT_MAX_STEPS);
	bool is_in_progress() const;
	Ref<JigsawError> create_error(const String &message, const TypedArray<JigsawParameter> &params = TypedArray<JigsawParameter>(), bool include_global_snapshot = false) const;

	DEFAULT_TO_STRING();

	static Ref<JigsawContext> make(JigsawGlobal *global, const Ref<JigsawContext> &parent);
};

template<>
Ref<JigsawError> JigsawContext::resolve_variable<JigsawParameter>(const Ref<JigsawParameter> &tmpl, Ref<JigsawParameter> &ret, const String &debug_name) const;

template<typename T>
_FORCE_INLINE_ Ref<JigsawError> JigsawContext::resolve_variable(const Ref<JigsawParameter> &tmpl, Ref<T> &ret, const String &debug_name) const {
	Ref<JigsawParameter> param;
	Ref<JigsawError> err = resolve_variable(tmpl, param, debug_name);

	ret = param;
	if (unlikely(err.is_null() && ret.is_null())) {
		err = create_error(vformat("'%s' is invalid parameter type %s", debug_name, WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawParameter", "Type", param->get_type())));
	}

	return err;
}

#endif // JIGSAW_CONTEXT_H
