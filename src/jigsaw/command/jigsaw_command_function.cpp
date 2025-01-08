#include "jigsaw_command_function.h"

void JigsawCommandFunction::_bind_methods() {
	BIND_PROPERTY_RESOURCE_NOT_SAVED(JigsawGlobal, editor_global);
	BIND_PROPERTY(Variant::INT, function_index);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawParameter, args);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawParameterLocalVariable, results);
}

Ref<JigsawFunction> JigsawCommandFunction::get_function() const {
	ERR_FAIL_NULL_V(_editor_global, Ref<JigsawFunction>());
	Ref<GameMode> mode = _editor_global->get_mode();
	ERR_FAIL_COND_V(mode.is_null(), Ref<JigsawFunction>());
	TypedArray<JigsawFunction> functions = mode->get_custom_functions();
	ERR_FAIL_INDEX_V(_function_index, functions.size(), Ref<JigsawFunction>());

	return functions[_function_index];
}

IMPLEMENT_PROPERTY_SIMPLE(JigsawCommandFunction, JigsawGlobal *, editor_global);
IMPLEMENT_PROPERTY(JigsawCommandFunction, int64_t, function_index);
IMPLEMENT_PROPERTY(JigsawCommandFunction, TypedArray<JigsawParameter>, args);
IMPLEMENT_PROPERTY(JigsawCommandFunction, TypedArray<JigsawParameterLocalVariable>, results);

bool JigsawCommandFunction::allowed_in_mode(enums::JigsawProcedure::Mode mode, bool any_config) const {
	if (any_config) {
		return true;
	}

	Ref<JigsawFunction> function = get_function();
	ERR_FAIL_COND_V(function.is_null(), false);

	return mode == function->get_mode();
}
JigsawExecutionState JigsawCommandFunction::evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const {
	err = context->create_error("internal error: TODO (function)");

	return JigsawExecutionState::ERROR;
}

int64_t JigsawCommandFunction::get_num_configs() const {
	ERR_FAIL_NULL_V(_editor_global, false);
	Ref<GameMode> mode = _editor_global->get_mode();
	ERR_FAIL_COND_V(mode.is_null(), false);
	TypedArray<JigsawFunction> functions = mode->get_custom_functions();

	return functions.is_empty() ? 0 : 1;
}
String JigsawCommandFunction::get_config_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "Function";
}
String JigsawCommandFunction::get_config_desc(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	Ref<JigsawFunction> func = get_function();
	ERR_FAIL_COND_V(func.is_null(), "");

	return func->get_editor_description();
}
int64_t JigsawCommandFunction::get_config_value(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, 0);

	return _function_index;
}
void JigsawCommandFunction::set_config_value(int64_t i, int64_t value) {
	ERR_FAIL_INDEX(i, 1);

	_function_index = value;
	emit_changed();
}
PackedStringArray JigsawCommandFunction::get_config_options(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, PackedStringArray());

	ERR_FAIL_NULL_V(_editor_global, PackedStringArray());
	Ref<GameMode> mode = _editor_global->get_mode();
	ERR_FAIL_COND_V(mode.is_null(), PackedStringArray());
	TypedArray<JigsawFunction> functions = mode->get_custom_functions();

	PackedStringArray names;
	names.resize(functions.size());

	for (int64_t j = 0; j < functions.size(); j++) {
		Ref<JigsawFunction> func = functions[j];
		ERR_CONTINUE(func.is_null());

		names[j] = func->get_editor_name();
	}

	return names;
}

int64_t JigsawCommandFunction::get_num_arguments() const {
	Ref<JigsawFunction> func = get_function();
	ERR_FAIL_COND_V(func.is_null(), 0);

	return func->get_arguments().size();
}
Ref<JigsawParameter> JigsawCommandFunction::get_argument(int64_t i) const {
	return _args.size() > i ? _args[i] : Variant();
}
TypedArray<JigsawParameter> JigsawCommandFunction::get_argument_template(int64_t i, const Ref<JigsawContext> &context) const {
	Ref<JigsawFunction> func = get_function();
	ERR_FAIL_COND_V(func.is_null(), TypedArray<JigsawParameter>());

	TypedArray<JigsawParameter> args = func->get_arguments();
	ERR_FAIL_INDEX_V(i, args.size(), TypedArray<JigsawParameter>());

	return Array::make(args[i]);
}
void JigsawCommandFunction::set_argument(int64_t i, const Ref<JigsawParameter> &arg) {
	int64_t num_args = get_num_arguments();
	ERR_FAIL_INDEX(i, num_args);

	_args.resize(num_args);
	_args[i] = arg;
	emit_changed();
}
String JigsawCommandFunction::get_argument_name(int64_t i) const {
	Ref<JigsawFunction> func = get_function();
	ERR_FAIL_COND_V(func.is_null(), "");

	PackedStringArray names = func->get_argument_names();

	return names.size() > i ? names[i] : vformat("arg%d", i);
}

int64_t JigsawCommandFunction::get_num_results() const {
	Ref<JigsawFunction> func = get_function();
	ERR_FAIL_COND_V(func.is_null(), 0);

	return func->get_results().size();
}
Ref<JigsawParameterLocalVariable> JigsawCommandFunction::get_result(int64_t i) const {
	return _results.size() > i ? _results[i] : Variant();
}
TypedArray<JigsawParameter> JigsawCommandFunction::get_result_template(int64_t i, const Ref<JigsawContext> &context) const {
	Ref<JigsawFunction> func = get_function();
	ERR_FAIL_COND_V(func.is_null(), TypedArray<JigsawParameter>());

	TypedArray<JigsawParameter> results = func->get_results();
	ERR_FAIL_INDEX_V(i, results.size(), TypedArray<JigsawParameter>());

	return Array::make(results[i]);
}
void JigsawCommandFunction::set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) {
	int64_t num_results = get_num_results();
	ERR_FAIL_INDEX(i, num_results);

	_results.resize(num_results);
	_results[i] = result;
	emit_changed();
}
String JigsawCommandFunction::get_result_name(int64_t i) const {
	Ref<JigsawFunction> func = get_function();
	ERR_FAIL_COND_V(func.is_null(), "");

	PackedStringArray names = func->get_result_names();

	return names.size() > i ? names[i] : vformat("result%d", i);
}
