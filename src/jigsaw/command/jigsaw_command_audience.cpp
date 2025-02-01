#include "jigsaw/command/jigsaw_command_audience.h"

#include "jigsaw/jigsaw_global.h"
#include "jigsaw/parameter/jigsaw_parameter_audience.h"
#include "jigsaw/parameter/jigsaw_parameter_audience_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_boolean.h"
#include "jigsaw/parameter/jigsaw_parameter_color.h"
#include "jigsaw/parameter/jigsaw_parameter_float.h"

void JigsawCommandAudience::_bind_methods() {
	BIND_ENUM_CONSTANT(CREATE);
	BIND_ENUM_CONSTANT(GET_BY_WEIGHT);
	BIND_ENUM_CONSTANT(GET_TOTAL_WEIGHT);

	BIND_PROPERTY_ENUM(JigsawCommandAudience::Operation, operation);
	BIND_PROPERTY_RESOURCE(JigsawParameter, member);
	BIND_PROPERTY_RESOURCE(JigsawParameter, x);
	BIND_PROPERTY_RESOURCE(JigsawParameter, y);
	BIND_PROPERTY_RESOURCE(JigsawParameter, z);
	BIND_PROPERTY_RESOURCE(JigsawParameter, back);
	BIND_PROPERTY_RESOURCE(JigsawParameter, flip);
	BIND_PROPERTY_RESOURCE(JigsawParameter, left);
	BIND_PROPERTY_RESOURCE(JigsawParameter, color);
	BIND_PROPERTY_RESOURCE(JigsawParameter, excitement);
	BIND_PROPERTY_RESOURCE(JigsawParameter, weight);
	BIND_PROPERTY_RESOURCE(JigsawParameterLocalVariable, output);
}

IMPLEMENT_PROPERTY(JigsawCommandAudience, JigsawCommandAudience::Operation, operation);
IMPLEMENT_PROPERTY(JigsawCommandAudience, Ref<JigsawParameter>, member);
IMPLEMENT_PROPERTY(JigsawCommandAudience, Ref<JigsawParameter>, x);
IMPLEMENT_PROPERTY(JigsawCommandAudience, Ref<JigsawParameter>, y);
IMPLEMENT_PROPERTY(JigsawCommandAudience, Ref<JigsawParameter>, z);
IMPLEMENT_PROPERTY(JigsawCommandAudience, Ref<JigsawParameter>, back);
IMPLEMENT_PROPERTY(JigsawCommandAudience, Ref<JigsawParameter>, flip);
IMPLEMENT_PROPERTY(JigsawCommandAudience, Ref<JigsawParameter>, left);
IMPLEMENT_PROPERTY(JigsawCommandAudience, Ref<JigsawParameter>, color);
IMPLEMENT_PROPERTY(JigsawCommandAudience, Ref<JigsawParameter>, excitement);
IMPLEMENT_PROPERTY(JigsawCommandAudience, Ref<JigsawParameter>, weight);
IMPLEMENT_PROPERTY(JigsawCommandAudience, Ref<JigsawParameterLocalVariable>, output);

bool JigsawCommandAudience::allowed_in_mode(enums::JigsawProcedure::Mode mode, bool any_config) const {
	using namespace enums::JigsawProcedure;

	switch (mode) {
	case FUNCTIONAL:
		return false;
	case INIT:
	case MAIN:
		return true;
	case SELECT:
	case VISUAL:
	case CHOICE_PREVIEW:
		return false;
	case REALTIME_LOGIC:
		return true;
	case REALTIME_VISUAL:
		return false;
	}

	ERR_FAIL_V(false);
}
JigsawExecutionState JigsawCommandAudience::evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const {
	switch (_operation) {
	case CREATE:
	{
		Ref<Audience> audience = context->get_global()->get_state()->get_audience();
		if (unlikely(audience.is_null())) {
			err = context->create_error("internal error: missing audience manager!");
			return JigsawExecutionState::ERROR;
		}

		Ref<JigsawParameterAudience> def;
		err = context->resolve_variable(_member, def, "def");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		if (unlikely(def->get_audience() == enums::AudienceDef::NONE)) {
			err = context->create_error("cannot create audience member with NONE def");
			return JigsawExecutionState::ERROR;
		}

		Ref<JigsawParameterFloat> x, y, z;
		Ref<JigsawParameterBoolean> back, flip, left;
		Ref<JigsawParameterColor> color;
		Ref<JigsawParameterFloat> excitement;

		err = context->resolve_variable(_x, x, "x");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		err = context->resolve_variable(_y, y, "y");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		err = context->resolve_variable(_z, z, "z");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		err = context->resolve_variable(_back, back, "back");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		err = context->resolve_variable(_flip, flip, "flip");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		err = context->resolve_variable(_left, left, "left");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		err = context->resolve_variable(_color, color, "color");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		err = context->resolve_variable(_excitement, excitement, "excitement");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		if (!Math::is_finite(x->get_value()) || !Math::is_finite(y->get_value()) || !Math::is_finite(z->get_value())) {
			err = context->create_error(vformat("audience member position must be finite! got (%f, %f, %f)", x->get_value(), y->get_value(), z->get_value()));
			return JigsawExecutionState::ERROR;
		}

		if (!Math::is_finite(excitement->get_value()) && excitement->get_value() > 0.0) {
			err = context->create_error(vformat("audience member excitement must be finite and positive! got %f", excitement->get_value()));
			return JigsawExecutionState::ERROR;
		}

		int64_t group_number = def->get_audience() * 2;
		if (back->get_boolean()) {
			group_number++;
		}
		int64_t member_index = 0;
		while (member_index < audience->_members[group_number].size() && !audience->_members[group_number][member_index].reusable) {
			member_index++;
		}
		if (member_index >= audience->_members[group_number].size()) {
			audience->_members.write[group_number].append(AudienceMember_t{});
		}

		AudienceMember_t &member = audience->_members.write[group_number].write[member_index];
		member.base_position = Vector3(x->get_value(), y->get_value(), z->get_value());
		member.color = color->get_color();
		member.excitement = excitement->get_value();
		member.want_cheer = 0;
		member.cheering = 0;
		member.hop_time = Math::fast_ftoi(Math::floor(72 - 60 / (2 - Math::clamp(excitement->get_value(), 0.0, 1.0))));
		member.hop = audience->_hop_offset++;
		member.left = left->get_boolean();
		member.flip = flip->get_boolean();
		member.deleted = false;
		member.reusable = false;

		return set_command_result(context, err, 0, JigsawParameterAudienceInstance::make(group_number, member_index));
	}
	case GET_BY_WEIGHT:
	{
		Ref<JigsawParameterFloat> weight_var;
		err = context->resolve_variable(_weight, weight_var, "weight");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		JigsawGlobal *global = context->get_global();
		Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
		TypedArray<AudienceDef> audience = likely(mode.is_valid()) ? mode->get_audience() : TypedArray<AudienceDef>();

		double weight = weight_var->get_value();
		for (int64_t i = 0; i < audience.size(); i++) {
			Ref<AudienceDef> member = audience[i];
			if (likely(member.is_valid())) {
				weight -= member->get_weight();
				if (weight < 0.0) {
					return set_command_result(context, err, 0, JigsawParameterAudience::make(static_cast<enums::AudienceDef::Audience>(i + enums::AudienceDef::FIRST_CUSTOM)));
				}
			}
		}

		// fall back to the last audience member type if the weight was too big (rounding error?)
		return set_command_result(context, err, 0, JigsawParameterAudience::make(static_cast<enums::AudienceDef::Audience>(audience.size() - 1 + enums::AudienceDef::FIRST_CUSTOM)));
	}
	case GET_TOTAL_WEIGHT:
	{
		JigsawGlobal *global = context->get_global();
		Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
		TypedArray<AudienceDef> audience = likely(mode.is_valid()) ? mode->get_audience() : TypedArray<AudienceDef>();

		double total_weight = 0.0;
		for (int64_t i = 0; i < audience.size(); i++) {
			Ref<AudienceDef> member = audience[i];
			if (likely(member.is_valid())) {
				total_weight += member->get_weight();
			}
		}

		return set_command_result(context, err, 0, JigsawParameterFloat::make(total_weight));
	}
	}

	err = context->create_error(vformat("internal error: unhandled audience operation %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawCommandAudience", "Operation", _operation)));
	return JigsawExecutionState::ERROR;
}

int64_t JigsawCommandAudience::get_num_configs() const {
	return 1;
}
String JigsawCommandAudience::get_config_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "Operation";
}
String JigsawCommandAudience::get_config_desc(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "";
}
int64_t JigsawCommandAudience::get_config_value(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, 0);

	return _operation;
}
void JigsawCommandAudience::set_config_value(int64_t i, int64_t value) {
	ERR_FAIL_INDEX(i, 1);

	_operation = static_cast<Operation>(value);
	emit_changed();
}
PackedStringArray JigsawCommandAudience::get_config_options(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, PackedStringArray());

	// If these asserts fail, you have broken compatibility with existing game modes.
	static_assert(CREATE == 0);
	static_assert(GET_BY_WEIGHT == 1);
	static_assert(GET_TOTAL_WEIGHT == 2);

	return PackedStringArray{
		"Create audience member",
		"Get member type by weight",
		"Get total weight",
	};
}

int64_t JigsawCommandAudience::get_num_arguments() const {
	switch (_operation) {
	case CREATE:
		return 9;
	case GET_BY_WEIGHT:
		return 1;
	case GET_TOTAL_WEIGHT:
		return 0;
	}

	return 0;
}
Ref<JigsawParameter> JigsawCommandAudience::get_argument(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), Ref<JigsawParameter>());

	switch (_operation) {
	case CREATE:
		if (i == 0) {
			return _member;
		} else if (i == 1) {
			return _x;
		} else if (i == 2) {
			return _y;
		} else if (i == 3) {
			return _z;
		} else if (i == 4) {
			return _back;
		} else if (i == 5) {
			return _flip;
		} else if (i == 6) {
			return _left;
		} else if (i == 7) {
			return _color;
		} else if (i == 8) {
			return _excitement;
		}
		break;
	case GET_BY_WEIGHT:
		return _weight;
		break;
	case GET_TOTAL_WEIGHT:
		break;
	}

	return Ref<JigsawParameter>();
}
TypedArray<JigsawParameter> JigsawCommandAudience::get_argument_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), TypedArray<JigsawParameter>());
	switch (_operation) {
	case CREATE:
		if (i == 0) {
			return Array::make(JigsawParameterAudience::make(enums::AudienceDef::NONE));
		} else if (i == 1) {
			return Array::make(JigsawParameterFloat::make(0.0));
		} else if (i == 2) {
			return Array::make(JigsawParameterFloat::make(0.0));
		} else if (i == 3) {
			return Array::make(JigsawParameterFloat::make(0.0));
		} else if (i == 4) {
			return Array::make(JigsawParameterBoolean::make(false));
		} else if (i == 5) {
			return Array::make(JigsawParameterBoolean::make(false));
		} else if (i == 6) {
			return Array::make(JigsawParameterBoolean::make(false));
		} else if (i == 7) {
			return Array::make(JigsawParameterColor::make(Color()));
		} else if (i == 8) {
			return Array::make(JigsawParameterFloat::make(0.0));
		}
		break;
	case GET_BY_WEIGHT:
		return Array::make(JigsawParameterFloat::make(0.0));
	case GET_TOTAL_WEIGHT:
		break;
	}

	return TypedArray<JigsawParameter>();
}
void JigsawCommandAudience::set_argument(int64_t i, const Ref<JigsawParameter> &arg) {
	ERR_FAIL_INDEX(i, get_num_arguments());

	switch (_operation) {
	case CREATE:
		if (i == 0) {
			_member = arg;
			emit_changed();
		} else if (i == 1) {
			_x = arg;
			emit_changed();
		} else if (i == 2) {
			_y = arg;
			emit_changed();
		} else if (i == 3) {
			_z = arg;
			emit_changed();
		} else if (i == 4) {
			_back = arg;
			emit_changed();
		} else if (i == 5) {
			_flip = arg;
			emit_changed();
		} else if (i == 6) {
			_left = arg;
			emit_changed();
		} else if (i == 7) {
			_color = arg;
			emit_changed();
		} else if (i == 8) {
			_excitement = arg;
			emit_changed();
		}
		break;
	case GET_BY_WEIGHT:
		_weight = arg;
		emit_changed();
		break;
	case GET_TOTAL_WEIGHT:
		break;
	}
}
String JigsawCommandAudience::get_argument_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), "");


	switch (_operation) {
	case CREATE:
		if (i == 0) {
			return "def";
		} else if (i == 1) {
			return "x";
		} else if (i == 2) {
			return "y";
		} else if (i == 3) {
			return "z";
		} else if (i == 4) {
			return "back";
		} else if (i == 5) {
			return "flip";
		} else if (i == 6) {
			return "left";
		} else if (i == 7) {
			return "color";
		} else if (i == 8) {
			return "excitement";
		}
		break;
	case GET_BY_WEIGHT:
		return "weight";
	case GET_TOTAL_WEIGHT:
		break;
	}

	return "";
}

int64_t JigsawCommandAudience::get_num_results() const {
	switch (_operation) {
	case CREATE:
	case GET_BY_WEIGHT:
	case GET_TOTAL_WEIGHT:
		return 1;
	}

	return 0;
}
Ref<JigsawParameterLocalVariable> JigsawCommandAudience::get_result(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_results(), Ref<JigsawParameterLocalVariable>());

	return _output;
}
TypedArray<JigsawParameter> JigsawCommandAudience::get_result_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, get_num_results(), TypedArray<JigsawParameter>());

	switch (_operation) {
	case CREATE:
		return Array::make(JigsawParameterAudienceInstance::make(-1, -1));
	case GET_BY_WEIGHT:
		return Array::make(JigsawParameterAudience::make(enums::AudienceDef::NONE));
	case GET_TOTAL_WEIGHT:
		return Array::make(JigsawParameterFloat::make(0.0));
	}

	return TypedArray<JigsawParameter>();
}
bool JigsawCommandAudience::is_result_required(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_results(), true);

	switch (_operation) {
	case CREATE:
		return false;
	default:
		return true;
	}
}
void JigsawCommandAudience::set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) {
	ERR_FAIL_INDEX(i, get_num_results());

	_output = result;
	emit_changed();
}
String JigsawCommandAudience::get_result_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_results(), "");

	switch (_operation) {
	case CREATE:
		return "member";
	case GET_BY_WEIGHT:
		return "def";
	case GET_TOTAL_WEIGHT:
		return "total_weight";
	}

	return "";
}
