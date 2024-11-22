#include "jigsaw_command_lookup_definition_property.h"

#include "jigsaw_global.h"
#include "jigsaw_parameter_amount.h"
#include "jigsaw_parameter_boolean.h"
#include "jigsaw_parameter_card.h"
#include "jigsaw_parameter_card_instance.h"
#include "jigsaw_parameter_color.h"
#include "jigsaw_parameter_effect_instance.h"
#include "jigsaw_parameter_formatted_text.h"
#include "jigsaw_parameter_icon.h"
#include "jigsaw_parameter_ordered_list.h"
#include "jigsaw_parameter_rank.h"
#include "jigsaw_parameter_stat.h"
#include "jigsaw_parameter_stat_value.h"
#include "jigsaw_parameter_string.h"
#include "jigsaw_parameter_tribe.h"
#include "player_preferences_helper.h"
#include "why_isnt_this_in_godot.h"

void JigsawCommandLookupDefinitionProperty::_bind_methods() {
	BIND_ENUM_CONSTANT(CARD_DEF);
	BIND_ENUM_CONSTANT(CARD_NAME);
	BIND_ENUM_CONSTANT(CARD_RANK);
	BIND_ENUM_CONSTANT(CARD_COSTS);
	BIND_ENUM_CONSTANT(CARD_PORTRAIT);
	BIND_ENUM_CONSTANT(CARD_TRIBES);
	BIND_ENUM_CONSTANT(CARD_EFFECTS);

	BIND_ENUM_CONSTANT(RANK_NAME);
	BIND_ENUM_CONSTANT(RANK_COLOR);
	BIND_ENUM_CONSTANT(RANK_COLOR_ALT);
	BIND_ENUM_CONSTANT(RANK_COLOR_BASED_ON_PREFERENCES);
	BIND_ENUM_CONSTANT(RANK_FRONT);
	BIND_ENUM_CONSTANT(RANK_BACK);

	BIND_ENUM_CONSTANT(TRIBE_NAME);
	BIND_ENUM_CONSTANT(TRIBE_COLOR);
	BIND_ENUM_CONSTANT(TRIBE_IS_HIDDEN);
	BIND_ENUM_CONSTANT(TRIBE_IS_WIDE);

	BIND_ENUM_CONSTANT(STAT_DEF);
	BIND_ENUM_CONSTANT(STAT_NAME);
	BIND_ENUM_CONSTANT(STAT_ICON);
	BIND_ENUM_CONSTANT(STAT_AMOUNT);

	BIND_PROPERTY_ENUM(JigsawCommandLookupDefinitionProperty::Property, property);
	BIND_PROPERTY_RESOURCE(JigsawParameter, definition);
	BIND_PROPERTY_RESOURCE(JigsawParameterLocalVariable, value);
}

IMPLEMENT_PROPERTY(JigsawCommandLookupDefinitionProperty, JigsawCommandLookupDefinitionProperty::Property, property);
IMPLEMENT_PROPERTY(JigsawCommandLookupDefinitionProperty, Ref<JigsawParameter>, definition);
IMPLEMENT_PROPERTY(JigsawCommandLookupDefinitionProperty, Ref<JigsawParameterLocalVariable>, value);

JigsawExecutionState JigsawCommandLookupDefinitionProperty::evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const {
	Ref<JigsawParameter> def;
	err = context->resolve_variable(_definition, def, get_argument_name(0));
	if (unlikely(err.is_valid())) {
		return JigsawExecutionState::ERROR;
	}

	if (unlikely(def.is_null())) {
		err = context->create_error("cannot read property of null definition");
		return JigsawExecutionState::ERROR;
	}

	switch (_property) {
	case CARD_DEF:
		if (likely(def->get_type() == JigsawParameter::CARD_INSTANCE)) {
			Ref<JigsawParameterCardInstance> inst_param = def;
			Ref<CardInstance> inst = inst_param->get_instance();

			if (unlikely(inst.is_null())) {
				err = context->create_error("cannot read card ID of null card instance");
				return JigsawExecutionState::ERROR;
			}

			Ref<CardDef> card_def = inst->get_def();

			return set_command_result(context, err, 0, JigsawParameterCard::make(likely(card_def.is_valid()) ? card_def->get_id() : enums::CardDef::NONE));
		}

		err = context->create_error(vformat("cannot read card ID of parameter type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawParameter", "Type", def->get_type())));
		return JigsawExecutionState::ERROR;
	case CARD_NAME:
		if (def->get_type() == JigsawParameter::CARD) {
			Ref<JigsawParameterCard> card_param = def;
			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<CardDef> card_def = likely(mode.is_valid()) ? mode->get_card(card_param->get_card()) : Ref<CardDef>();

			if (unlikely(card_def.is_null())) {
				err = context->create_error(vformat("cannot read card name of missing card %d", card_param->get_card()));
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterFormattedText::make(FormattedText::make_plain(card_def->get_name())));
		}

		if (likely(def->get_type() == JigsawParameter::CARD_INSTANCE)) {
			Ref<JigsawParameterCardInstance> inst_param = def;
			Ref<CardInstance> inst = inst_param->get_instance();

			if (unlikely(inst.is_null())) {
				err = context->create_error("cannot read card name of null card instance");
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterFormattedText::make(inst->get_name()));
		}

		err = context->create_error(vformat("cannot read card name of parameter type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawParameter", "Type", def->get_type())));
		return JigsawExecutionState::ERROR;
	case CARD_RANK:
		if (def->get_type() == JigsawParameter::CARD) {
			Ref<JigsawParameterCard> card_param = def;
			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<CardDef> card_def = likely(mode.is_valid()) ? mode->get_card(card_param->get_card()) : Ref<CardDef>();

			if (unlikely(card_def.is_null())) {
				err = context->create_error(vformat("cannot read card rank of missing card %d", card_param->get_card()));
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterRank::make(card_def->get_rank()));
		}

		if (likely(def->get_type() == JigsawParameter::CARD_INSTANCE)) {
			Ref<JigsawParameterCardInstance> inst_param = def;
			Ref<CardInstance> inst = inst_param->get_instance();

			if (unlikely(inst.is_null())) {
				err = context->create_error("cannot read card rank of null card instance");
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterRank::make(inst->get_rank()));
		}

		err = context->create_error(vformat("cannot read card rank of parameter type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawParameter", "Type", def->get_type())));
		return JigsawExecutionState::ERROR;
	case CARD_COSTS:
		if (def->get_type() == JigsawParameter::CARD) {
			Ref<JigsawParameterCard> card_param = def;
			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<CardDef> card_def = likely(mode.is_valid()) ? mode->get_card(card_param->get_card()) : Ref<CardDef>();

			if (unlikely(card_def.is_null())) {
				err = context->create_error(vformat("cannot read card costs of missing card %d", card_param->get_card()));
				return JigsawExecutionState::ERROR;
			}

			TypedArray<JigsawParameter> costs = card_def->get_costs().map(callable_mp_static(&JigsawParameterStatValue::convert));

			return set_command_result(context, err, 0, JigsawParameterOrderedList::make(costs));
		}

		if (likely(def->get_type() == JigsawParameter::CARD_INSTANCE)) {
			Ref<JigsawParameterCardInstance> inst_param = def;
			Ref<CardInstance> inst = inst_param->get_instance();

			if (unlikely(inst.is_null())) {
				err = context->create_error("cannot read card costs of null card instance");
				return JigsawExecutionState::ERROR;
			}

			TypedArray<JigsawParameter> costs = inst->get_costs().map(callable_mp_static(&JigsawParameterStatValue::convert));

			return set_command_result(context, err, 0, JigsawParameterOrderedList::make(costs));
		}

		err = context->create_error(vformat("cannot read card costs of parameter type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawParameter", "Type", def->get_type())));
		return JigsawExecutionState::ERROR;
	case CARD_PORTRAIT:
		if (def->get_type() == JigsawParameter::CARD) {
			Ref<JigsawParameterCard> card_param = def;
			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<CardDef> card_def = likely(mode.is_valid()) ? mode->get_card(card_param->get_card()) : Ref<CardDef>();

			if (unlikely(card_def.is_null())) {
				err = context->create_error(vformat("cannot read card portrait of missing card %d", card_param->get_card()));
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterIcon::make(card_def->get_portrait()));
		}

		if (likely(def->get_type() == JigsawParameter::CARD_INSTANCE)) {
			Ref<JigsawParameterCardInstance> inst_param = def;
			Ref<CardInstance> inst = inst_param->get_instance();

			if (unlikely(inst.is_null())) {
				err = context->create_error("cannot read card portrait of null card instance");
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterIcon::make(inst->get_portrait()));
		}

		err = context->create_error(vformat("cannot read card portrait of parameter type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawParameter", "Type", def->get_type())));
		return JigsawExecutionState::ERROR;
	case CARD_TRIBES:
		if (def->get_type() == JigsawParameter::CARD) {
			Ref<JigsawParameterCard> card_param = def;
			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<CardDef> card_def = likely(mode.is_valid()) ? mode->get_card(card_param->get_card()) : Ref<CardDef>();

			if (unlikely(card_def.is_null())) {
				err = context->create_error(vformat("cannot read card tribes of missing card %d", card_param->get_card()));
				return JigsawExecutionState::ERROR;
			}

			TypedArray<JigsawParameter> tribes = card_def->get_tribes().map(callable_mp_static(&JigsawParameterTribe::make));

			return set_command_result(context, err, 0, JigsawParameterOrderedList::make(tribes));
		}

		if (likely(def->get_type() == JigsawParameter::CARD_INSTANCE)) {
			Ref<JigsawParameterCardInstance> inst_param = def;
			Ref<CardInstance> inst = inst_param->get_instance();

			if (unlikely(inst.is_null())) {
				err = context->create_error("cannot read card tribes of null card instance");
				return JigsawExecutionState::ERROR;
			}

			TypedArray<JigsawParameter> tribes = inst->get_tribes().map(callable_mp_static(&JigsawParameterTribe::make));

			return set_command_result(context, err, 0, JigsawParameterOrderedList::make(tribes));
		}

		err = context->create_error(vformat("cannot read card tribes of parameter type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawParameter", "Type", def->get_type())));
		return JigsawExecutionState::ERROR;
	case CARD_EFFECTS:
		if (def->get_type() == JigsawParameter::CARD) {
			Ref<JigsawParameterCard> card_param = def;
			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<CardDef> card_def = likely(mode.is_valid()) ? mode->get_card(card_param->get_card()) : Ref<CardDef>();

			if (unlikely(card_def.is_null())) {
				err = context->create_error(vformat("cannot read card effects of missing card %d", card_param->get_card()));
				return JigsawExecutionState::ERROR;
			}

			// for some reason, if we do this the same way as CARD_INSTANCE, we encounter some kind of emscripten bug
			// that makes JigsawParameterEffectInstance::make be both a void(i32, i32) and a void().
			TypedArray<EffectInstance> effects = card_def->get_effects();
			TypedArray<JigsawParameter> wrapped_effects;
			wrapped_effects.resize(effects.size());
			for (int64_t i = 0; i < effects.size(); i++) {
				wrapped_effects[i] = JigsawParameterEffectInstance::make(effects[i]);
			}

			return set_command_result(context, err, 0, JigsawParameterOrderedList::make(wrapped_effects));
		}

		if (likely(def->get_type() == JigsawParameter::CARD_INSTANCE)) {
			Ref<JigsawParameterCardInstance> inst_param = def;
			Ref<CardInstance> inst = inst_param->get_instance();

			if (unlikely(inst.is_null())) {
				err = context->create_error("cannot read card effects of null card instance");
				return JigsawExecutionState::ERROR;
			}

			TypedArray<JigsawParameter> effects = inst->get_effects().map(callable_mp_static(&JigsawParameterEffectInstance::make));

			return set_command_result(context, err, 0, JigsawParameterOrderedList::make(effects));
		}

		err = context->create_error(vformat("cannot read card effects of parameter type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawParameter", "Type", def->get_type())));
		return JigsawExecutionState::ERROR;
	case RANK_NAME:
		if (def->get_type() == JigsawParameter::RANK) {
			Ref<JigsawParameterRank> rank_param = def;
			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<RankDef> rank_def = likely(mode.is_valid()) ? mode->get_rank(rank_param->get_rank()) : Ref<RankDef>();

			if (unlikely(rank_def.is_null())) {
				err = context->create_error(vformat("cannot read rank name of missing rank %d", rank_param->get_rank()));
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterString::make(rank_def->get_name()));
		}

		if (def->get_type() == JigsawParameter::CARD) {
			Ref<JigsawParameterCard> card_param = def;
			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<CardDef> card_def = likely(mode.is_valid()) ? mode->get_card(card_param->get_card()) : Ref<CardDef>();

			if (unlikely(card_def.is_null())) {
				err = context->create_error(vformat("cannot read rank name of missing card %d", card_param->get_card()));
				return JigsawExecutionState::ERROR;
			}

			Ref<RankDef> rank_def = mode->get_rank(card_def->get_rank());

			if (unlikely(rank_def.is_null())) {
				err = context->create_error(vformat("cannot read rank name of missing rank %d", card_def->get_rank()));
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterString::make(rank_def->get_name()));
		}

		if (likely(def->get_type() == JigsawParameter::CARD_INSTANCE)) {
			Ref<JigsawParameterCardInstance> inst_param = def;
			Ref<CardInstance> inst = inst_param->get_instance();

			if (unlikely(inst.is_null())) {
				err = context->create_error("cannot read rank name of null card instance");
				return JigsawExecutionState::ERROR;
			}

			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<RankDef> rank_def = likely(mode.is_valid()) ? mode->get_rank(inst->get_rank()) : Ref<RankDef>();

			if (unlikely(rank_def.is_null())) {
				err = context->create_error(vformat("cannot read rank name of missing rank %d", inst->get_rank()));
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterString::make(rank_def->get_name()));
		}

		err = context->create_error(vformat("cannot read rank name of parameter type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawParameter", "Type", def->get_type())));
		return JigsawExecutionState::ERROR;
	case RANK_COLOR:
		if (def->get_type() == JigsawParameter::RANK) {
			Ref<JigsawParameterRank> rank_param = def;
			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<RankDef> rank_def = likely(mode.is_valid()) ? mode->get_rank(rank_param->get_rank()) : Ref<RankDef>();

			if (unlikely(rank_def.is_null())) {
				err = context->create_error(vformat("cannot read rank color of missing rank %d", rank_param->get_rank()));
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterColor::make(rank_def->get_color()));
		}

		if (def->get_type() == JigsawParameter::CARD) {
			Ref<JigsawParameterCard> card_param = def;
			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<CardDef> card_def = likely(mode.is_valid()) ? mode->get_card(card_param->get_card()) : Ref<CardDef>();

			if (unlikely(card_def.is_null())) {
				err = context->create_error(vformat("cannot read rank color of missing card %d", card_param->get_card()));
				return JigsawExecutionState::ERROR;
			}

			Ref<RankDef> rank_def = mode->get_rank(card_def->get_rank());

			if (unlikely(rank_def.is_null())) {
				err = context->create_error(vformat("cannot read rank color of missing rank %d", card_def->get_rank()));
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterColor::make(rank_def->get_color()));
		}

		if (likely(def->get_type() == JigsawParameter::CARD_INSTANCE)) {
			Ref<JigsawParameterCardInstance> inst_param = def;
			Ref<CardInstance> inst = inst_param->get_instance();

			if (unlikely(inst.is_null())) {
				err = context->create_error("cannot read rank color of null card instance");
				return JigsawExecutionState::ERROR;
			}

			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<RankDef> rank_def = likely(mode.is_valid()) ? mode->get_rank(inst->get_rank()) : Ref<RankDef>();

			if (unlikely(rank_def.is_null())) {
				err = context->create_error(vformat("cannot read rank color of missing rank %d", inst->get_rank()));
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterColor::make(rank_def->get_color()));
		}

		err = context->create_error(vformat("cannot read rank color of parameter type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawParameter", "Type", def->get_type())));
		return JigsawExecutionState::ERROR;
	case RANK_COLOR_ALT:
		if (def->get_type() == JigsawParameter::RANK) {
			Ref<JigsawParameterRank> rank_param = def;
			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<RankDef> rank_def = likely(mode.is_valid()) ? mode->get_rank(rank_param->get_rank()) : Ref<RankDef>();

			if (unlikely(rank_def.is_null())) {
				err = context->create_error(vformat("cannot read rank color of missing rank %d", rank_param->get_rank()));
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterColor::make(rank_def->get_color_alt()));
		}

		if (def->get_type() == JigsawParameter::CARD) {
			Ref<JigsawParameterCard> card_param = def;
			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<CardDef> card_def = likely(mode.is_valid()) ? mode->get_card(card_param->get_card()) : Ref<CardDef>();

			if (unlikely(card_def.is_null())) {
				err = context->create_error(vformat("cannot read rank color of missing card %d", card_param->get_card()));
				return JigsawExecutionState::ERROR;
			}

			Ref<RankDef> rank_def = mode->get_rank(card_def->get_rank());

			if (unlikely(rank_def.is_null())) {
				err = context->create_error(vformat("cannot read rank color of missing rank %d", card_def->get_rank()));
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterColor::make(rank_def->get_color_alt()));
		}

		if (likely(def->get_type() == JigsawParameter::CARD_INSTANCE)) {
			Ref<JigsawParameterCardInstance> inst_param = def;
			Ref<CardInstance> inst = inst_param->get_instance();

			if (unlikely(inst.is_null())) {
				err = context->create_error("cannot read rank color of null card instance");
				return JigsawExecutionState::ERROR;
			}

			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<RankDef> rank_def = likely(mode.is_valid()) ? mode->get_rank(inst->get_rank()) : Ref<RankDef>();

			if (unlikely(rank_def.is_null())) {
				err = context->create_error(vformat("cannot read rank color of missing rank %d", inst->get_rank()));
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterColor::make(rank_def->get_color_alt()));
		}

		err = context->create_error(vformat("cannot read rank color of parameter type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawParameter", "Type", def->get_type())));
		return JigsawExecutionState::ERROR;
	case RANK_COLOR_BASED_ON_PREFERENCES:
		if (def->get_type() == JigsawParameter::RANK) {
			Ref<JigsawParameterRank> rank_param = def;
			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<RankDef> rank_def = likely(mode.is_valid()) ? mode->get_rank(rank_param->get_rank()) : Ref<RankDef>();

			if (unlikely(rank_def.is_null())) {
				err = context->create_error(vformat("cannot read rank color of missing rank %d", rank_param->get_rank()));
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterColor::make(rank_def->get_color_by_preference()));
		}

		if (def->get_type() == JigsawParameter::CARD) {
			Ref<JigsawParameterCard> card_param = def;
			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<CardDef> card_def = likely(mode.is_valid()) ? mode->get_card(card_param->get_card()) : Ref<CardDef>();

			if (unlikely(card_def.is_null())) {
				err = context->create_error(vformat("cannot read rank color of missing card %d", card_param->get_card()));
				return JigsawExecutionState::ERROR;
			}

			Ref<RankDef> rank_def = mode->get_rank(card_def->get_rank());

			if (unlikely(rank_def.is_null())) {
				err = context->create_error(vformat("cannot read rank color of missing rank %d", card_def->get_rank()));
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterColor::make(rank_def->get_color_by_preference()));
		}

		if (likely(def->get_type() == JigsawParameter::CARD_INSTANCE)) {
			Ref<JigsawParameterCardInstance> inst_param = def;
			Ref<CardInstance> inst = inst_param->get_instance();

			if (unlikely(inst.is_null())) {
				err = context->create_error("cannot read rank color of null card instance");
				return JigsawExecutionState::ERROR;
			}

			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<RankDef> rank_def = likely(mode.is_valid()) ? mode->get_rank(inst->get_rank()) : Ref<RankDef>();

			if (unlikely(rank_def.is_null())) {
				err = context->create_error(vformat("cannot read rank color of missing rank %d", inst->get_rank()));
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterColor::make(rank_def->get_color_by_preference()));
		}

		err = context->create_error(vformat("cannot read rank color of parameter type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawParameter", "Type", def->get_type())));
		return JigsawExecutionState::ERROR;
	case RANK_FRONT:
		if (def->get_type() == JigsawParameter::RANK) {
			Ref<JigsawParameterRank> rank_param = def;
			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<RankDef> rank_def = likely(mode.is_valid()) ? mode->get_rank(rank_param->get_rank()) : Ref<RankDef>();

			if (unlikely(rank_def.is_null())) {
				err = context->create_error(vformat("cannot read rank front of missing rank %d", rank_param->get_rank()));
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterIcon::make(rank_def->get_front()));
		}

		if (def->get_type() == JigsawParameter::CARD) {
			Ref<JigsawParameterCard> card_param = def;
			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<CardDef> card_def = likely(mode.is_valid()) ? mode->get_card(card_param->get_card()) : Ref<CardDef>();

			if (unlikely(card_def.is_null())) {
				err = context->create_error(vformat("cannot read rank front of missing card %d", card_param->get_card()));
				return JigsawExecutionState::ERROR;
			}

			Ref<RankDef> rank_def = mode->get_rank(card_def->get_rank());

			if (unlikely(rank_def.is_null())) {
				err = context->create_error(vformat("cannot read rank front of missing rank %d", card_def->get_rank()));
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterIcon::make(rank_def->get_front()));
		}

		if (likely(def->get_type() == JigsawParameter::CARD_INSTANCE)) {
			Ref<JigsawParameterCardInstance> inst_param = def;
			Ref<CardInstance> inst = inst_param->get_instance();

			if (unlikely(inst.is_null())) {
				err = context->create_error("cannot read rank front of null card instance");
				return JigsawExecutionState::ERROR;
			}

			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<RankDef> rank_def = likely(mode.is_valid()) ? mode->get_rank(inst->get_rank()) : Ref<RankDef>();

			if (unlikely(rank_def.is_null())) {
				err = context->create_error(vformat("cannot read rank front of missing rank %d", inst->get_rank()));
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterIcon::make(rank_def->get_front()));
		}

		err = context->create_error(vformat("cannot read rank front of parameter type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawParameter", "Type", def->get_type())));
		return JigsawExecutionState::ERROR;
	case RANK_BACK:
		if (def->get_type() == JigsawParameter::RANK) {
			Ref<JigsawParameterRank> rank_param = def;
			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<RankDef> rank_def = likely(mode.is_valid()) ? mode->get_rank(rank_param->get_rank()) : Ref<RankDef>();

			if (unlikely(rank_def.is_null())) {
				err = context->create_error(vformat("cannot read rank back of missing rank %d", rank_param->get_rank()));
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterIcon::make(rank_def->get_back()));
		}

		if (def->get_type() == JigsawParameter::CARD) {
			Ref<JigsawParameterCard> card_param = def;
			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<CardDef> card_def = likely(mode.is_valid()) ? mode->get_card(card_param->get_card()) : Ref<CardDef>();

			if (unlikely(card_def.is_null())) {
				err = context->create_error(vformat("cannot read rank back of missing card %d", card_param->get_card()));
				return JigsawExecutionState::ERROR;
			}

			Ref<RankDef> rank_def = mode->get_rank(card_def->get_rank());

			if (unlikely(rank_def.is_null())) {
				err = context->create_error(vformat("cannot read rank back of missing rank %d", card_def->get_rank()));
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterIcon::make(rank_def->get_back()));
		}

		if (likely(def->get_type() == JigsawParameter::CARD_INSTANCE)) {
			Ref<JigsawParameterCardInstance> inst_param = def;
			Ref<CardInstance> inst = inst_param->get_instance();

			if (unlikely(inst.is_null())) {
				err = context->create_error("cannot read rank back of null card instance");
				return JigsawExecutionState::ERROR;
			}

			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<RankDef> rank_def = likely(mode.is_valid()) ? mode->get_rank(inst->get_rank()) : Ref<RankDef>();

			if (unlikely(rank_def.is_null())) {
				err = context->create_error(vformat("cannot read rank back of missing rank %d", inst->get_rank()));
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterIcon::make(rank_def->get_back()));
		}

		err = context->create_error(vformat("cannot read rank back of parameter type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawParameter", "Type", def->get_type())));
		return JigsawExecutionState::ERROR;
	case TRIBE_NAME:
		if (likely(def->get_type() == JigsawParameter::TRIBE)) {
			Ref<JigsawParameterTribe> tribe_param = def;
			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<TribeDef> tribe_def = likely(mode.is_valid()) ? mode->get_tribe(tribe_param->get_tribe()) : Ref<TribeDef>();

			if (unlikely(tribe_def.is_null())) {
				err = context->create_error(vformat("cannot read tribe name of missing tribe %d", tribe_param->get_tribe()));
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterString::make(tribe_def->get_name()));
		}

		err = context->create_error(vformat("cannot read tribe name of parameter type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawParameter", "Type", def->get_type())));
		return JigsawExecutionState::ERROR;
	case TRIBE_COLOR:
		if (likely(def->get_type() == JigsawParameter::TRIBE)) {
			Ref<JigsawParameterTribe> tribe_param = def;
			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<TribeDef> tribe_def = likely(mode.is_valid()) ? mode->get_tribe(tribe_param->get_tribe()) : Ref<TribeDef>();

			if (unlikely(tribe_def.is_null())) {
				err = context->create_error(vformat("cannot read tribe color of missing tribe %d", tribe_param->get_tribe()));
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterColor::make(tribe_def->get_color()));
		}

		err = context->create_error(vformat("cannot read tribe color of parameter type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawParameter", "Type", def->get_type())));
		return JigsawExecutionState::ERROR;
	case TRIBE_IS_HIDDEN:
		if (likely(def->get_type() == JigsawParameter::TRIBE)) {
			Ref<JigsawParameterTribe> tribe_param = def;
			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<TribeDef> tribe_def = likely(mode.is_valid()) ? mode->get_tribe(tribe_param->get_tribe()) : Ref<TribeDef>();

			if (unlikely(tribe_def.is_null())) {
				err = context->create_error(vformat("cannot read tribe display type of missing tribe %d", tribe_param->get_tribe()));
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterBoolean::make(tribe_def->get_display() == TribeDef::HIDDEN));
		}

		err = context->create_error(vformat("cannot read tribe display type of parameter type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawParameter", "Type", def->get_type())));
		return JigsawExecutionState::ERROR;
	case TRIBE_IS_WIDE:
		if (likely(def->get_type() == JigsawParameter::TRIBE)) {
			Ref<JigsawParameterTribe> tribe_param = def;
			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<TribeDef> tribe_def = likely(mode.is_valid()) ? mode->get_tribe(tribe_param->get_tribe()) : Ref<TribeDef>();

			if (unlikely(tribe_def.is_null())) {
				err = context->create_error(vformat("cannot read tribe display type of missing tribe %d", tribe_param->get_tribe()));
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterBoolean::make(tribe_def->get_display() == TribeDef::WIDE));
		}

		err = context->create_error(vformat("cannot read tribe display type of parameter type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawParameter", "Type", def->get_type())));
		return JigsawExecutionState::ERROR;
	case STAT_DEF:
		if (likely(def->get_type() == JigsawParameter::STAT_VALUE)) {
			Ref<JigsawParameterStatValue> stat_param = def;

			return set_command_result(context, err, 0, JigsawParameterStat::make(stat_param->get_stat()));
		}

		err = context->create_error(vformat("cannot read stat ID of parameter type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawParameter", "Type", def->get_type())));
		return JigsawExecutionState::ERROR;
	case STAT_NAME:
		if (likely(def->get_type() == JigsawParameter::STAT_VALUE)) {
			Ref<JigsawParameterStatValue> stat_param = def;
			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<StatDef> stat_def = likely(mode.is_valid()) ? mode->get_stat(stat_param->get_stat()) : Ref<StatDef>();

			if (unlikely(stat_def.is_null())) {
				err = context->create_error(vformat("cannot read stat name of missing stat %d", stat_param->get_stat()));
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterString::make(stat_def->get_name()));
		}

		if (likely(def->get_type() == JigsawParameter::STAT)) {
			Ref<JigsawParameterStat> stat_param = def;
			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<StatDef> stat_def = likely(mode.is_valid()) ? mode->get_stat(stat_param->get_stat()) : Ref<StatDef>();

			if (unlikely(stat_def.is_null())) {
				err = context->create_error(vformat("cannot read stat name of missing stat %d", stat_param->get_stat()));
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterString::make(stat_def->get_name()));
		}

		err = context->create_error(vformat("cannot read stat name of parameter type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawParameter", "Type", def->get_type())));
		return JigsawExecutionState::ERROR;
	case STAT_ICON:
		if (likely(def->get_type() == JigsawParameter::STAT_VALUE)) {
			Ref<JigsawParameterStatValue> stat_param = def;
			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<StatDef> stat_def = likely(mode.is_valid()) ? mode->get_stat(stat_param->get_stat()) : Ref<StatDef>();

			if (unlikely(stat_def.is_null())) {
				err = context->create_error(vformat("cannot read stat icon of missing stat %d", stat_param->get_stat()));
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterIcon::make(stat_def->get_icon()));
		}

		if (likely(def->get_type() == JigsawParameter::STAT)) {
			Ref<JigsawParameterStat> stat_param = def;
			JigsawGlobal *global = context->get_global();
			Ref<GameMode> mode = likely(global) ? global->get_mode() : Ref<GameMode>();
			Ref<StatDef> stat_def = likely(mode.is_valid()) ? mode->get_stat(stat_param->get_stat()) : Ref<StatDef>();

			if (unlikely(stat_def.is_null())) {
				err = context->create_error(vformat("cannot read stat icon of missing stat %d", stat_param->get_stat()));
				return JigsawExecutionState::ERROR;
			}

			return set_command_result(context, err, 0, JigsawParameterIcon::make(stat_def->get_icon()));
		}

		err = context->create_error(vformat("cannot read stat icon of parameter type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawParameter", "Type", def->get_type())));
		return JigsawExecutionState::ERROR;
	case STAT_AMOUNT:
		if (likely(def->get_type() == JigsawParameter::STAT_VALUE)) {
			Ref<JigsawParameterStatValue> stat_param = def;

			return set_command_result(context, err, 0, stat_param->is_nan() ? JigsawParameterAmount::make_nan() : JigsawParameterAmount::make(stat_param->get_amount(), stat_param->get_amount_inf()));
		}

		err = context->create_error(vformat("cannot read stat amount of parameter type %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawParameter", "Type", def->get_type())));
		return JigsawExecutionState::ERROR;
	}

	err = context->create_error(vformat("internal error: missing handler for Lookup Definition Property %s", WhyIsntThisInGodot::find_builtin_enum_key_name("JigsawCommandLookupDefinitionProperty", "Property", _property)));
	return JigsawExecutionState::ERROR;
}

int64_t JigsawCommandLookupDefinitionProperty::get_num_configs() const {
	return 1;
}
String JigsawCommandLookupDefinitionProperty::get_config_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "Property";
}
String JigsawCommandLookupDefinitionProperty::get_config_desc(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	return "";
}
int64_t JigsawCommandLookupDefinitionProperty::get_config_value(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, 0);

	return _property;
}
void JigsawCommandLookupDefinitionProperty::set_config_value(int64_t i, int64_t value) {
	ERR_FAIL_INDEX(i, 1);

	_property = static_cast<Property>(value);
	emit_changed();
}
PackedStringArray JigsawCommandLookupDefinitionProperty::get_config_options(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, PackedStringArray());

	// if any of these asserts fail, you've broken compatibility with existing game modes.
	static_assert(CARD_DEF == 0);
	static_assert(CARD_NAME == 1);
	static_assert(CARD_RANK == 2);
	static_assert(CARD_COSTS == 3);
	static_assert(CARD_PORTRAIT == 4);
	static_assert(CARD_TRIBES == 5);
	static_assert(CARD_EFFECTS == 6);
	static_assert(RANK_NAME == 7);
	static_assert(RANK_COLOR == 8);
	static_assert(RANK_COLOR_ALT == 9);
	static_assert(RANK_COLOR_BASED_ON_PREFERENCES == 10);
	static_assert(RANK_FRONT == 11);
	static_assert(RANK_BACK == 12);
	static_assert(TRIBE_NAME == 13);
	static_assert(TRIBE_COLOR == 14);
	static_assert(TRIBE_IS_HIDDEN == 15);
	static_assert(TRIBE_IS_WIDE == 16);
	static_assert(STAT_DEF == 17);
	static_assert(STAT_NAME == 18);
	static_assert(STAT_ICON == 19);

	return PackedStringArray{
		"Card: Definition",
		"Card: Name",
		"Card: Rank",
		"Card: Costs",
		"Card: Portrait",
		"Card: Tribes",
		"Card: Effects",
		"Rank: Name",
		"Rank: Color (default)",
		"Rank: Color (alternate)",
		"Rank: Color (based on preferences)",
		"Rank: Front texture",
		"Rank: Back texture",
		"Tribe: Name",
		"Tribe: Color",
		"Tribe: Hidden",
		"Tribe: Wide",
		"Stat: Definition",
		"Stat: Name",
		"Stat: Icon",
		"Stat: Amount",
	};
}

int64_t JigsawCommandLookupDefinitionProperty::get_num_arguments() const {
	return 1;
}
Ref<JigsawParameter> JigsawCommandLookupDefinitionProperty::get_argument(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, Ref<JigsawParameter>());

	return _definition;
}
TypedArray<JigsawParameter> JigsawCommandLookupDefinitionProperty::get_argument_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, 1, TypedArray<JigsawParameter>());

	switch (_property) {
	case CARD_DEF:
		return Array::make(JigsawParameterCardInstance::make(Ref<CardInstance>()));
	case CARD_NAME:
	case CARD_RANK:
	case CARD_COSTS:
	case CARD_PORTRAIT:
	case CARD_TRIBES:
	case CARD_EFFECTS:
		return Array::make(JigsawParameterCard::make(enums::CardDef::NONE), JigsawParameterCardInstance::make(Ref<CardInstance>()));
	case RANK_NAME:
	case RANK_COLOR:
	case RANK_COLOR_ALT:
	case RANK_COLOR_BASED_ON_PREFERENCES:
	case RANK_FRONT:
	case RANK_BACK:
		return Array::make(JigsawParameterRank::make(enums::RankDef::NONE), JigsawParameterCard::make(enums::CardDef::NONE), JigsawParameterCardInstance::make(Ref<CardInstance>()));
	case TRIBE_NAME:
	case TRIBE_COLOR:
	case TRIBE_IS_HIDDEN:
	case TRIBE_IS_WIDE:
		return Array::make(JigsawParameterTribe::make(enums::TribeDef::NONE));
	case STAT_DEF:
		return Array::make(JigsawParameterStatValue::make(enums::StatDef::NONE, 0));
	case STAT_NAME:
	case STAT_ICON:
		return Array::make(JigsawParameterStat::make(enums::StatDef::NONE), JigsawParameterStatValue::make(enums::StatDef::NONE, 0));
	case STAT_AMOUNT:
		return Array::make(JigsawParameterStatValue::make(enums::StatDef::NONE, 0));
	}

	return TypedArray<JigsawParameter>();
}
void JigsawCommandLookupDefinitionProperty::set_argument(int64_t i, const Ref<JigsawParameter> &arg) {
	ERR_FAIL_INDEX(i, 1);

	_definition = arg;
	emit_changed();
}
String JigsawCommandLookupDefinitionProperty::get_argument_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	switch (_property) {
	case CARD_DEF:
	case CARD_NAME:
	case CARD_RANK:
	case CARD_COSTS:
	case CARD_PORTRAIT:
	case CARD_TRIBES:
	case CARD_EFFECTS:
		return "card";
	case RANK_NAME:
	case RANK_COLOR:
	case RANK_COLOR_ALT:
	case RANK_COLOR_BASED_ON_PREFERENCES:
	case RANK_FRONT:
	case RANK_BACK:
		return "rank";
	case TRIBE_NAME:
	case TRIBE_COLOR:
	case TRIBE_IS_HIDDEN:
	case TRIBE_IS_WIDE:
		return "tribe";
	case STAT_DEF:
	case STAT_NAME:
	case STAT_ICON:
	case STAT_AMOUNT:
		return "stat";
	}

	return "";
}

int64_t JigsawCommandLookupDefinitionProperty::get_num_results() const {
	return 1;
}
Ref<JigsawParameterLocalVariable> JigsawCommandLookupDefinitionProperty::get_result(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, Ref<JigsawParameterLocalVariable>());

	return _value;
}
TypedArray<JigsawParameter> JigsawCommandLookupDefinitionProperty::get_result_template(int64_t i, const Ref<JigsawContext> &context) const {
	ERR_FAIL_INDEX_V(i, 1, TypedArray<JigsawParameter>());

	switch (_property) {
	case CARD_DEF:
		return Array::make(JigsawParameterCard::make(enums::CardDef::NONE));
	case CARD_NAME:
		return Array::make(JigsawParameterFormattedText::make(TypedArray<FormattedText>()));
	case CARD_RANK:
		return Array::make(JigsawParameterRank::make(enums::RankDef::NONE));
	case CARD_COSTS:
		return Array::make(JigsawParameterOrderedList::make_template(Array::make(JigsawParameterStatValue::make(enums::StatDef::NONE, 0))));
	case CARD_PORTRAIT:
		return Array::make(JigsawParameterIcon::make(enums::IconDef::NONE));
	case CARD_TRIBES:
		return Array::make(JigsawParameterOrderedList::make_template(Array::make(JigsawParameterTribe::make(enums::TribeDef::NONE))));
	case CARD_EFFECTS:
		return Array::make(JigsawParameterOrderedList::make_template(Array::make(JigsawParameterEffectInstance::make(Ref<EffectInstance>()))));
	case RANK_NAME:
		return Array::make(JigsawParameterString::make(""));
	case RANK_COLOR:
	case RANK_COLOR_ALT:
	case RANK_COLOR_BASED_ON_PREFERENCES:
		return Array::make(JigsawParameterColor::make(Color()));
	case RANK_FRONT:
	case RANK_BACK:
		return Array::make(JigsawParameterIcon::make(enums::IconDef::NONE));
	case TRIBE_NAME:
		return Array::make(JigsawParameterString::make(""));
	case TRIBE_COLOR:
		return Array::make(JigsawParameterColor::make(Color()));
	case TRIBE_IS_HIDDEN:
	case TRIBE_IS_WIDE:
		return Array::make(JigsawParameterBoolean::make(false));
	case STAT_DEF:
		return Array::make(JigsawParameterStat::make(enums::StatDef::NONE));
	case STAT_NAME:
		return Array::make(JigsawParameterString::make(""));
	case STAT_ICON:
		return Array::make(JigsawParameterIcon::make(enums::IconDef::NONE));
	case STAT_AMOUNT:
		return Array::make(JigsawParameterAmount::make(0));
	}

	return TypedArray<JigsawParameter>();
}
void JigsawCommandLookupDefinitionProperty::set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) {
	ERR_FAIL_INDEX(i, 1);

	_value = result;
	emit_changed();
}
String JigsawCommandLookupDefinitionProperty::get_result_name(int64_t i) const {
	ERR_FAIL_INDEX_V(i, 1, "");

	switch (_property) {
	case CARD_DEF:
		return "def";
	case CARD_NAME:
		return "name";
	case CARD_RANK:
		return "rank";
	case CARD_COSTS:
		return "costs";
	case CARD_PORTRAIT:
		return "portrait";
	case CARD_TRIBES:
		return "tribes";
	case CARD_EFFECTS:
		return "effects";
	case RANK_NAME:
		return "name";
	case RANK_COLOR:
		return "color";
	case RANK_COLOR_ALT:
		return "color";
	case RANK_COLOR_BASED_ON_PREFERENCES:
		return "color";
	case RANK_FRONT:
		return "front";
	case RANK_BACK:
		return "back";
	case TRIBE_NAME:
		return "name";
	case TRIBE_COLOR:
		return "color";
	case TRIBE_IS_HIDDEN:
		return "is_hidden";
	case TRIBE_IS_WIDE:
		return "is_wide";
	case STAT_DEF:
		return "def";
	case STAT_NAME:
		return "name";
	case STAT_ICON:
		return "icon";
	case STAT_AMOUNT:
		return "amount";
	}

	return "";
}
