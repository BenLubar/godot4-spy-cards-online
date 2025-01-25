#include "active/card_instance.h"

#include "jigsaw/jigsaw_global.h"
#include "jigsaw/parameter/jigsaw_parameter_boolean.h"
#include "jigsaw/parameter/jigsaw_parameter_color.h"
#include "jigsaw/parameter/jigsaw_parameter_string.h"

void CardInstance::_bind_methods() {
	BIND_PROPERTY(Variant::INT, index);
	BIND_PROPERTY_RESOURCE(CardDef, def);
	BIND_PROPERTY_RESOURCE_ARRAY(FormattedText, name);
	BIND_PROPERTY_ENUM(enums::RankDef::Rank, rank);
	BIND_PROPERTY_ENUM(enums::IconDef::Icon, back);
	BIND_PROPERTY_RESOURCE_ARRAY(StatValue, costs);
	BIND_PROPERTY_ENUM(enums::IconDef::Icon, portrait);
	BIND_PROPERTY_RESOURCE_ARRAY(EffectInstance, effects);
	BIND_PROPERTY_PACKED_ENUM_ARRAY(enums::TribeDef::Tribe, tribes);
	BIND_PROPERTY_RESOURCE_ARRAY(FormattedText, description);
	BIND_PROPERTY_RESOURCE_ARRAY(FormattedTextWithIcon, simple_description);
	BIND_PROPERTY_RESOURCE_ARRAY(ModifierInstance, modifiers);
	BIND_PROPERTY(Variant::PACKED_INT32_ARRAY, face_down_for_side);
	BIND_PROPERTY(Variant::INT, linked_parent);

	ClassDB::bind_method(D_METHOD("update_description", "global", "parent_context"), &CardInstance::update_description);
	ClassDB::bind_method(D_METHOD("update_simple_description", "global", "parent_context"), &CardInstance::update_simple_description);
	ClassDB::bind_method(D_METHOD("description_requires_update"), &CardInstance::description_requires_update);
	ClassDB::bind_method(D_METHOD("get_design", "global"), &CardInstance::get_design);
	ClassDB::bind_method(D_METHOD("assign", "instance"), &CardInstance::assign);
	ClassDB::bind_method(D_METHOD("write_hash", "hash"), &CardInstance::write_hash);

	ClassDB::bind_static_method("CardInstance", D_METHOD("make", "global", "def"), &CardInstance::make);
}

IMPLEMENT_PROPERTY(CardInstance, int64_t, index);
IMPLEMENT_PROPERTY(CardInstance, Ref<CardDef>, def);
IMPLEMENT_PROPERTY(CardInstance, TypedArray<FormattedText>, name);
IMPLEMENT_PROPERTY(CardInstance, enums::RankDef::Rank, rank);
IMPLEMENT_PROPERTY(CardInstance, enums::IconDef::Icon, back);
IMPLEMENT_PROPERTY(CardInstance, TypedArray<StatValue>, costs);
IMPLEMENT_PROPERTY(CardInstance, enums::IconDef::Icon, portrait);
IMPLEMENT_PROPERTY(CardInstance, TypedArray<EffectInstance>, effects);
IMPLEMENT_PROPERTY(CardInstance, PackedArray<enums::TribeDef::Tribe>, tribes);
IMPLEMENT_PROPERTY(CardInstance, TypedArray<FormattedText>, description);
IMPLEMENT_PROPERTY(CardInstance, TypedArray<FormattedTextWithIcon>, simple_description);
IMPLEMENT_PROPERTY(CardInstance, TypedArray<ModifierInstance>, modifiers);
IMPLEMENT_PROPERTY(CardInstance, PackedInt32Array, face_down_for_side);
IMPLEMENT_PROPERTY(CardInstance, int64_t, linked_parent);

Ref<CardInstance> CardInstance::make(JigsawGlobal *global, const Ref<CardDef> &def) {
	ERR_FAIL_NULL_V(global, nullptr);
	ERR_FAIL_COND_V(def.is_null(), nullptr);
	Ref<GameMode> mode = global->get_mode();
	ERR_FAIL_COND_V(mode.is_null(), nullptr);

	TypedArray<CardInstance> cards = global->get_state()->get_cards();
	int64_t index = cards.find(Ref<CardInstance>());
	if (index == -1) {
		index = cards.size();
		cards.append(Ref<CardInstance>());
		global->get_state()->set_cards(cards);
	}

	Ref<CardInstance> inst;
	inst.instantiate();
	cards[index] = inst;
	inst->set_index(index);
	inst->set_def(def);
	inst->set_name(FormattedText::make_plain(def->get_name()));

	inst->set_rank(def->get_rank());
	Ref<RankDef> rank = mode->get_rank(def->get_rank());
	inst->set_back(rank.is_valid() ? rank->get_back() : IconDef::Icon::NONE);
	inst->set_costs(Array(def->get_costs()));
	inst->set_portrait(def->get_portrait());
	inst->set_effects(Array(def->get_effects()));
	inst->set_tribes(Array(def->get_tribes()));

	if (!inst->update_simple_description(global)) {
		inst->update_description(global);
	}

	return inst;
}

void CardInstance::update_description(JigsawGlobal *global, const Ref<JigsawContext> &parent_context) {
	TypedArray<FormattedText> description;
	bool first = true;
	for (int64_t i = 0; i < _effects.size(); i++) {
		Ref<EffectInstance> effect = _effects[i];
		ERR_CONTINUE(effect.is_null());

		TypedArray<FormattedText> effect_description = effect->format_description(global, get_index(), parent_context);
		if (effect_description.is_empty()) {
			continue;
		}

		if (first) {
			first = false;
		} else {
			description.append_array(FormattedText::make_plain("\n"));
		}

		description.append_array(effect_description);
	}
	
	set_description(description);
}

bool CardInstance::update_simple_description(JigsawGlobal *global, const Ref<JigsawContext> &parent_context) {
	TypedArray<FormattedTextWithIcon> simple_description;
	for (int64_t i = 0; i < _effects.size(); i++) {
		Ref<EffectInstance> e = _effects[i];
		ERR_CONTINUE(e.is_null());

		Ref<FormattedTextWithIcon> simple_desc = e->format_simple_description(global, get_index(), parent_context);
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

Ref<CardDesign> CardInstance::get_design(JigsawGlobal *global) const {
	ERR_FAIL_NULL_V(global, Ref<CardDesign>());
	Ref<GameMode> mode = global->get_mode();
	ERR_FAIL_COND_V(mode.is_null(), Ref<CardDesign>());
	Ref<RankDef> rank = mode->get_rank(get_rank());
	ERR_FAIL_COND_V(rank.is_null(), Ref<CardDesign>());

	if (rank->get_custom_design().is_valid()) {
		return rank->get_custom_design();
	}

	return mode->get_default_card_design();
}

void CardInstance::assign(const Ref<CardInstance> &instance) {
	ERR_FAIL_COND(instance.is_null());

	_index = instance->_index;
	_def = instance->_def;
	_name.assign(instance->_name);
	_rank = instance->_rank;
	_back = instance->_back;
	_costs.assign(instance->_costs);
	_portrait = instance->_portrait;
	_effects.assign(instance->_effects);
	_tribes = instance->_tribes;
	_description.assign(instance->_description);
	_simple_description.assign(instance->_simple_description);
	_modifiers = instance->_modifiers.duplicate(true);
	_face_down_for_side = instance->_face_down_for_side;
	_linked_parent = instance->_linked_parent;
}
