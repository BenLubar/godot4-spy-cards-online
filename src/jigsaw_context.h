#ifndef JIGSAW_CONTEXT_H
#define JIGSAW_CONTEXT_H

#include "dry.h"

class JigsawContext;
class JigsawGlobal;

#include "jigsaw_command_list.h"
#include "jigsaw_error.h"
#include "jigsaw_parameter.h"
#include "jigsaw_procedure.h"
#include "jigsaw_stack_frame.h"

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
	enum ExecutionState {
		STATE_ERROR = 0,
		STATE_DONE = 1,
		STATE_CONTINUE = 2,
		STATE_PAUSE = 3,
	};
	Ref<JigsawError> append_stack_frame(const Ref<JigsawCommandList> &commands, int64_t branch);
	Ref<JigsawError> resolve_or_copy_variable(const Ref<JigsawParameter> &tmpl, Ref<JigsawParameter> &ret, const String &debug_name) const;

private:
	void cleanup();
	ExecutionState evaluate_next(Ref<JigsawError> &err);

public:
	Ref<JigsawError> evaluate(const Ref<JigsawProcedure> &procedure, const TypedArray<JigsawParameter> &args, const TypedArray<JigsawParameter> &results, int64_t max_steps = DEFAULT_MAX_STEPS);
	Ref<JigsawError> run(const Ref<JigsawProcedure> &procedure, const TypedArray<JigsawParameter> &args, int64_t max_steps = DEFAULT_MAX_STEPS);
	Ref<JigsawError> continue_run(int64_t max_steps = DEFAULT_MAX_STEPS);
	bool is_in_progress() const;
	Ref<JigsawError> create_error(const String &message, const TypedArray<JigsawParameter> &params = TypedArray<JigsawParameter>(), bool include_global_snapshot = false) const;

	static Ref<JigsawContext> make(JigsawGlobal *global, const Ref<JigsawContext> &parent);
};

#endif // JIGSAW_CONTEXT_H
