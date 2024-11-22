#include "jigsaw_command_crash.h"

#include "jigsaw_parameter_string.h"
#include "jigsaw_parameter_ordered_list.h"

void JigsawCommandCrash::_bind_methods() {
	BIND_PROPERTY_RESOURCE(JigsawParameter, message);
	BIND_PROPERTY_RESOURCE(JigsawParameter, params);
}

IMPLEMENT_PROPERTY(JigsawCommandCrash, Ref<JigsawParameter>, message);
IMPLEMENT_PROPERTY(JigsawCommandCrash, Ref<JigsawParameter>, params);

JigsawExecutionState JigsawCommandCrash::evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const {
	Ref<JigsawParameterString> message;
	err = context->resolve_variable(_message, message, "message");
	if (unlikely(err.is_valid())) {
		return JigsawExecutionState::ERROR;
	}

	TypedArray<JigsawParameter> params;
	if (_params.is_valid()) {
		Ref<JigsawParameterOrderedList> params_param;
		err = context->resolve_variable(_params, params_param, "params");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		params = params_param->get_list();
	}

	// wrap custom error messages in quotes to tell them apart from internal errors
	err = context->create_error("\"" + message->get_string() + "\"", params);
	return JigsawExecutionState::ERROR;
}

int64_t JigsawCommandCrash::get_num_arguments() const {
	return 2;
}
Ref<JigsawParameter> JigsawCommandCrash::get_argument(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 2, Ref<JigsawParameter>());

	if (i == 0) {
		return _message;
	} else if (i == 1) {
		return _params;
	}

	return Ref<JigsawParameter>();
}
TypedArray<JigsawParameter> JigsawCommandCrash::get_argument_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, 2, TypedArray<JigsawParameter>());

	if (i == 0) {
		return Array::make(JigsawParameterString::make(""));
	} else if (i == 1) {
		return Array::make(JigsawParameterOrderedList::make(TypedArray<JigsawParameter>()));
	}

	return TypedArray<JigsawParameter>();
}
bool JigsawCommandCrash::is_argument_required(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 2, false);

	// message required; params not
	return i == 0;
}
void JigsawCommandCrash::set_argument(int64_t i, const Ref<JigsawParameter> &arg) {
	ERR_FAIL_INDEX(i, 2);

	if (i == 0) {
		_message = arg;
	} else if (i == 1) {
		_params = arg;
	}

}
String JigsawCommandCrash::get_argument_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 2, "");

	if (i == 0) {
		return "message";
	} else if (i == 1) {
		return "params";
	}

	return "";
}
