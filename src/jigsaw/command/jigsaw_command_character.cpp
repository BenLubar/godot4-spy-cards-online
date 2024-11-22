#include "jigsaw_command_character.h"

#include "jigsaw/jigsaw_global.h"
#include "jigsaw/parameter/jigsaw_parameter_amount.h"
#include "jigsaw/parameter/jigsaw_parameter_boolean.h"
#include "jigsaw/parameter/jigsaw_parameter_character.h"
#include "jigsaw/parameter/jigsaw_parameter_character_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_float.h"
#include "jigsaw/jigsaw_visual.h"

#include <godot_cpp/classes/sub_viewport.hpp>

void JigsawCommandCharacter::_bind_methods() {
	BIND_ENUM_CONSTANT(CREATE);

	BIND_PROPERTY_ENUM(JigsawCommandCharacter::Operation, operation);
	BIND_PROPERTY_RESOURCE(JigsawParameter, def);
	BIND_PROPERTY_RESOURCE(JigsawParameter, x);
	BIND_PROPERTY_RESOURCE(JigsawParameter, y);
	BIND_PROPERTY_RESOURCE(JigsawParameter, z);
	BIND_PROPERTY_RESOURCE(JigsawParameter, flip);
	BIND_PROPERTY_RESOURCE(JigsawParameter, player_number);
	BIND_PROPERTY_RESOURCE(JigsawParameterLocalVariable, output);
}

IMPLEMENT_PROPERTY(JigsawCommandCharacter, JigsawCommandCharacter::Operation, operation);
IMPLEMENT_PROPERTY(JigsawCommandCharacter, Ref<JigsawParameter>, def);
IMPLEMENT_PROPERTY(JigsawCommandCharacter, Ref<JigsawParameter>, x);
IMPLEMENT_PROPERTY(JigsawCommandCharacter, Ref<JigsawParameter>, y);
IMPLEMENT_PROPERTY(JigsawCommandCharacter, Ref<JigsawParameter>, z);
IMPLEMENT_PROPERTY(JigsawCommandCharacter, Ref<JigsawParameter>, flip);
IMPLEMENT_PROPERTY(JigsawCommandCharacter, Ref<JigsawParameter>, player_number);
IMPLEMENT_PROPERTY(JigsawCommandCharacter, Ref<JigsawParameterLocalVariable>, output);

JigsawExecutionState JigsawCommandCharacter::evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const {
	static LazyGlobal<GDScript> character_sprite_script([]() -> Ref<GDScript> { return ResourceLoader::get_singleton()->load("res://stage/character_sprite.gd", "GDScript"); });

	switch (_operation) {
	case CREATE:
	{
		Ref<JigsawParameterCharacter> def;
		Ref<JigsawParameterFloat> x, y, z;
		Ref<JigsawParameterBoolean> flip;
		Ref<JigsawParameterAmount> player_number;
		err = context->resolve_variable(_def, def, "def");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
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
		err = context->resolve_variable(_flip, flip, "flip");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}
		err = context->resolve_variable(_player_number, player_number, "player_number");
		if (unlikely(err.is_valid())) {
			return JigsawExecutionState::ERROR;
		}

		JigsawGlobal *global = context->get_global();
		Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
		TypedArray<MeshInstance3D> character_nodes = likely(global) ? global->get_character_nodes() : TypedArray<MeshInstance3D>();

		int64_t i = character_nodes.size();

		if (likely(mode.is_valid())) {
			if (unlikely(def->get_character() < 0 || def->get_character() >= mode->get_characters().size())) {
				err = context->create_error(vformat("invalid character id %d", def->get_character()));
				return JigsawExecutionState::ERROR;
			}

			MeshInstance3D *sprite = Object::cast_to<MeshInstance3D>(character_sprite_script->new_());

			JigsawVisual *visual = global->get_visual();
			if (likely(visual)) {
				visual->get_stage_viewport()->add_child(sprite);
			}

			sprite->set("mode", mode);
			sprite->set("def", mode->get_characters()[def->get_character()]);
			sprite->set("player_number", player_number->is_nan() || player_number->get_amount_inf() != 0 ? 0 : player_number->get_amount());
			sprite->set("flip", flip->get_boolean());
			sprite->set_position(Vector3(x->get_value(), y->get_value(), z->get_value()));

			character_nodes.append(sprite);

			global->set_character_nodes(character_nodes);
		} else {
			i = -1;
		}

		return set_command_result(context, err, 0, JigsawParameterCharacterInstance::make(i));
	}
	}

	err = context->create_error(vformat("internal error: unhandled character operation %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawCommandCharacter", "Operation", _operation)));
	return JigsawExecutionState::ERROR;
}

int64_t JigsawCommandCharacter::get_num_configs() const {
	return 1;
}
String JigsawCommandCharacter::get_config_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "Operation";
}
String JigsawCommandCharacter::get_config_desc(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "";
}
int64_t JigsawCommandCharacter::get_config_value(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, 0);

	return _operation;
}
void JigsawCommandCharacter::set_config_value(int64_t i, int64_t value) {
	ERR_FAIL_INDEX(i, 1);

	_operation = static_cast<Operation>(value);
	emit_changed();
}
PackedStringArray JigsawCommandCharacter::get_config_options(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, PackedStringArray());

	// If these asserts fail, you have broken compatibility with existing game modes.
	static_assert(CREATE == 0);

	return PackedStringArray{
		"Create",
	};
}

int64_t JigsawCommandCharacter::get_num_arguments() const {
	switch (_operation) {
	case CREATE:
		return 6;
	}

	return 0;
}
Ref<JigsawParameter> JigsawCommandCharacter::get_argument(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), Ref<JigsawParameter>());

	switch (_operation) {
	case CREATE:
		if (i == 0) {
			return _def;
		} else if (i == 1) {
			return _x;
		} else if (i == 2) {
			return _y;
		} else if (i == 3) {
			return _z;
		} else if (i == 4) {
			return _flip;
		} else if (i == 5) {
			return _player_number;
		}
		break;
	}

	return Ref<JigsawParameter>();
}
TypedArray<JigsawParameter> JigsawCommandCharacter::get_argument_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, get_num_arguments(), TypedArray<JigsawParameter>());

	switch (_operation) {
	case CREATE:
		if (i == 0) {
			return Array::make(JigsawParameterCharacter::make(enums::CharacterDef::NONE));
		} else if (i == 1) {
			return Array::make(JigsawParameterFloat::make(0.0));
		} else if (i == 2) {
			return Array::make(JigsawParameterFloat::make(0.0));
		} else if (i == 3) {
			return Array::make(JigsawParameterFloat::make(0.0));
		} else if (i == 4) {
			return Array::make(JigsawParameterBoolean::make(false));
		} else if (i == 5) {
			return Array::make(JigsawParameterAmount::make(0));
		}
		break;
	}

	return TypedArray<JigsawParameter>();
}
void JigsawCommandCharacter::set_argument(int64_t i, const Ref<JigsawParameter> &arg) {
	ERR_FAIL_INDEX(i, get_num_arguments());

	switch (_operation) {
	case CREATE:
		if (i == 0) {
			_def = arg;
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
			_flip = arg;
			emit_changed();
		} else if (i == 5) {
			_player_number = arg;
			emit_changed();
		}
		break;
	}
}
String JigsawCommandCharacter::get_argument_name(int64_t i) const {
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
			return "flip";
		} else if (i == 5) {
			return "player_number";
		}
		break;
	}

	return "";
}

int64_t JigsawCommandCharacter::get_num_results() const {
	switch (_operation) {
	case CREATE:
		return 1;
	}

	return 0;
}
Ref<JigsawParameterLocalVariable> JigsawCommandCharacter::get_result(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_results(), Ref<JigsawParameterLocalVariable>());

	return _output;
}
TypedArray<JigsawParameter> JigsawCommandCharacter::get_result_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, get_num_results(), TypedArray<JigsawParameter>());

	switch (_operation) {
	case CREATE:
		return Array::make(JigsawParameterCharacterInstance::make(-1));
	}

	return TypedArray<JigsawParameter>();
}
void JigsawCommandCharacter::set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) {
	ERR_FAIL_INDEX(i, get_num_results());

	_output = result;
	emit_changed();
}
String JigsawCommandCharacter::get_result_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, get_num_results(), "");

	switch (_operation) {
	case CREATE:
		return "sprite";
	}

	return "";
}
