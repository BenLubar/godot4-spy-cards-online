#include "jigsaw_command_ordered_list.h"

#include "jigsaw_parameter_amount.h"
#include "jigsaw_parameter_ordered_list.h"

void JigsawCommandOrderedList::_bind_methods() {
	BIND_ENUM_CONSTANT(GET_NUM_ITEMS);

	BIND_PROPERTY_ENUM(JigsawCommandOrderedList::Operation, operation);
	BIND_PROPERTY_RESOURCE(JigsawParameter, list);
	BIND_PROPERTY_RESOURCE(JigsawParameterLocalVariable, output);
}

IMPLEMENT_PROPERTY(JigsawCommandOrderedList, JigsawCommandOrderedList::Operation, operation);
IMPLEMENT_PROPERTY(JigsawCommandOrderedList, Ref<JigsawParameter>, list);
IMPLEMENT_PROPERTY(JigsawCommandOrderedList, Ref<JigsawParameterLocalVariable>, output);

JigsawExecutionState JigsawCommandOrderedList::evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const {
	switch (_operation) {
	case GET_NUM_ITEMS:
	{
		Ref<JigsawParameterOrderedList> list;
		err = context->resolve_variable(_list, list, "list");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		int64_t count = unlikely(list->is_template()) ? 0 : list->get_list().size();

		return set_command_result(context, err, 0, JigsawParameterAmount::make(count));
	}
	}

	err = context->create_error(vformat("internal error: unhandled ordered list operation %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawCommandOrderedList", "Operation", _operation)));
	return JigsawExecutionState::ERROR;
}

int64_t JigsawCommandOrderedList::get_num_configs() const {
	return 1;
}
String JigsawCommandOrderedList::get_config_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "Operation";
}
String JigsawCommandOrderedList::get_config_desc(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "";
}
int64_t JigsawCommandOrderedList::get_config_value(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, 0);

	return _operation;
}
void JigsawCommandOrderedList::set_config_value(int64_t i, int64_t value) {
	ERR_FAIL_INDEX(i, 1);

	_operation = static_cast<Operation>(value);
	emit_changed();
}
PackedStringArray JigsawCommandOrderedList::get_config_options(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, PackedStringArray());

	// If these asserts fail, you have broken compatibility with existing game modes.
	static_assert(GET_NUM_ITEMS == 0);

	return PackedStringArray{
		"Get number of items",
	};
}

int64_t JigsawCommandOrderedList::get_num_arguments() const {
	switch (_operation) {
	case GET_NUM_ITEMS:
		return 1;
	}

	return 0;
}
Ref<JigsawParameter> JigsawCommandOrderedList::get_argument(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), Ref<JigsawParameter>());

	if (i == 0) {
		return _list;
	}

	return Ref<JigsawParameter>();
}
TypedArray<JigsawParameter> JigsawCommandOrderedList::get_argument_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), TypedArray<JigsawParameter>());

	if (i == 0) {
		return Array::make(JigsawParameterOrderedList::make(TypedArray<JigsawParameter>()));
	}

	return TypedArray<JigsawParameter>();
}
void JigsawCommandOrderedList::set_argument(int64_t i, const Ref<JigsawParameter> &arg) {
	ERR_FAIL_INDEX(i, get_num_arguments());

	if (i == 0) {
		_list = arg;
		emit_changed();
		return;
	}
}
String JigsawCommandOrderedList::get_argument_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), "");

	if (i == 0) {
		return "list";
	}

	return "";
}

int64_t JigsawCommandOrderedList::get_num_results() const {
	switch (_operation) {
	case GET_NUM_ITEMS:
		return 1;
	}

	return 0;
}
Ref<JigsawParameterLocalVariable> JigsawCommandOrderedList::get_result(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_results(), Ref<JigsawParameter>());

	if (i == 0) {
		return _output;
	}

	return Ref<JigsawParameter>();
}
TypedArray<JigsawParameter> JigsawCommandOrderedList::get_result_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, get_num_results(), TypedArray<JigsawParameter>());

	switch (_operation) {
	case GET_NUM_ITEMS:
		if (i == 0) {
			return Array::make(JigsawParameterAmount::make(0));
		}

		return TypedArray<JigsawParameter>();
	}

	return TypedArray<JigsawParameter>();
}
void JigsawCommandOrderedList::set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) {
	ERR_FAIL_INDEX(i, get_num_results());

	if (i == 0) {
		_output = result;
		emit_changed();
		return;
	}
}
String JigsawCommandOrderedList::get_result_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_results(), "");

	switch (_operation) {
	case GET_NUM_ITEMS:
		if (i == 0) {
			return "count";
		}

		return "";
	}

	return "";
}

int64_t JigsawCommandOrderedList::get_num_branches() const {
	switch (_operation) {
	case GET_NUM_ITEMS:
		return 0;
	}
}
Ref<JigsawCommandList> JigsawCommandOrderedList::get_branch(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_branches(), Ref<JigsawCommandList>());

	return Ref<JigsawCommandList>();
}
void JigsawCommandOrderedList::set_branch(int64_t i, const Ref<JigsawCommandList> &commands) {
	ERR_FAIL_INDEX(i, get_num_branches());
}
String JigsawCommandOrderedList::get_branch_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_branches(), "");

	return "";
}
TypedArray<Array> JigsawCommandOrderedList::get_branch_argument_templates(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_branches(), TypedArray<Array>());

	return TypedArray<Array>();
}
PackedStringArray JigsawCommandOrderedList::get_branch_argument_names(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_branches(), PackedStringArray());

	return PackedStringArray();
}
TypedArray<Array> JigsawCommandOrderedList::get_branch_result_templates(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, get_num_branches(), TypedArray<Array>());

	return TypedArray<Array>();
}
PackedStringArray JigsawCommandOrderedList::get_branch_result_names(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_branches(), PackedStringArray());

	return PackedStringArray();
}
