#ifndef JIGSAW_CONTEXT_H
#define JIGSAW_CONTEXT_H

#include "dry.h"

class JigsawContext;

class JigsawGlobal; // including the header means CardDef::Card gets defined after GameMode, which causes an error
#include "jigsaw_command_list.h"
#include "jigsaw_stack_frame.h"
#include "jigsaw_error.h"

class JigsawContext : public RefCounted {
	GDCLASS(JigsawContext, RefCounted);

protected:
	static void _bind_methods();

public:
	JigsawContext() = default;
	~JigsawContext() = default;

	DECLARE_PROPERTY(JigsawGlobal *, global, = nullptr);
	DECLARE_PROPERTY(Ref<JigsawContext>, parent);
	DECLARE_PROPERTY(bool, read_only, = false);
	DECLARE_PROPERTY(TypedArray<JigsawStackFrame>, stack);
	DECLARE_PROPERTY(TypedArray<JigsawCommandList>, command_stack);
	DECLARE_PROPERTY(TypedArray<JigsawParameter>, environment);
	DECLARE_PROPERTY(TypedArray<JigsawParameter>, arguments);
	DECLARE_PROPERTY(TypedArray<JigsawParameter>, results);

	TypedArray<JigsawError> validate(const Ref<JigsawCommandList> &commands, const TypedArray<JigsawParameter> &env, const TypedArray<JigsawParameter> &args, const TypedArray<JigsawParameter> &results, const String &debug_name, const PackedStringArray &arg_names, const PackedStringArray &result_names) const;
	Ref<JigsawError> run(const Ref<JigsawCommandList> &commands, const TypedArray<JigsawParameter> &env, const TypedArray<JigsawParameter> &args, const TypedArray<JigsawParameter> &results);
	Ref<JigsawError> create_error(const String &message, const TypedArray<JigsawParameter> &params = TypedArray<JigsawParameter>(), bool include_global_snapshot = false) const;

	static Ref<JigsawContext> make(JigsawGlobal *global, const Ref<JigsawContext> &parent);
};

#endif // JIGSAW_CONTEXT_H
