#include "effect_instance.h"

#include "card_instance.h"
#include "jigsaw_global.h"
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
	if (def.is_null()) {
		Ref<FormattedText> before_text;
		before_text.instantiate();
		before_text->set_command(FormattedText::PUSH_FONT_MONO);
		description.append(before_text);

		description.append_array(FormattedText::make_plain(vformat("missing effect #%d", get_effect())));

		Ref<FormattedText> after_text;
		after_text.instantiate();
		after_text->set_command(FormattedText::POP);
		description.append(after_text);
	} else if (def->get_describe().is_null()) {
		Ref<FormattedText> before_text;
		before_text.instantiate();
		before_text->set_command(FormattedText::PUSH_FONT_MONO);
		description.append(before_text);

		description.append_array(FormattedText::make_plain(def->get_editor_name()));

		Ref<FormattedText> after_text;
		after_text.instantiate();
		after_text->set_command(FormattedText::POP);
		description.append(after_text);
	} else {
		Ref<JigsawParameterFormattedText> effect_description;
		effect_description.instantiate();

		Ref<JigsawContext> ctx = JigsawContext::make(card->get_global(), parent_context);
		ctx->set_read_only(true);

		TypedArray<JigsawParameter> env; // TODO
		TypedArray<JigsawParameter> results = Array::make(effect_description);

		TypedArray<JigsawError> errors = ctx->validate(def->get_describe(), env, params, results, "describe effect " + def->get_editor_name() + " on " + card->get_def()->get_name(), def->get_parameter_names(), PackedStringArray{"formatted_text"});
		if (!errors.is_empty()) {
			Ref<FormattedText> before_text;
			before_text.instantiate();
			before_text->set_command(FormattedText::PUSH_FONT_MONO);
			description.append(before_text);

			description.append_array(FormattedText::make_plain("error in describe for " + def->get_editor_name()));

			Ref<FormattedText> after_text;
			after_text.instantiate();
			after_text->set_command(FormattedText::POP);
			description.append(after_text);
		}

		Ref<JigsawError> error = ctx->run(def->get_describe(), env, params, results);
		if (error.is_valid()) {
			Ref<FormattedText> before_text;
			before_text.instantiate();
			before_text->set_command(FormattedText::PUSH_FONT_MONO);
			description.append(before_text);

			description.append_array(FormattedText::make_plain("error in describe for " + def->get_editor_name()));

			Ref<FormattedText> after_text;
			after_text.instantiate();
			after_text->set_command(FormattedText::POP);
			description.append(after_text);
		} else {
			effect_description = results[0];
			if (effect_description.is_valid()) {
				description.append_array(effect_description->get_text());
			}
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

	TypedArray<FormattedText> description;

	Ref<FormattedText> before;
	before.instantiate();
	before->set_command(FormattedText::PUSH_EFFECT_INSTANCE);
	before->set_instance(const_cast<EffectInstance *>(this));
	description.append(before);

	Ref<JigsawParameterIcon> icon;
	icon.instantiate();

	Ref<JigsawParameterFormattedText> effect_description;
	effect_description.instantiate();

	Ref<JigsawContext> ctx = JigsawContext::make(card->get_global(), parent_context);
	ctx->set_read_only(true);

	TypedArray<JigsawParameter> env; // TODO
	TypedArray<JigsawParameter> results = Array::make(icon, effect_description);

	TypedArray<JigsawError> errors = ctx->validate(def->get_describe(), env, params, results, "simple describe effect " + def->get_editor_name() + " on " + card->get_def()->get_name(), def->get_parameter_names(), PackedStringArray{"icon", "formatted_text"});
	if (!errors.is_empty()) {
		return Ref<FormattedTextWithIcon>();
	}

	Ref<JigsawError> error = ctx->run(def->get_describe(), env, params, results);
	if (error.is_valid()) {
		return Ref<FormattedTextWithIcon>();
	}

	icon = results[0];
	effect_description = results[1];
	if (effect_description.is_valid()) {
		description.append_array(effect_description->get_text());
	}

	Ref<FormattedText> after;
	after.instantiate();
	after->set_command(FormattedText::POP);
	description.append(after);

	Ref<FormattedTextWithIcon> with_icon;
	with_icon.instantiate();
	if (icon.is_valid()) {
		with_icon->set_icon(icon->get_icon());
	}
	with_icon->set_text(description);

	return with_icon;
}
