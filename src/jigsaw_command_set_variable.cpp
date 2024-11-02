#include "jigsaw_command_set_variable.h"

#include "jigsaw_parameter_variable.h"

void JigsawCommandSetVariable::_bind_methods() {
	BIND_PROPERTY_IS(Variant::BOOL, persistent);
	BIND_PROPERTY_RESOURCE(JigsawParameter, value);
	BIND_PROPERTY_RESOURCE(JigsawParameter, variable);
}

IMPLEMENT_PROPERTY_IS(JigsawCommandSetVariable, bool, persistent);
IMPLEMENT_PROPERTY(JigsawCommandSetVariable, Ref<JigsawParameter>, value);
IMPLEMENT_PROPERTY(JigsawCommandSetVariable, Ref<JigsawParameter>, variable);

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
bool JigsawCommandSetVariable::is_argument_unresolved(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 2, false);

	return i == 0;
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
