#include "jigsaw_command_is_same.h"

#include "jigsaw/parameter/jigsaw_parameter_audience.h"
#include "jigsaw/parameter/jigsaw_parameter_audience_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_boolean.h"
#include "jigsaw/parameter/jigsaw_parameter_stat.h"
#include "jigsaw/parameter/jigsaw_parameter_stat_value.h"

void JigsawCommandIsSame::_bind_methods() {
	BIND_PROPERTY_RESOURCE(JigsawParameter, compare_to);
	BIND_PROPERTY_RESOURCE(JigsawParameter, object);
	BIND_PROPERTY_RESOURCE(JigsawParameterLocalVariable, is_same);
}

IMPLEMENT_PROPERTY(JigsawCommandIsSame, Ref<JigsawParameter>, compare_to);
IMPLEMENT_PROPERTY(JigsawCommandIsSame, Ref<JigsawParameter>, object);
IMPLEMENT_PROPERTY(JigsawCommandIsSame, Ref<JigsawParameterLocalVariable>, is_same);

JigsawExecutionState JigsawCommandIsSame::evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const {
	Ref<JigsawParameter> compare_to;
	Ref<JigsawParameter> object;

	err = context->resolve_variable(_compare_to, compare_to, "compare_to");
	if (unlikely(err.is_valid())) {
		return JigsawExecutionState::ERROR;
	}
	err = context->resolve_variable(_object, object, "object");
	if (unlikely(err.is_valid())) {
		return JigsawExecutionState::ERROR;
	}

	if (compare_to->get_type() == JigsawParameter::BOOLEAN) {
		Ref<JigsawParameterBoolean> compare_to_boolean = compare_to;

		if (object->get_type() == JigsawParameter::BOOLEAN) {
			Ref<JigsawParameterBoolean> object_boolean = object;

			return set_command_result(context, err, 0, JigsawParameterBoolean::make(compare_to_boolean->get_boolean() == object_boolean->get_boolean()));
		}
	}

	if (compare_to->get_type() == JigsawParameter::STAT) {
		Ref<JigsawParameterStat> compare_to_stat = compare_to;

		if (object->get_type() == JigsawParameter::STAT) {
			Ref<JigsawParameterStat> object_stat = object;

			return set_command_result(context, err, 0, JigsawParameterBoolean::make(compare_to_stat->get_stat() == object_stat->get_stat()));
		}

		if (object->get_type() == JigsawParameter::STAT_VALUE) {
			Ref<JigsawParameterStatValue> object_stat_value = object;

			return set_command_result(context, err, 0, JigsawParameterBoolean::make(compare_to_stat->get_stat() == object_stat_value->get_stat()));
		}
	}

	if (compare_to->get_type() == JigsawParameter::STAT_VALUE) {
		Ref<JigsawParameterStatValue> compare_to_stat_value = compare_to;

		if (object->get_type() == JigsawParameter::STAT_VALUE) {
			Ref<JigsawParameterStatValue> object_stat_value = object;

			return set_command_result(context, err, 0, JigsawParameterBoolean::make(compare_to_stat_value->get_stat() == object_stat_value->get_stat() && (compare_to_stat_value->is_nan() ? object_stat_value->is_nan() : (!object_stat_value->is_nan() && compare_to_stat_value->get_amount() == object_stat_value->get_amount() && compare_to_stat_value->get_amount_inf() == object_stat_value->get_amount_inf()))));
		}
	}

	if (compare_to->get_type() == JigsawParameter::AUDIENCE) {
		Ref<JigsawParameterAudience> compare_to_audience = compare_to;

		if (object->get_type() == JigsawParameter::AUDIENCE) {
			Ref<JigsawParameterAudience> object_audience = object;

			return set_command_result(context, err, 0, JigsawParameterBoolean::make(compare_to_audience->get_audience() == object_audience->get_audience()));
		}

		if (object->get_type() == JigsawParameter::AUDIENCE_INSTANCE) {
			Ref<JigsawParameterAudienceInstance> object_audience_instance = object;

			if (object_audience_instance->get_group() == -1) {
				return set_command_result(context, err, 0, JigsawParameterBoolean::make(compare_to_audience->get_audience() == enums::AudienceDef::NONE));
			}

			return set_command_result(context, err, 0, JigsawParameterBoolean::make(compare_to_audience->get_audience() == object_audience_instance->get_group() / 2));
		}
	}

	err = context->create_error(vformat("cannot check if %s is the same as %s (let Ben know if you need this fixed)", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawParameter", "Type", compare_to->get_type()), WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawParameter", "Type", object->get_type())));
	return JigsawExecutionState::ERROR;
}

int64_t JigsawCommandIsSame::get_num_arguments() const {
	return 2;
}
Ref<JigsawParameter> JigsawCommandIsSame::get_argument(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 2, Ref<JigsawParameter>());

	if (i == 0) {
		return _compare_to;
	} else if (i == 1) {
		return _object;
	}

	return Ref<JigsawParameter>();
}
TypedArray<JigsawParameter> JigsawCommandIsSame::get_argument_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, 2, TypedArray<JigsawParameter>());

	if (i == 0) {
		// TODO!
	} else if (i == 1) {
		// TODO!
	}

	return TypedArray<JigsawParameter>();
}
void JigsawCommandIsSame::set_argument(int64_t i, const Ref<JigsawParameter> &arg) {
	ERR_FAIL_INDEX(i, 2);

	if (i == 0) {
		_compare_to = arg;
		emit_changed();
	} else if (i == 1) {
		_object = arg;
		emit_changed();
	}
}
String JigsawCommandIsSame::get_argument_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 2, "");

	if (i == 0) {
		return "compare_to";
	} else if (i == 1) {
		return "object";
	}

	return "";
}

int64_t JigsawCommandIsSame::get_num_results() const {
	return 1;
}
Ref<JigsawParameterLocalVariable> JigsawCommandIsSame::get_result(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, Ref<JigsawParameterLocalVariable>());

	return _is_same;
}
TypedArray<JigsawParameter> JigsawCommandIsSame::get_result_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, 1, TypedArray<JigsawParameter>());

	return Array::make(JigsawParameterBoolean::make(false));
}
void JigsawCommandIsSame::set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) {
	ERR_FAIL_INDEX(i, 1);

	_is_same = result;
	emit_changed();
}
String JigsawCommandIsSame::get_result_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "is_same";
}
