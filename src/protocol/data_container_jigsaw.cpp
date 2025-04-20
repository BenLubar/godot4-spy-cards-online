#include "protocol/data_container.h"

#include <godot_cpp/classes/json.hpp>

#include "jigsaw/command/jigsaw_command_crash.h"
#include "jigsaw/command/jigsaw_command_comment.h"
#include "jigsaw/command/jigsaw_command_function.h"
#include "jigsaw/command/jigsaw_command_math.h"
#include "jigsaw/command/jigsaw_command_log.h"
#include "jigsaw/command/jigsaw_command_format_text.h"
#include "jigsaw/command/jigsaw_command_if.h"
#include "jigsaw/command/jigsaw_command_set_variable.h"
#include "jigsaw/command/jigsaw_command_lookup_definition_property.h"
#include "jigsaw/command/jigsaw_command_is_same.h"
#include "jigsaw/command/jigsaw_command_ordered_list.h"
#include "jigsaw/command/jigsaw_command_rng.h"
#include "jigsaw/command/jigsaw_command_audience.h"
#include "jigsaw/command/jigsaw_command_character.h"
#include "jigsaw/command/jigsaw_command_audio.h"
#include "jigsaw/command/jigsaw_command_scene.h"
#include "jigsaw/command/jigsaw_command_sprite.h"
#include "jigsaw/command/jigsaw_command_camera.h"
#include "jigsaw/command/jigsaw_command_ui.h"
#include "jigsaw/command/jigsaw_command_loop.h"
#include "jigsaw/command/jigsaw_command_color.h"
#include "jigsaw/command/jigsaw_command_cleanup.h"

#include "jigsaw/parameter/jigsaw_parameter_amount.h"
#include "jigsaw/parameter/jigsaw_parameter_audience.h"
#include "jigsaw/parameter/jigsaw_parameter_audience_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_boolean.h"
#include "jigsaw/parameter/jigsaw_parameter_card.h"
#include "jigsaw/parameter/jigsaw_parameter_card_filter.h"
#include "jigsaw/parameter/jigsaw_parameter_card_grid.h"
#include "jigsaw/parameter/jigsaw_parameter_card_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_character.h"
#include "jigsaw/parameter/jigsaw_parameter_character_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_choice.h"
#include "jigsaw/parameter/jigsaw_parameter_cid_opus.h"
#include "jigsaw/parameter/jigsaw_parameter_color.h"
#include "jigsaw/parameter/jigsaw_parameter_effect_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_effect_instance_parameter.h"
#include "jigsaw/parameter/jigsaw_parameter_expression.h"
#include "jigsaw/parameter/jigsaw_parameter_file_id_gltf.h"
#include "jigsaw/parameter/jigsaw_parameter_file_id_opus.h"
#include "jigsaw/parameter/jigsaw_parameter_float.h"
#include "jigsaw/parameter/jigsaw_parameter_formatted_text.h"
#include "jigsaw/parameter/jigsaw_parameter_icon.h"
#include "jigsaw/parameter/jigsaw_parameter_local_variable.h"
#include "jigsaw/parameter/jigsaw_parameter_location.h"
#include "jigsaw/parameter/jigsaw_parameter_modifier.h"
#include "jigsaw/parameter/jigsaw_parameter_modifier_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_ordered_list.h"
#include "jigsaw/parameter/jigsaw_parameter_queued_effect.h"
#include "jigsaw/parameter/jigsaw_parameter_rank.h"
#include "jigsaw/parameter/jigsaw_parameter_scene_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_sound_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_sprite_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_stat.h"
#include "jigsaw/parameter/jigsaw_parameter_stat_value.h"
#include "jigsaw/parameter/jigsaw_parameter_string.h"
#include "jigsaw/parameter/jigsaw_parameter_tribe.h"
#include "jigsaw/parameter/jigsaw_parameter_ui_icon_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_ui_label_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_variable.h"

bool DataContainer::_decode_jigsaw_function(const Ref<FormatHelper> &fh, const Ref<JigsawFunction> &function) {
	DECODE_VARIANT(function, editor_name);
	DECODE_VARIANT(function, editor_description);

	PackedStringArray argument_names;
	TypedArray<JigsawParameter> arguments;
	argument_names.resize(fh->read_uvarint());
	arguments.resize(argument_names.size());
	for (int64_t i = 0; i < arguments.size(); i++) {
		argument_names[i] = fh->read_stringvar();

		Ref<JigsawParameter> param;
		ERR_FAIL_COND_V(!_decode_jigsaw_parameter(fh, param), false);
		ERR_FAIL_COND_V(param.is_null(), false);
		ERR_FAIL_COND_V(!JigsawParameter::is_concrete_type(param->get_type()), false);
		arguments[i] = param;
	}
	function->set_arguments(arguments);
	function->set_argument_names(argument_names);

	PackedStringArray result_names;
	TypedArray<JigsawParameter> results;
	result_names.resize(fh->read_uvarint());
	results.resize(result_names.size());
	for (int64_t i = 0; i < results.size(); i++) {
		result_names[i] = fh->read_stringvar();

		Ref<JigsawParameter> param;
		ERR_FAIL_COND_V(!_decode_jigsaw_parameter(fh, param), false);
		ERR_FAIL_COND_V(param.is_null(), false);
		ERR_FAIL_COND_V(!JigsawParameter::is_concrete_type(param->get_type()), false);
		results[i] = param;
	}
	function->set_results(results);
	function->set_result_names(result_names);

	return fh->is_valid();
}
bool DataContainer::_encode_jigsaw_function(const Ref<FormatHelper> &fh, const Ref<JigsawFunction> &function) const {
	ENCODE_VARIANT(function, editor_name);
	ENCODE_VARIANT(function, editor_description);

	PackedStringArray argument_names = function->get_argument_names();
	TypedArray<JigsawParameter> arguments = function->get_arguments();
	ERR_FAIL_COND_V(argument_names.size() != arguments.size(), false);
	fh->write_uvarint(arguments.size());
	for (int64_t i = 0; i < arguments.size(); i++) {
		fh->write_stringvar(argument_names[i]);

		Ref<JigsawParameter> param = arguments[i];
		ERR_FAIL_COND_V(param.is_null(), false);
		ERR_FAIL_COND_V(!JigsawParameter::is_concrete_type(param->get_type()), false);
		ERR_FAIL_COND_V(!_encode_jigsaw_parameter(fh, param), false);
		arguments[i] = param;
	}

	PackedStringArray result_names = function->get_result_names();
	TypedArray<JigsawParameter> results = function->get_results();
	ERR_FAIL_COND_V(result_names.size() != results.size(), false);
	fh->write_uvarint(results.size());
	for (int64_t i = 0; i < results.size(); i++) {
		fh->write_stringvar(result_names[i]);

		Ref<JigsawParameter> param = results[i];
		ERR_FAIL_COND_V(param.is_null(), false);
		ERR_FAIL_COND_V(!JigsawParameter::is_concrete_type(param->get_type()), false);
		ERR_FAIL_COND_V(!_encode_jigsaw_parameter(fh, param), false);
	}

	return fh->is_valid();
}

bool DataContainer::_decode_jigsaw_trigger_effect(const Ref<FormatHelper> &fh, Ref<JigsawTriggerEffect> &trigger) {
	trigger = Ref<JigsawTriggerEffect>();

	JigsawTriggerEffect::Type type = static_cast<JigsawTriggerEffect::Type>(fh->read_uvarint());
	switch (type) {
	case JigsawTriggerEffect::CUSTOM:
	{
		Ref<JigsawTriggerEffectCustom> custom;
		custom.instantiate();
		custom->set_custom_tag(JigsawParameterChoice::make(fh->read_id<enums::ChoicesDef::Choices>(), int64_t(fh->read_uvarint()) - 1));
		trigger = custom;
		break;
	}
	}
	ERR_FAIL_COND_V_MSG(trigger.is_null(), false, vformat("unhandled JigsawTriggerEffect type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawTriggerEffect", "Type", type)));
	ERR_FAIL_COND_V(!_decode_jigsaw_procedure(fh, trigger), false);

	return fh->is_valid();
}
bool DataContainer::_encode_jigsaw_trigger_effect(const Ref<FormatHelper> &fh, const Ref<JigsawTriggerEffect> &trigger) const {
	fh->write_uvarint(trigger->get_type());

	if (JigsawTriggerEffectCustom *custom = Object::cast_to<JigsawTriggerEffectCustom>(trigger.ptr())) {
		Ref<JigsawParameterChoice> tag = custom->get_custom_tag();
		if (tag.is_null()) {
			fh->write_id(enums::ChoicesDef::NONE);
			fh->write_uvarint(0);
		} else {
			fh->write_id(tag->get_choices());
			fh->write_uvarint(tag->get_index() + 1);
		}
	}

	ERR_FAIL_COND_V(!_encode_jigsaw_procedure(fh, trigger), false);

	return fh->is_valid();
}
bool DataContainer::_decode_jigsaw_trigger_modifier(const Ref<FormatHelper> &fh, Ref<JigsawTriggerModifier> &trigger) {
	trigger = Ref<JigsawTriggerModifier>();

	JigsawTriggerModifier::Type type = static_cast<JigsawTriggerModifier::Type>(fh->read_uvarint());
	switch (type) {
	case JigsawTriggerModifier::CUSTOM:
	{
		Ref<JigsawTriggerModifierCustom> custom;
		custom.instantiate();
		custom->set_custom_tag(JigsawParameterChoice::make(fh->read_id<enums::ChoicesDef::Choices>(), int64_t(fh->read_uvarint()) - 1));
		trigger = custom;
		break;
	}
	}
	ERR_FAIL_COND_V_MSG(trigger.is_null(), false, vformat("unhandled JigsawTriggerModifier type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawTriggerModifier", "Type", type)));
	ERR_FAIL_COND_V(!_decode_jigsaw_procedure(fh, trigger), false);

	return fh->is_valid();
}
bool DataContainer::_encode_jigsaw_trigger_modifier(const Ref<FormatHelper> &fh, const Ref<JigsawTriggerModifier> &trigger) const {
	fh->write_uvarint(trigger->get_type());

	if (JigsawTriggerModifierCustom *custom = Object::cast_to<JigsawTriggerModifierCustom>(trigger.ptr())) {
		Ref<JigsawParameterChoice> tag = custom->get_custom_tag();
		if (tag.is_null()) {
			fh->write_id(enums::ChoicesDef::NONE);
			fh->write_uvarint(0);
		} else {
			fh->write_id(tag->get_choices());
			fh->write_uvarint(tag->get_index() + 1);
		}
	}

	ERR_FAIL_COND_V(!_encode_jigsaw_procedure(fh, trigger), false);

	return fh->is_valid();
}
bool DataContainer::_decode_jigsaw_trigger_npc(const Ref<FormatHelper> &fh, Ref<JigsawTriggerNPC> &trigger) {
	trigger = Ref<JigsawTriggerNPC>();

	JigsawTriggerNPC::Type type = static_cast<JigsawTriggerNPC::Type>(fh->read_uvarint());
	switch (type) {
	case JigsawTriggerNPC::CUSTOM:
	{
		Ref<JigsawTriggerNPCCustom> custom;
		custom.instantiate();
		custom->set_custom_tag(JigsawParameterChoice::make(fh->read_id<enums::ChoicesDef::Choices>(), int64_t(fh->read_uvarint()) - 1));
		trigger = custom;
		break;
	}
	}
	ERR_FAIL_COND_V_MSG(trigger.is_null(), false, vformat("unhandled JigsawTriggerNPC type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawTriggerNPC", "Type", type)));
	ERR_FAIL_COND_V(!_decode_jigsaw_procedure(fh, trigger), false);

	return fh->is_valid();
}
bool DataContainer::_encode_jigsaw_trigger_npc(const Ref<FormatHelper> &fh, const Ref<JigsawTriggerNPC> &trigger) const {
	fh->write_uvarint(trigger->get_type());

	if (JigsawTriggerNPCCustom *custom = Object::cast_to<JigsawTriggerNPCCustom>(trigger.ptr())) {
		Ref<JigsawParameterChoice> tag = custom->get_custom_tag();
		if (tag.is_null()) {
			fh->write_id(enums::ChoicesDef::NONE);
			fh->write_uvarint(0);
		} else {
			fh->write_id(tag->get_choices());
			fh->write_uvarint(tag->get_index() + 1);
		}
	}

	ERR_FAIL_COND_V(!_encode_jigsaw_procedure(fh, trigger), false);

	return fh->is_valid();
}
bool DataContainer::_decode_jigsaw_trigger_variant(const Ref<FormatHelper> &fh, Ref<JigsawTriggerVariant> &trigger) {
	trigger = Ref<JigsawTriggerVariant>();

	JigsawTriggerVariant::Type type = static_cast<JigsawTriggerVariant::Type>(fh->read_uvarint());
	switch (type) {
	case JigsawTriggerVariant::CUSTOM:
	{
		Ref<JigsawTriggerVariantCustom> custom;
		custom.instantiate();
		custom->set_custom_tag(JigsawParameterChoice::make(fh->read_id<enums::ChoicesDef::Choices>(), int64_t(fh->read_uvarint()) - 1));
		trigger = custom;
		break;
	}
	case JigsawTriggerVariant::COSMETIC_INIT:
		trigger.reference_ptr(memnew(JigsawTriggerVariantCosmeticInit));
		break;
	case JigsawTriggerVariant::CHARACTER_INIT:
		trigger.reference_ptr(memnew(JigsawTriggerVariantCharacterInit));
		break;
	case JigsawTriggerVariant::DECK_INIT:
		trigger.reference_ptr(memnew(JigsawTriggerVariantDeckInit));
		break;
	case JigsawTriggerVariant::MATCH_START:
		trigger.reference_ptr(memnew(JigsawTriggerVariantMatchStart));
		break;
	}
	ERR_FAIL_COND_V_MSG(trigger.is_null(), false, vformat("unhandled JigsawTriggerVariant type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawTriggerVariant", "Type", type)));
	ERR_FAIL_COND_V(!_decode_jigsaw_procedure(fh, trigger), false);

	return fh->is_valid();
}
bool DataContainer::_encode_jigsaw_trigger_variant(const Ref<FormatHelper> &fh, const Ref<JigsawTriggerVariant> &trigger) const {
	fh->write_uvarint(trigger->get_type());

	if (JigsawTriggerVariantCustom *custom = Object::cast_to<JigsawTriggerVariantCustom>(trigger.ptr())) {
		Ref<JigsawParameterChoice> tag = custom->get_custom_tag();
		if (tag.is_null()) {
			fh->write_id(enums::ChoicesDef::NONE);
			fh->write_uvarint(0);
		} else {
			fh->write_id(tag->get_choices());
			fh->write_uvarint(tag->get_index() + 1);
		}
	}

	ERR_FAIL_COND_V(!_encode_jigsaw_procedure(fh, trigger), false);

	return fh->is_valid();
}

bool DataContainer::_decode_jigsaw_procedure(const Ref<FormatHelper> &fh, const Ref<JigsawProcedure> &procedure) {
	Ref<JigsawCommandList> commands;
	ERR_FAIL_COND_V(!_decode_jigsaw_command_list(fh, commands), false);
	procedure->set_commands(commands);

	return fh->is_valid();
}
bool DataContainer::_encode_jigsaw_procedure(const Ref<FormatHelper> &fh, const Ref<JigsawProcedure> &procedure) const {
	ERR_FAIL_COND_V(!_encode_jigsaw_command_list(fh, procedure->get_commands()), false);

	return fh->is_valid();
}

bool DataContainer::_decode_jigsaw_command_list(const Ref<FormatHelper> &fh, Ref<JigsawCommandList> &commands) {
	PackedStringArray names;
	TypedArray<JigsawParameter> vars;
	TypedArray<JigsawCommand> list;

	names.resize(fh->read_uvarint());
	vars.resize(names.size());

	for (int64_t i = 0; i < names.size(); i++) {
		names[i] = fh->read_stringvar();

		Ref<JigsawParameter> param;
		ERR_FAIL_COND_V(!_decode_jigsaw_parameter(fh, param), false);
		ERR_FAIL_COND_V(param.is_null(), false);
		vars[i] = param;
	}

	list.resize(fh->read_uvarint());

	for (int64_t i = 0; i < list.size(); i++) {
		Ref<JigsawCommand> command;
		ERR_FAIL_COND_V(!_decode_jigsaw_command(fh, command), false);
		ERR_FAIL_COND_V(command.is_null(), false);

		list[i] = command;
	}

	if (names.size() == 0 && list.size() == 0) {
		if (!fh->read_bool()) {
			commands = Ref<JigsawCommandList>();

			return fh->is_valid();
		}
	}

	commands.instantiate();
	commands->set_local_variable_names(names);
	commands->set_local_variables(vars);
	commands->set_list(list);

	return fh->is_valid();
}
bool DataContainer::_encode_jigsaw_command_list(const Ref<FormatHelper> &fh, const Ref<JigsawCommandList> &commands) const {
	if (commands.is_null()) {
		fh->write_uvarint(0);
		fh->write_uvarint(0);
		fh->write_bool(false);
	} else {
		PackedStringArray names = commands->get_local_variable_names();
		TypedArray<JigsawParameter> vars = commands->get_local_variables();
		TypedArray<JigsawCommand> list = commands->get_list();

		ERR_FAIL_COND_V(names.size() != vars.size(), false);

		fh->write_uvarint(names.size());

		for (int64_t i = 0; i < names.size(); i++) {
			fh->write_stringvar(names[i]);

			Ref<JigsawParameter> param = vars[i];
			ERR_FAIL_COND_V(param.is_null(), false);

			ERR_FAIL_COND_V(!_encode_jigsaw_parameter(fh, param), false);
		}

		fh->write_uvarint(list.size());

		for (int64_t i = 0; i < list.size(); i++) {
			Ref<JigsawCommand> command = list[i];
			ERR_FAIL_COND_V(command.is_null(), false);
			ERR_FAIL_COND_V(!_encode_jigsaw_command(fh, command), false);
		}

		if (names.size() == 0 && list.size() == 0) {
			fh->write_bool(true);
		}
	}

	return fh->is_valid();
}

bool DataContainer::_decode_jigsaw_command(const Ref<FormatHelper> &fh, Ref<JigsawCommand> &command) {
	JigsawCommand::Type type = fh->read_id<JigsawCommand::Type>();
	command = Ref<JigsawCommand>();
	switch (type) {
	case JigsawCommand::NONE:
		return fh->is_valid();
	case JigsawCommand::CRASH:
		command.reference_ptr(memnew(JigsawCommandCrash));
		break;
	case JigsawCommand::COMMENT:
		command.reference_ptr(memnew(JigsawCommandComment));
		break;
	case JigsawCommand::FUNCTION:
		command.reference_ptr(memnew(JigsawCommandFunction));
		break;
	case JigsawCommand::MATH:
		command.reference_ptr(memnew(JigsawCommandMath));
		break;
	case JigsawCommand::LOG:
		command.reference_ptr(memnew(JigsawCommandLog));
		break;
	case JigsawCommand::FORMAT_TEXT:
		command.reference_ptr(memnew(JigsawCommandFormatText));
		break;
	case JigsawCommand::IF:
		command.reference_ptr(memnew(JigsawCommandIf));
		break;
	case JigsawCommand::SET_VARIABLE:
		command.reference_ptr(memnew(JigsawCommandSetVariable));
		break;
	case JigsawCommand::LOOKUP_DEFINITION_PROPERTY:
		command.reference_ptr(memnew(JigsawCommandLookupDefinitionProperty));
		break;
	case JigsawCommand::IS_SAME:
		command.reference_ptr(memnew(JigsawCommandIsSame));
		break;
	case JigsawCommand::ORDERED_LIST:
		command.reference_ptr(memnew(JigsawCommandOrderedList));
		break;
	case JigsawCommand::RNG:
		command.reference_ptr(memnew(JigsawCommandRNG));
		break;
	case JigsawCommand::AUDIENCE:
		command.reference_ptr(memnew(JigsawCommandAudience));
		break;
	case JigsawCommand::CHARACTER:
		command.reference_ptr(memnew(JigsawCommandCharacter));
		break;
	case JigsawCommand::AUDIO:
		command.reference_ptr(memnew(JigsawCommandAudio));
		break;
	case JigsawCommand::SCENE:
		command.reference_ptr(memnew(JigsawCommandScene));
		break;
	case JigsawCommand::SPRITE:
		command.reference_ptr(memnew(JigsawCommandSprite));
		break;
	case JigsawCommand::CAMERA:
		command.reference_ptr(memnew(JigsawCommandCamera));
		break;
	case JigsawCommand::UI:
		command.reference_ptr(memnew(JigsawCommandUI));
		break;
	case JigsawCommand::LOOP:
		command.reference_ptr(memnew(JigsawCommandLoop));
		break;
	case JigsawCommand::COLOR:
		command.reference_ptr(memnew(JigsawCommandColor));
		break;
	case JigsawCommand::CLEANUP:
		command.reference_ptr(memnew(JigsawCommandCleanup));
		break;
	}

	ERR_FAIL_COND_V_MSG(command.is_null(), false, vformat("DataContainer cannot decode Jigsaw command type %s (not implemented)", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawCommand", "Type", type)));

	// do it in two steps so we don't have to worry about special cases for when the counts mismatch (or change during assignment)
	PackedInt64Array configs;
	configs.resize(fh->read_uvarint());
	for (int64_t i = 0; i < configs.size(); i++) {
		configs[i] = fh->read_uvarint();
	}

	for (int64_t i = 0; i < configs.size() && i < command->get_num_configs(); i++) {
		command->set_config_value(i, configs[i]);
	}

	TypedArray<JigsawParameter> arguments;
	arguments.resize(fh->read_uvarint());
	for (int64_t i = 0; i < arguments.size(); i++) {
		Ref<JigsawParameter> param;
		ERR_FAIL_COND_V(!_decode_jigsaw_parameter(fh, param), false);
		arguments[i] = param;
	}

	for (int64_t i = 0; i < arguments.size() && i < command->get_num_arguments(); i++) {
		command->set_argument(i, arguments[i]);
	}

	TypedArray<JigsawParameterLocalVariable> results;
	results.resize(fh->read_uvarint());
	for (int64_t i = 0; i < results.size(); i++) {
		Ref<JigsawParameter> param;
		ERR_FAIL_COND_V(!_decode_jigsaw_parameter(fh, param), false);
		ERR_FAIL_COND_V(param.is_valid() && param->get_type() != JigsawParameter::LOCAL_VARIABLE, false);
		results[i] = param;
	}

	for (int64_t i = 0; i < results.size() && i < command->get_num_results(); i++) {
		command->set_result(i, results[i]);
	}

	TypedArray<JigsawCommandList> branches;
	branches.resize(fh->read_uvarint());
	for (int64_t i = 0; i < branches.size(); i++) {
		Ref<JigsawCommandList> branch;
		ERR_FAIL_COND_V(!_decode_jigsaw_command_list(fh, branch), false);
		branches[i] = branch;
	}

	for (int64_t i = 0; i < branches.size() && i < command->get_num_branches(); i++) {
		command->set_branch(i, branches[i]);
	}

	return fh->is_valid();
}
bool DataContainer::_encode_jigsaw_command(const Ref<FormatHelper> &fh, const Ref<JigsawCommand> &command) const {
	if (command.is_null()) {
		fh->write_id(JigsawCommand::NONE);

		return fh->is_valid();
	}

	fh->write_id(command->get_type());

	fh->write_uvarint(command->get_num_configs());
	for (int64_t i = 0; i < command->get_num_configs(); i++) {
		// these values are usually positive, so just make negative values inefficient
		fh->write_uvarint(command->get_config_value(i));
	}

	fh->write_uvarint(command->get_num_arguments());
	for (int64_t i = 0; i < command->get_num_arguments(); i++) {
		ERR_FAIL_COND_V(!_encode_jigsaw_parameter(fh, command->get_argument(i)), false);
	}

	fh->write_uvarint(command->get_num_results());
	for (int64_t i = 0; i < command->get_num_results(); i++) {
		ERR_FAIL_COND_V(!_encode_jigsaw_parameter(fh, command->get_result(i)), false);
	}

	fh->write_uvarint(command->get_num_branches());
	for (int64_t i = 0; i < command->get_num_branches(); i++) {
		ERR_FAIL_COND_V(!_encode_jigsaw_command_list(fh, command->get_branch(i)), false);
	}

	return fh->is_valid();
}

bool DataContainer::_decode_jigsaw_parameter(const Ref<FormatHelper> &fh, Ref<JigsawParameter> &parameter) {
	JigsawParameter::Type type = static_cast<JigsawParameter::Type>(fh->read_svarint());
	switch (type) {
	case JigsawParameter::SOUND_INSTANCE:
	{
		parameter = JigsawParameterSoundInstance::make(-1);
		return fh->is_valid();
	}
	case JigsawParameter::CARD_GRID:
	{
		parameter = JigsawParameterCardGrid::make(-1, true);
		return fh->is_valid();
	}
	case JigsawParameter::UI_LABEL_INSTANCE:
	{
		parameter = JigsawParameterUILabelInstance::make(-1);
		return fh->is_valid();
	}
	case JigsawParameter::UI_ICON_INSTANCE:
	{
		parameter = JigsawParameterUIIconInstance::make(-1);
		return fh->is_valid();
	}
	case JigsawParameter::AUDIENCE_INSTANCE:
	{
		parameter = JigsawParameterAudienceInstance::make(-1, -1);
		return fh->is_valid();
	}
	case JigsawParameter::CHARACTER_INSTANCE:
	{
		parameter = JigsawParameterCharacterInstance::make(-1);
		return fh->is_valid();
	}
	case JigsawParameter::SPRITE_INSTANCE:
	{
		parameter = JigsawParameterSpriteInstance::make(-1);
		return fh->is_valid();
	}
	case JigsawParameter::SCENE_INSTANCE:
	{
		parameter = JigsawParameterSceneInstance::make(-1);
		return fh->is_valid();
	}
	case JigsawParameter::QUEUED_EFFECT:
	{
		parameter = JigsawParameterQueuedEffect::make(-1, -1);
		return fh->is_valid();
	}
	case JigsawParameter::MODIFIER_INSTANCE:
	{
		parameter = JigsawParameterModifierInstance::make(-1, -1);
		return fh->is_valid();
	}
	case JigsawParameter::CARD_INSTANCE:
	{
		parameter = JigsawParameterCardInstance::make(-1);
		return fh->is_valid();
	}
	case JigsawParameter::FORMATTED_TEXT:
	{
		parameter = JigsawParameterFormattedText::make(TypedArray<FormattedText>());
		return fh->is_valid();
	}
	case JigsawParameter::NONE:
	{
		parameter = Ref<JigsawParameter>();
		return fh->is_valid();
	}
	case JigsawParameter::LOCAL_VARIABLE:
	{
		int64_t frame = fh->read_svarint();
		int64_t slot = int64_t(fh->read_uvarint()) - 1;
		parameter = JigsawParameterLocalVariable::make(frame, slot);
		return fh->is_valid();
	}
	case JigsawParameter::VARIABLE:
	{
		enums::VariableDef::Variable variable = fh->read_id<enums::VariableDef::Variable>();
		Ref<JigsawParameter> specifier;
		ERR_FAIL_COND_V(!_decode_jigsaw_parameter(fh, specifier), false);
		parameter = JigsawParameterVariable::make(variable, specifier);
		return fh->is_valid();
	}
	case JigsawParameter::ORDERED_LIST:
	{
		if (fh->read_bool()) {
			TypedArray<JigsawParameter> templates;
			templates.resize(fh->read_uvarint());
			for (int64_t i = 0; i < templates.size(); i++) {
				Ref<JigsawParameter> param;
				ERR_FAIL_COND_V(!_decode_jigsaw_parameter(fh, param), false);
				ERR_FAIL_COND_V(param.is_null(), false);
				templates[i] = param;
			}
			parameter = JigsawParameterOrderedList::make_template(templates);
		} else {
			TypedArray<JigsawParameter> params;
			params.resize(fh->read_uvarint());
			for (int64_t i = 0; i < params.size(); i++) {
				Ref<JigsawParameter> param;
				ERR_FAIL_COND_V(!_decode_jigsaw_parameter(fh, param), false);
				ERR_FAIL_COND_V(param.is_null(), false);
				params[i] = param;
			}
			parameter = JigsawParameterOrderedList::make(params);
		}
		return fh->is_valid();
	}
	case JigsawParameter::BOOLEAN:
	{
		parameter = JigsawParameterBoolean::make(fh->read_bool());
		return fh->is_valid();
	}
	case JigsawParameter::AMOUNT:
	{
		Ref<JigsawParameterAmount> amount;
		amount.instantiate();
		ERR_FAIL_COND_V(!_decode_amount(fh, amount), false);
		parameter = amount;
		return fh->is_valid();
	}
	case JigsawParameter::FLOAT:
	{
		parameter = JigsawParameterFloat::make(fh->read_float64());
		return fh->is_valid();
	}
	case JigsawParameter::STRING:
	{
		parameter = JigsawParameterString::make(fh->read_stringvar());
		return fh->is_valid();
	}
	case JigsawParameter::EFFECT_INSTANCE:
	{
		Ref<EffectInstance> inst;
		if (fh->read_bool()) {
			inst.instantiate();
			ERR_FAIL_COND_V(!_decode_effect_instance(fh, inst), false);
		}
		parameter = JigsawParameterEffectInstance::make(inst);
		return fh->is_valid();
	}
	case JigsawParameter::CARD_FILTER:
	{
		Ref<CardFilter> filter;
		ERR_FAIL_COND_V(!_decode_card_filter(fh, filter), false);
		parameter = JigsawParameterCardFilter::make(filter);
		return fh->is_valid();
	}
	case JigsawParameter::CARD:
	{
		enums::CardDef::Card card = fh->read_id<enums::CardDef::Card>();
		parameter = JigsawParameterCard::make(card);
		return fh->is_valid();
	}
	case JigsawParameter::STAT:
	{
		enums::StatDef::Stat stat = fh->read_id<enums::StatDef::Stat>();
		parameter = JigsawParameterStat::make(stat);
		return fh->is_valid();
	}
	case JigsawParameter::RANK:
	{
		enums::RankDef::Rank rank = fh->read_id<enums::RankDef::Rank>();
		parameter = JigsawParameterRank::make(rank);
		return fh->is_valid();
	}
	case JigsawParameter::TRIBE:
	{
		enums::TribeDef::Tribe tribe = fh->read_id<enums::TribeDef::Tribe>();
		parameter = JigsawParameterTribe::make(tribe);
		return fh->is_valid();
	}
	case JigsawParameter::MODIFIER:
	{
		enums::ModifierDef::Modifier modifier = fh->read_id<enums::ModifierDef::Modifier>();
		parameter = JigsawParameterModifier::make(modifier);
		return fh->is_valid();
	}
	case JigsawParameter::LOCATION:
	{
		enums::LocationDef::Location location = fh->read_id<enums::LocationDef::Location>();
		parameter = JigsawParameterLocation::make(location);
		return fh->is_valid();
	}
	case JigsawParameter::ICON:
	{
		enums::IconDef::Icon icon = fh->read_id<enums::IconDef::Icon>();
		parameter = JigsawParameterIcon::make(icon);
		return fh->is_valid();
	}
	case JigsawParameter::CID_OPUS:
	{
		PackedByteArray cid = fh->read_bytesvar();
		if (!cid.is_empty()) {
			// very basic validity checking
			ERR_FAIL_COND_V(cid.size() != 36, false);
			ERR_FAIL_COND_V(cid[0] != 0x01, false);
			ERR_FAIL_COND_V(cid[1] != 0x70, false);
			ERR_FAIL_COND_V(cid[2] != 0x12, false);
		}

		float loop_start = fh->read_float32();
		float loop_end = fh->read_float32();

		parameter = JigsawParameterCIDOpus::make(cid, loop_start, loop_end);

		return fh->is_valid();
	}
	case JigsawParameter::FILE_ID_OPUS:
	{
		PackedByteArray file_id = fh->read_bytesvar();
		ERR_FAIL_COND_V(!file_id.is_empty() && !_validate_file_id(file_id, 1, 5), false);

		float loop_start = fh->read_float32();
		float loop_end = fh->read_float32();

		parameter = JigsawParameterFileIDOpus::make(file_id, loop_start, loop_end);

		return fh->is_valid();
	}
	case JigsawParameter::FILE_ID_GLTF:
	{
		PackedByteArray file_id = fh->read_bytesvar();
		ERR_FAIL_COND_V(!file_id.is_empty() && !_validate_file_id(file_id, 2, 6), false);

		Ref<JSON> json;
		json.instantiate();
		Error err = json->parse(fh->read_stringvar());
		ERR_FAIL_COND_V_MSG(err != OK, false, vformat("failed to parse JSON data in JigsawParameterFileIDGLTF: %s", json->get_error_message()));
		ERR_FAIL_COND_V(json->get_data().get_type() != Variant::DICTIONARY, "failed to parse JSON data in JigsawParameterFileIDGLTF: expected dictionary"); 

		parameter = JigsawParameterFileIDGLTF::make(file_id, json->get_data());

		return fh->is_valid();
	}
	case JigsawParameter::COLOR:
	{
		Color color;
		ERR_FAIL_COND_V(!_decode_godot_variant(fh, color), false);
		parameter = JigsawParameterColor::make(color);
		return fh->is_valid();
	}
	case JigsawParameter::CHOICE:
	{
		enums::ChoicesDef::Choices choices = fh->read_id<enums::ChoicesDef::Choices>();
		int64_t index = int64_t(fh->read_uvarint()) - 1;
		parameter = JigsawParameterChoice::make(choices, index);
		return fh->is_valid();
	}
	case JigsawParameter::CHARACTER:
	{
		enums::CharacterDef::Character character = fh->read_id<enums::CharacterDef::Character>();
		parameter = JigsawParameterCharacter::make(character);
		return fh->is_valid();
	}
	case JigsawParameter::AUDIENCE:
	{
		enums::AudienceDef::Audience audience = fh->read_id<enums::AudienceDef::Audience>();
		parameter = JigsawParameterAudience::make(audience);
		return fh->is_valid();
	}
	case JigsawParameter::EFFECT_INSTANCE_PARAMETER:
	{
		if (fh->read_bool()) {
			enums::EffectDef::Effect effect = fh->read_id<enums::EffectDef::Effect>();
			int64_t index = int64_t(fh->read_uvarint()) - 1;
			parameter = JigsawParameterEffectInstanceParameter::make_template(effect, index);			
		} else {
			Ref<JigsawParameter> inst;
			ERR_FAIL_COND_V(!_decode_jigsaw_parameter(fh, inst), false);
			ERR_FAIL_COND_V(inst.is_null(), false);
			int64_t index = int64_t(fh->read_uvarint()) - 1;
			parameter = JigsawParameterEffectInstanceParameter::make(inst, index);
		}
		return fh->is_valid();
	}
	case JigsawParameter::STAT_VALUE:
	{
		enums::StatDef::Stat stat = fh->read_id<enums::StatDef::Stat>();
		Ref<JigsawParameterStatValue> param = JigsawParameterStatValue::make(stat, 0, 0);
		ERR_FAIL_COND_V(!_decode_amount(fh, param), false);
		parameter = param;
		return fh->is_valid();
	}
	case JigsawParameter::EXPRESSION:
	{
		Ref<JigsawParameterExpression> param;
		param.instantiate();
		DECODE_VARIANT(param, expression);
		DECODE_VARIANT(param, input_names);
		(void)param->get_parsed_expression();
		return fh->is_valid();
	}
	}

	ERR_FAIL_V_MSG(false, vformat("DataContainer cannot decode Jigsaw parameter type %s (not implemented)", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawParameter", "Type", type)));
}
bool DataContainer::_encode_jigsaw_parameter(const Ref<FormatHelper> &fh, const Ref<JigsawParameter> &parameter) const {
	if (parameter.is_null()) {
		fh->write_svarint(JigsawParameter::NONE);
		return fh->is_valid();
	}

	fh->write_svarint(parameter->get_type());
	switch (parameter->get_type()) {
	case JigsawParameter::SOUND_INSTANCE:
	case JigsawParameter::CARD_GRID:
	case JigsawParameter::UI_LABEL_INSTANCE:
	case JigsawParameter::UI_ICON_INSTANCE:
	case JigsawParameter::AUDIENCE_INSTANCE:
	case JigsawParameter::CHARACTER_INSTANCE:
	case JigsawParameter::SPRITE_INSTANCE:
	case JigsawParameter::SCENE_INSTANCE:
	case JigsawParameter::QUEUED_EFFECT:
	case JigsawParameter::MODIFIER_INSTANCE:
	case JigsawParameter::CARD_INSTANCE:
	case JigsawParameter::FORMATTED_TEXT:
	{
		// TODO: should these be serializable?
		return fh->is_valid();
	}
	case JigsawParameter::NONE:
	{
		ERR_FAIL_V_MSG(false, "DataContainer cannot encode Jigsaw parameter type NONE (invalid)");
	}
	case JigsawParameter::LOCAL_VARIABLE:
	{
		Ref<JigsawParameterLocalVariable> param = parameter;
		fh->write_svarint(param->get_frame());
		fh->write_uvarint(param->get_slot() + 1);
		return fh->is_valid();
	}
	case JigsawParameter::VARIABLE:
	{
		Ref<JigsawParameterVariable> param = parameter;
		ENCODE_VARIANT(param, variable);
		ERR_FAIL_COND_V(!_encode_jigsaw_parameter(fh, param->get_specifier()), false);
		return fh->is_valid();
	}
	case JigsawParameter::ORDERED_LIST:
	{
		Ref<JigsawParameterOrderedList> param = parameter;
		fh->write_bool(param->is_template());
		TypedArray<JigsawParameter> list = param->get_list();
		fh->write_uvarint(list.size());
		for (int64_t i = 0; i < list.size(); i++) {
			Ref<JigsawParameter> item = list[i];
			ERR_FAIL_COND_V(item.is_null(), false);
			ERR_FAIL_COND_V(!_encode_jigsaw_parameter(fh, item), false);
		}
		return fh->is_valid();
	}
	case JigsawParameter::BOOLEAN:
	{
		Ref<JigsawParameterBoolean> param = parameter;
		ENCODE_VARIANT(param, boolean);
		return fh->is_valid();
	}
	case JigsawParameter::AMOUNT:
	{
		Ref<JigsawParameterAmount> param = parameter;
		ERR_FAIL_COND_V(!_encode_amount(fh, param), false);
		return fh->is_valid();
	}
	case JigsawParameter::FLOAT:
	{
		Ref<JigsawParameterFloat> param = parameter;
		ENCODE_VARIANT(param, value);
		return fh->is_valid();
	}
	case JigsawParameter::STRING:
	{
		Ref<JigsawParameterString> param = parameter;
		ENCODE_VARIANT(param, string);
		return fh->is_valid();
	}
	case JigsawParameter::EFFECT_INSTANCE:
	{
		Ref<JigsawParameterEffectInstance> param = parameter;
		Ref<EffectInstance> inst = param->get_instance();
		fh->write_bool(inst.is_valid());
		if (inst.is_valid()) {
			ERR_FAIL_COND_V(!_encode_effect_instance(fh, inst), false);
		}
		return fh->is_valid();
	}
	case JigsawParameter::CARD_FILTER:
	{
		Ref<JigsawParameterCardFilter> param = parameter;
		ERR_FAIL_COND_V(!_encode_card_filter(fh, param->get_filter()), false);
		return fh->is_valid();
	}
	case JigsawParameter::CARD:
	{
		Ref<JigsawParameterCard> param = parameter;
		ENCODE_VARIANT(param, card);
		return fh->is_valid();
	}
	case JigsawParameter::STAT:
	{
		Ref<JigsawParameterStat> param = parameter;
		ENCODE_VARIANT(param, stat);
		return fh->is_valid();
	}
	case JigsawParameter::RANK:
	{
		Ref<JigsawParameterRank> param = parameter;
		ENCODE_VARIANT(param, rank);
		return fh->is_valid();
	}
	case JigsawParameter::TRIBE:
	{
		Ref<JigsawParameterTribe> param = parameter;
		ENCODE_VARIANT(param, tribe);
		return fh->is_valid();
	}
	case JigsawParameter::MODIFIER:
	{
		Ref<JigsawParameterModifier> param = parameter;
		ENCODE_VARIANT(param, modifier);
		return fh->is_valid();
	}
	case JigsawParameter::LOCATION:
	{
		Ref<JigsawParameterLocation> param = parameter;
		ENCODE_VARIANT(param, location);
		return fh->is_valid();
	}
	case JigsawParameter::ICON:
	{
		Ref<JigsawParameterIcon> param = parameter;
		ENCODE_VARIANT(param, icon);
		return fh->is_valid();
	}
	case JigsawParameter::CID_OPUS:
	{
		Ref<JigsawParameterCIDOpus> param = parameter;
		PackedByteArray cid = param->get_cid();
		if (!cid.is_empty()) {
			// very basic validity checking
			ERR_FAIL_COND_V(cid.size() != 36, false);
			ERR_FAIL_COND_V(cid[0] != 0x01, false);
			ERR_FAIL_COND_V(cid[1] != 0x70, false);
			ERR_FAIL_COND_V(cid[2] != 0x12, false);
		}

		ENCODE_VARIANT(param, cid);
		ENCODE_VARIANT(param, loop_start);
		ENCODE_VARIANT(param, loop_end);

		return fh->is_valid();
	}
	case JigsawParameter::FILE_ID_OPUS:
	{
		Ref<JigsawParameterFileIDOpus> param = parameter;
		ERR_FAIL_COND_V(!param->get_file_id().is_empty() && !_validate_file_id(param->get_file_id(), 1, 5), false);

		ENCODE_VARIANT(param, file_id);
		ENCODE_VARIANT(param, loop_start);
		ENCODE_VARIANT(param, loop_end);

		return fh->is_valid();
	}
	case JigsawParameter::FILE_ID_GLTF:
	{
		Ref<JigsawParameterFileIDGLTF> param = parameter;
		ERR_FAIL_COND_V(!param->get_file_id().is_empty() && !_validate_file_id(param->get_file_id(), 2, 6), false);
		ENCODE_VARIANT(param, file_id);
		fh->write_stringvar(JSON::stringify(param->get_json_data(), "", false));

		return fh->is_valid();
	}
	case JigsawParameter::COLOR:
	{
		Ref<JigsawParameterColor> param = parameter;
		ENCODE_VARIANT(param, color);
		return fh->is_valid();
	}
	case JigsawParameter::CHOICE:
	{
		Ref<JigsawParameterChoice> param = parameter;
		fh->write_id(param->get_choices());
		fh->write_uvarint(param->get_index() + 1);
		return fh->is_valid();
	}
	case JigsawParameter::CHARACTER:
	{
		Ref<JigsawParameterCharacter> param = parameter;
		fh->write_id(param->get_character());
		return fh->is_valid();
	}
	case JigsawParameter::AUDIENCE:
	{
		Ref<JigsawParameterAudience> param = parameter;
		fh->write_id(param->get_audience());
		return fh->is_valid();
	}
	case JigsawParameter::EFFECT_INSTANCE_PARAMETER:
	{
		Ref<JigsawParameterEffectInstanceParameter> param = parameter;
		Ref<JigsawParameter> inst = param->get_effect_instance();
		fh->write_bool(inst.is_null());
		if (inst.is_null()) {
			fh->write_id(param->get_template_effect());
		} else {
			ERR_FAIL_COND_V(!_encode_jigsaw_parameter(fh, inst), false);
		}
		fh->write_uvarint(param->get_index() + 1);
		return fh->is_valid();
	}
	case JigsawParameter::STAT_VALUE:
	{
		Ref<JigsawParameterStatValue> param = parameter;
		ENCODE_VARIANT(param, stat);
		ERR_FAIL_COND_V(!_encode_amount(fh, param), false);
		return fh->is_valid();
	}
	case JigsawParameter::EXPRESSION:
	{
		Ref<JigsawParameterExpression> param = parameter;
		ENCODE_VARIANT(param, expression);
		ENCODE_VARIANT(param, input_names);
		return fh->is_valid();
	}
	}

	ERR_FAIL_V_MSG(false, vformat("DataContainer cannot encode Jigsaw parameter type %s (not implemented)", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawParameter", "Type", parameter->get_type())));
}
