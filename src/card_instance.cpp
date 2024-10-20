#include "card_instance.h"

#include "jigsaw_global.h"
#include "jigsaw_parameter_boolean.h"
#include "jigsaw_parameter_color.h"
#include "jigsaw_parameter_string.h"

void CardInstance::_bind_methods() {
	BIND_PROPERTY_RESOURCE(JigsawGlobal, global);
	BIND_PROPERTY_RESOURCE(CardDef, def);
	BIND_PROPERTY_RESOURCE_ARRAY(FormattedText, name);
	BIND_PROPERTY_ENUM(enums::RankDef::Rank, rank);
	BIND_PROPERTY_ENUM(enums::IconDef::Icon, back);
	BIND_PROPERTY_RESOURCE_ARRAY(StatValue, costs);
	BIND_PROPERTY_ENUM(enums::IconDef::Icon, portrait);
	BIND_PROPERTY_ENUM_ARRAY(enums::TribeDef::Tribe, tribes);
	BIND_PROPERTY_RESOURCE_ARRAY(FormattedText, description);
	BIND_PROPERTY_RESOURCE_ARRAY(FormattedTextWithIcon, simple_description);
	BIND_PROPERTY_RESOURCE_ARRAY(ModifierInstance, modifiers);

	ClassDB::bind_method(D_METHOD("update_description"), &CardInstance::update_description);
	ClassDB::bind_method(D_METHOD("update_simple_description"), &CardInstance::update_simple_description);
	ClassDB::bind_method(D_METHOD("description_requires_update"), &CardInstance::description_requires_update);

	ClassDB::bind_static_method("CardInstance", D_METHOD("make", "global", "def"), &CardInstance::make);
}

IMPLEMENT_PROPERTY(CardInstance, JigsawGlobal *, global);
IMPLEMENT_PROPERTY(CardInstance, Ref<CardDef>, def);
IMPLEMENT_PROPERTY(CardInstance, TypedArray<FormattedText>, name);
IMPLEMENT_PROPERTY(CardInstance, enums::RankDef::Rank, rank);
IMPLEMENT_PROPERTY(CardInstance, enums::IconDef::Icon, back);
IMPLEMENT_PROPERTY(CardInstance, TypedArray<StatValue>, costs);
IMPLEMENT_PROPERTY(CardInstance, enums::IconDef::Icon, portrait);
IMPLEMENT_PROPERTY(CardInstance, TypedArray<enums::TribeDef::Tribe>, tribes);
IMPLEMENT_PROPERTY(CardInstance, TypedArray<FormattedText>, description);
IMPLEMENT_PROPERTY(CardInstance, TypedArray<FormattedTextWithIcon>, simple_description);
IMPLEMENT_PROPERTY(CardInstance, TypedArray<ModifierInstance>, modifiers);

Ref<CardInstance> CardInstance::make(JigsawGlobal *global, const Ref<CardDef> &def) {
	ERR_FAIL_NULL_V(global, nullptr);
	ERR_FAIL_COND_V(def.is_null(), nullptr);
	Ref<GameMode> mode = global->get_mode();
	ERR_FAIL_COND_V(mode.is_null(), nullptr);

	Ref<CardInstance> inst;
	inst.instantiate();
	inst->set_global(global);
	inst->set_def(def);
	inst->set_name(FormattedText::make_plain(def->get_name()));

	inst->set_rank(def->get_rank());
	Ref<RankDef> rank = mode->get_rank(def->get_rank());
	inst->set_back(rank.is_valid() ? rank->get_back() : IconDef::Icon::NONE);
	inst->set_costs(Array(def->get_costs()));
	inst->set_portrait(def->get_portrait());
	inst->set_tribes(Array(def->get_tribes()));

	if (!inst->update_simple_description()) {
		inst->update_description();
	}

	return inst;
}

void CardInstance::update_description() {
	Ref<CardDef> card_def = get_def();
	TypedArray<EffectInstance> effects = card_def->get_effects();

	TypedArray<FormattedText> description;
	bool first = true;
	for (int64_t i = 0; i < effects.size(); i++) {
		Ref<EffectInstance> e = effects[i];
		ERR_CONTINUE(e.is_null());

		if (first) {
			first = false;
		} else {
			description.append_array(FormattedText::make_plain("\n"));
		}

		description.append_array(e->format_description(this));
	}
	
	set_description(description);
}

bool CardInstance::update_simple_description() {
	Ref<CardDef> card_def = get_def();
	TypedArray<EffectInstance> effects = card_def->get_effects();

	TypedArray<FormattedTextWithIcon> simple_description;
	for (int64_t i = 0; i < effects.size(); i++) {
		Ref<EffectInstance> e = effects[i];
		ERR_CONTINUE(e.is_null());

		Ref<FormattedTextWithIcon> simple_desc = e->format_simple_description(this);
		if (simple_desc.is_null()) {
			set_simple_description(TypedArray<FormattedTextWithIcon>());
			return false;
		}
		if (simple_desc->get_text().size() == 0 && simple_desc->get_icon() == enums::IconDef::Icon::NONE) {
			continue;
		}

		simple_description.append(simple_desc);

		for (int64_t j = 0; j < simple_desc->get_text().size(); j++) {
			Ref<FormattedText> command = simple_desc->get_text()[j];
			if (command->get_command() == FormattedText::FORCE_END_OF_TEXT) {
				set_simple_description(simple_description);
				return true;
			}
		}
	}
	
	set_simple_description(simple_description);
	return true;
}

bool CardInstance::description_requires_update() const {
	for (int64_t i = 0; i < _description.size(); i++) {
		Ref<FormattedText> text = _description[i];
		if (text.is_null()) {
			continue;
		}
		if (FormattedText::requires_update(text->get_command())) {
			return true;
		}
		if (text->get_command() == FormattedText::FORCE_END_OF_TEXT) {
			break;
		}
	}

	for (int64_t i = 0; i < _simple_description.size(); i++) {
		Ref<FormattedTextWithIcon> text_with_icon = _simple_description[i];
		if (text_with_icon.is_null()) {
			continue;
		}
		TypedArray<FormattedText> icon_text = text_with_icon->get_text();
		for (int64_t j = 0; j < icon_text.size(); j++) {
			Ref<FormattedText> text = icon_text[j];
			if (text.is_null()) {
				continue;
			}
			if (FormattedText::requires_update(text->get_command())) {
				return true;
			}
			if (text->get_command() == FormattedText::FORCE_END_OF_TEXT) {
				break;
			}
		}
	}

	return false;
}
