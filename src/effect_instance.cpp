#include "effect_instance.h"

#include "card_instance.h"
#include "jigsaw_global.h"
#include "jigsaw_parameter_card_instance.h"
#include "jigsaw_parameter_effect_instance.h"
#include "jigsaw_parameter_formatted_text.h"
#include "jigsaw_parameter_icon.h"

void EffectInstance::_bind_methods() {
	BIND_PROPERTY_ENUM(EffectDef::Effect, effect);
	BIND_PROPERTY(Variant::INT, priority);
	BIND_PROPERTY_RESOURCE_ARRAY(JigsawParameter, params);
}

IMPLEMENT_PROPERTY(EffectInstance, EffectDef::Effect, effect);
IMPLEMENT_PROPERTY(EffectInstance, int64_t, priority);
IMPLEMENT_PROPERTY(EffectInstance, TypedArray<JigsawParameter>, params);

TypedArray<FormattedText> EffectInstance::format_description(const Ref<CardInstance> &card, const Ref<JigsawContext> &parent_context) const {
	TypedArray<FormattedText> description;

	Ref<FormattedText> before;
	before.instantiate();
	before->set_command(FormattedText::PUSH_EFFECT_INSTANCE);
	before->set_instance(const_cast<EffectInstance *>(this));
	description.append(before);

	ERR_FAIL_COND_V(card.is_null(), FormattedText::make_plain("MISSING CARD"));
	ERR_FAIL_NULL_V(card->get_global(), FormattedText::make_plain("MISSING GLOBAL"));
	ERR_FAIL_COND_V(card->get_global()->get_mode().is_null(), FormattedText::make_plain("MISSING MODE"));

	Ref<EffectDef> def = card->get_global()->get_mode()->get_effect(get_effect());
	Ref<JigsawProcedureEffectDescribe> describe = def.is_valid() ? def->get_describe() : Ref<JigsawProcedureEffectDescribe>();
	if (def.is_null()) {
		Ref<FormattedText> before_text;
		before_text.instantiate();
		before_text->set_command(FormattedText::PUSH_FONT_MONO);
		description.append(before_text);

		Ref<FormattedText> before_text2;
		before_text2.instantiate();
		before_text2->set_command(FormattedText::PUSH_TEXT_COLOR);
		before_text2->set_color(Color(1.0, 0.0, 0.0, 1.0));
		description.append(before_text2);

		description.append_array(FormattedText::make_plain(vformat("missing effect #%d", get_effect())));

		Ref<FormattedText> after_text2;
		after_text2.instantiate();
		after_text2->set_command(FormattedText::POP);
		description.append(after_text2);

		Ref<FormattedText> after_text;
		after_text.instantiate();
		after_text->set_command(FormattedText::POP);
		description.append(after_text);
	} else if (describe.is_null()) {
		Ref<FormattedText> before_text;
		before_text.instantiate();
		before_text->set_command(FormattedText::PUSH_FONT_MONO);
		description.append(before_text);

		Ref<FormattedText> before_text2;
		before_text2.instantiate();
		before_text2->set_command(FormattedText::PUSH_TEXT_COLOR);
		before_text2->set_color(Color(1.0, 0.0, 0.0, 1.0));
		description.append(before_text2);

		description.append_array(FormattedText::make_plain(vformat("missing 'describe' procedure for effect #%d '%s'", get_effect(), def->get_editor_name())));

		Ref<FormattedText> after_text2;
		after_text2.instantiate();
		after_text2->set_command(FormattedText::POP);
		description.append(after_text2);

		Ref<FormattedText> after_text;
		after_text.instantiate();
		after_text->set_command(FormattedText::POP);
		description.append(after_text);
	} else {
		Ref<JigsawParameterFormattedText> effect_description;
		effect_description.instantiate();

		Ref<JigsawContext> ctx = JigsawContext::make(card->get_global(), parent_context);
		ctx->set_procedure(describe);

		TypedArray<JigsawParameter> results = Array::make(effect_description);

		Ref<JigsawError> error = ctx->evaluate(describe->get_commands(), Array::make(
			JigsawParameterCardInstance::make(card),
			JigsawParameterEffectInstance::make(const_cast<EffectInstance *>(this))
		), results);
		if (error.is_valid()) {
			// TODO: log error
			WARN_PRINT(vformat("error in card '%s' effect '%s' describe procedure: %s", card->get_def()->get_name(), def->get_editor_name(), error->get_message()));

			Ref<FormattedText> before_text;
			before_text.instantiate();
			before_text->set_command(FormattedText::PUSH_FONT_MONO);
			description.append(before_text);

			Ref<FormattedText> before_text2;
			before_text2.instantiate();
			before_text2->set_command(FormattedText::PUSH_TEXT_COLOR);
			before_text2->set_color(Color(1.0, 0.0, 0.0, 1.0));
			description.append(before_text2);

			description.append_array(FormattedText::make_plain(vformat("error in 'describe' procedure for effect #%d: '%s': %s", get_effect(), def->get_editor_name(), error->get_message())));

			Ref<FormattedText> after_text2;
			after_text2.instantiate();
			after_text2->set_command(FormattedText::POP);
			description.append(after_text2);

			Ref<FormattedText> after_text;
			after_text.instantiate();
			after_text->set_command(FormattedText::POP);
			description.append(after_text);
		} else {
			effect_description = ctx->get_results()[0];
			TypedArray<FormattedText> effect_description_text = effect_description->get_text();
			if (effect_description_text.is_empty()) {
				return TypedArray<FormattedText>();
			}
			description.append_array(effect_description_text);
		}
	}

	Ref<FormattedText> after;
	after.instantiate();
	after->set_command(FormattedText::POP);
	description.append(after);

	return description;
}

Ref<FormattedTextWithIcon> EffectInstance::format_simple_description(const Ref<CardInstance> &card, const Ref<JigsawContext> &parent_context) const {
	ERR_FAIL_COND_V(card.is_null(), Ref<FormattedTextWithIcon>());
	ERR_FAIL_NULL_V(card->get_global(), Ref<FormattedTextWithIcon>());
	ERR_FAIL_COND_V(card->get_global()->get_mode().is_null(), Ref<FormattedTextWithIcon>());

	Ref<EffectDef> def = card->get_global()->get_mode()->get_effect(get_effect());
	ERR_FAIL_COND_V(def.is_null(), Ref<FormattedTextWithIcon>());

	Ref<JigsawProcedureEffectSimpleDescribe> simple_describe = def->get_simple_describe();
	if (simple_describe.is_null()) {
		return Ref<FormattedTextWithIcon>();
	}

	TypedArray<FormattedText> description;

	Ref<FormattedText> before;
	before.instantiate();
	before->set_command(FormattedText::PUSH_EFFECT_INSTANCE);
	before->set_instance(const_cast<EffectInstance *>(this));
	description.append(before);

	Ref<JigsawParameterFormattedText> effect_description;
	effect_description.instantiate();

	Ref<JigsawParameterIcon> icon;
	icon.instantiate();

	Ref<JigsawContext> ctx = JigsawContext::make(card->get_global(), parent_context);
	ctx->set_procedure(simple_describe);

	TypedArray<JigsawParameter> results = Array::make(effect_description, icon);

	Ref<JigsawError> error = ctx->evaluate(simple_describe, Array::make(
		JigsawParameterCardInstance::make(card),
		JigsawParameterEffectInstance::make(const_cast<EffectInstance *>(this))
	), results);
	if (error.is_valid()) {
		// TODO: log error
		WARN_PRINT(vformat("error in card '%s' effect '%s' simple describe procedure: %s", card->get_def()->get_name(), def->get_editor_name(), error->get_message()));
		return Ref<FormattedTextWithIcon>();
	}

	icon = ctx->get_results()[0];
	effect_description = ctx->get_results()[1];

	TypedArray<FormattedText> effect_description_text = effect_description->get_text();
	description.append_array(effect_description_text);

	Ref<FormattedText> after;
	after.instantiate();
	after->set_command(FormattedText::POP);
	description.append(after);

	Ref<FormattedTextWithIcon> with_icon;
	with_icon.instantiate();
	with_icon->set_icon(icon->get_icon());
	with_icon->set_text(description);

	return with_icon;
}
