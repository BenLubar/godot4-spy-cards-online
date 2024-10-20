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
	JigsawContext() = default;
	~JigsawContext() = default;

	DECLARE_PROPERTY(JigsawGlobal *, global, = nullptr);
	DECLARE_PROPERTY(Ref<JigsawContext>, parent);
	DECLARE_PROPERTY(Ref<JigsawProcedure>, procedure);
	DECLARE_PROPERTY(TypedArray<JigsawStackFrame>, stack);
	DECLARE_PROPERTY(TypedArray<JigsawCommandList>, command_stack);
	DECLARE_PROPERTY(TypedArray<JigsawParameter>, arguments);
	DECLARE_PROPERTY(TypedArray<JigsawParameter>, results);

private:
	enum ExecutionState {
		STATE_ERROR = 0,
		STATE_DONE = 1,
		STATE_CONTINUE = 2,
		STATE_PAUSE = 3,
	};
	void cleanup();
	void append_stack_frame(const Ref<JigsawCommandList> &commands);
	ExecutionState evaluate_next(Ref<JigsawError> &err);

public:
	Ref<JigsawError> evaluate(const Ref<JigsawProcedure> &procedure, const TypedArray<JigsawParameter> &args, const TypedArray<JigsawParameter> &results);
	Ref<JigsawError> run(const Ref<JigsawProcedure> &procedure, const TypedArray<JigsawParameter> &args);
	Ref<JigsawError> continue_run();
	bool is_in_progress() const;
	Ref<JigsawError> create_error(const String &message, const TypedArray<JigsawParameter> &params = TypedArray<JigsawParameter>(), bool include_global_snapshot = false) const;

	static Ref<JigsawContext> make(JigsawGlobal *global, const Ref<JigsawContext> &parent);
};

#endif // JIGSAW_CONTEXT_H
