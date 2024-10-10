#include "jigsaw_context.h"
#include "jigsaw_global.h"

void JigsawContext::_bind_methods() {
	BIND_PROPERTY_RESOURCE(JigsawGlobal, global);
	BIND_PROPERTY_RESOURCE(JigsawContext, parent);
	BIND_PROPERTY(Variant::BOOL, read_only);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawStackFrame, stack);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawCommandList, command_stack);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawParameter, environment);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawParameter, arguments);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawParameter, results);

	ClassDB::bind_method(D_METHOD("validate", "commands", "env", "args", "results", "debug_name", "arg_names", "result_names"), &JigsawContext::validate, DEFVAL(PackedStringArray()), DEFVAL(PackedStringArray()));
	ClassDB::bind_method(D_METHOD("run", "commands", "env", "args", "results"), &JigsawContext::run);
	ClassDB::bind_method(D_METHOD("create_error", "message", "params", "include_global_snapshot"), &JigsawContext::create_error, DEFVAL(TypedArray<JigsawParameter>()), DEFVAL(false));

	ClassDB::bind_static_method("JigsawContext", D_METHOD("make", "global", "parent"), &JigsawContext::make, DEFVAL(Ref<JigsawContext>()));
}

IMPLEMENT_PROPERTY_SIMPLE(JigsawContext, JigsawGlobal *, global);
IMPLEMENT_PROPERTY_SIMPLE(JigsawContext, Ref<JigsawContext>, parent);
IMPLEMENT_PROPERTY_SIMPLE(JigsawContext, bool, read_only);
IMPLEMENT_PROPERTY_SIMPLE(JigsawContext, TypedArray<JigsawStackFrame>, stack);
IMPLEMENT_PROPERTY_SIMPLE(JigsawContext, TypedArray<JigsawCommandList>, command_stack);
IMPLEMENT_PROPERTY_SIMPLE(JigsawContext, TypedArray<JigsawParameter>, environment);
IMPLEMENT_PROPERTY_SIMPLE(JigsawContext, TypedArray<JigsawParameter>, arguments);
IMPLEMENT_PROPERTY_SIMPLE(JigsawContext, TypedArray<JigsawParameter>, results);

TypedArray<JigsawError> JigsawContext::validate(const Ref<JigsawCommandList> &commands, const TypedArray<JigsawParameter> &env, const TypedArray<JigsawParameter> &args, const TypedArray<JigsawParameter> &results, const String &debug_name, const PackedStringArray &arg_names, const PackedStringArray &result_names) const {
	TypedArray<JigsawError> errors;
	errors.append(create_error("TODO"));
	return errors;
}
Ref<JigsawError> JigsawContext::run(const Ref<JigsawCommandList> &commands, const TypedArray<JigsawParameter> &env, const TypedArray<JigsawParameter> &args, const TypedArray<JigsawParameter> &results) {
	return create_error("TODO");
}
Ref<JigsawError> JigsawContext::create_error(const String &message, const TypedArray<JigsawParameter> &params, bool include_global_snapshot) const {
	Ref<JigsawError> error;
	error.instantiate();

	if (include_global_snapshot) {
		error->flags1.set_flag(JigsawError::CONTAINS_GLOBAL_SNAPSHOT);
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
