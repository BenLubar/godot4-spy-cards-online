#include "jigsaw/command/jigsaw_command_set_variable.h"

#include "jigsaw/parameter/jigsaw_parameter_variable.h"

void JigsawCommandSetVariable::_bind_methods() {
	BIND_PROPERTY_IS(Variant::BOOL, persistent);
	BIND_PROPERTY_RESOURCE(JigsawParameter, value);
	BIND_PROPERTY_RESOURCE(JigsawParameter, variable);
}

IMPLEMENT_PROPERTY_IS(JigsawCommandSetVariable, bool, persistent);
IMPLEMENT_PROPERTY(JigsawCommandSetVariable, Ref<JigsawParameter>, value);
IMPLEMENT_PROPERTY(JigsawCommandSetVariable, Ref<JigsawParameter>, variable);

bool JigsawCommandSetVariable::allowed_in_mode(enums::JigsawProcedure::Mode mode, bool any_config) const {
	using namespace enums::JigsawProcedure;

	if (any_config) {
		return true;
	}

	switch (mode) {
	case FUNCTIONAL:
		return !_persistent;
	case INIT:
	case MAIN:
		return true;
	case SELECT:
	case VISUAL:
	case CHOICE_PREVIEW:
		return !_persistent;
	case REALTIME_LOGIC:
		return true;
	case REALTIME_VISUAL:
		return !_persistent;
	}

	ERR_FAIL_V(false);
}
JigsawExecutionState JigsawCommandSetVariable::evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const {
	Ref<JigsawParameter> value;
	err = context->resolve_variable(_value, value, "value");
	if (unlikely(err.is_valid())) {
		return JigsawExecutionState::ERROR;
	}

	if (_persistent) {
		Ref<JigsawParameterVariable> variable = _variable;
		if (unlikely(variable.is_null())) {
			err = context->create_error("missing variable in set variable command");
			return JigsawExecutionState::ERROR;
		}

		err = context->set_persistent_variable(variable, value, "variable");
		return unlikely(err.is_valid()) ? JigsawExecutionState::ERROR : JigsawExecutionState::CONTINUE;
	} else {
		Ref<JigsawParameterLocalVariable> variable = _variable;

		err = context->set_local_variable(variable, value, "variable");
		return unlikely(err.is_valid()) ? JigsawExecutionState::ERROR : JigsawExecutionState::CONTINUE;
	}
}

int64_t JigsawCommandSetVariable::get_num_configs() const {
	return 1;
}
String JigsawCommandSetVariable::get_config_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "Variable type";
}
String JigsawCommandSetVariable::get_config_desc(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "";
}
int64_t JigsawCommandSetVariable::get_config_value(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, 0);

	return _persistent ? 0 : 1;
}
void JigsawCommandSetVariable::set_config_value(int64_t i, int64_t value) {
	ERR_FAIL_INDEX(i, 1);

	_persistent = value == 0;
	emit_changed();
}
PackedStringArray JigsawCommandSetVariable::get_config_options(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, PackedStringArray());

	return PackedStringArray{"Persistent", "Local"};
}

int64_t JigsawCommandSetVariable::get_num_arguments() const {
	return 2;
}
Ref<JigsawParameter> JigsawCommandSetVariable::get_argument(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 2, Ref<JigsawParameter>());

	if (i == 0) {
		return _variable;
	} else if (i == 1) {
		return _value;
	}

	return Ref<JigsawParameter>();
}
TypedArray<JigsawParameter> JigsawCommandSetVariable::get_argument_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, 2, TypedArray<JigsawParameter>());

	if (i == 0) {
		return _persistent ? Array::make(JigsawParameterVariable::make(enums::VariableDef::NONE)) : Array::make(JigsawParameterLocalVariable::make(0, -1));
	} else if (i == 1) {
		// TODO: determine variable type hint
	}

	return TypedArray<JigsawParameter>();
}
void JigsawCommandSetVariable::set_argument(int64_t i, const Ref<JigsawParameter> &arg) {
	ERR_FAIL_INDEX(i, 2);

	if (i == 0) {
		_variable = arg;
		emit_changed();
	} else if (i == 1) {
		_value = arg;
		emit_changed();
	}
}
String JigsawCommandSetVariable::get_argument_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 2, "");

	if (i == 0) {
		return "variable";
	} else if (i == 1) {
		return "value";
	}

	return "";
}
