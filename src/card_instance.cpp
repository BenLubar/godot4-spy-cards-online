#include "card_instance.h"

#include "jigsaw_parameter_boolean.h"
#include "jigsaw_parameter_color.h"
#include "jigsaw_parameter_string.h"

void CardInstance::_bind_methods() {
	BIND_PROPERTY(Variant::OBJECT, global);
	BIND_PROPERTY_RESOURCE(CardDef, def);
	BIND_PROPERTY_RESOURCE_ARRAY(FormattedText, name);
	BIND_PROPERTY(Variant::INT, rank);
	BIND_PROPERTY(Variant::INT, back);
	BIND_PROPERTY_RESOURCE_ARRAY(StatValue, costs);
	BIND_PROPERTY(Variant::INT, portrait);
	BIND_PROPERTY(Variant::ARRAY, tribes);
	BIND_PROPERTY_RESOURCE_ARRAY(FormattedText, description);
	BIND_PROPERTY(Variant::ARRAY, modifiers);

	ClassDB::bind_static_method("CardInstance", D_METHOD("make", "global", "def"), &CardInstance::make);
}

IMPLEMENT_PROPERTY_SIMPLE(CardInstance, JigsawGlobal *, global);
IMPLEMENT_PROPERTY_SIMPLE(CardInstance, Ref<CardDef>, def);
IMPLEMENT_PROPERTY_SIMPLE(CardInstance, TypedArray<FormattedText>, name);
IMPLEMENT_PROPERTY_SIMPLE(CardInstance, RankDef::Rank, rank);
IMPLEMENT_PROPERTY_SIMPLE(CardInstance, IconDef::Icon, back);
IMPLEMENT_PROPERTY_SIMPLE(CardInstance, TypedArray<StatValue>, costs);
IMPLEMENT_PROPERTY_SIMPLE(CardInstance, IconDef::Icon, portrait);
IMPLEMENT_PROPERTY_SIMPLE(CardInstance, TypedArray<TribeDef::Tribe>, tribes);
IMPLEMENT_PROPERTY_SIMPLE(CardInstance, TypedArray<FormattedText>, description);
IMPLEMENT_PROPERTY_SIMPLE(CardInstance, TypedArray<ModifierInstance>, modifiers);

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

	// TODO
	TypedArray<FormattedText> description;
	bool first = true;
	TypedArray<EffectInstance> effects = def->get_effects();
	for (int64_t i = 0; i < effects.size(); i++) {
		EffectInstance *e = Object::cast_to<EffectInstance>(effects[i]);
		if (!e) {
			continue;
		}
		if (first) {
			first = false;
		} else {
			description.append_array(FormattedText::make_plain("\n"));
		}

		FormattedText *before = memnew(FormattedText);
		before->set_command(FormattedText::PUSH_EFFECT_INSTANCE);
		before->set_instance(e);
		description.append(before);

		if (e->get_effect() == EffectDef::FLAVOR_TEXT) {
			FormattedText *flavor = memnew(FormattedText);
			flavor->set_command(FormattedText::PUSH_TEXT_COLOR);
			// TODO: this is unsafe and could cause crashes on malformed effects but also it's temporary
			flavor->set_color(Object::cast_to<JigsawParameterColor>(e->get_params()[1])->get_color());
			description.append(flavor);

			flavor = memnew(FormattedText);
			flavor->set_command(FormattedText::ADD_TEXT);
			// TODO: this is also unsafe and also temporary
			flavor->set_text(Object::cast_to<JigsawParameterString>(e->get_params()[0])->get_string());
			description.append(flavor);

			flavor = memnew(FormattedText);
			flavor->set_command(FormattedText::POP);
			description.append(flavor);

			// TODO: again
			if (Object::cast_to<JigsawParameterBoolean>(e->get_params()[2])->get_boolean()) {
				flavor = memnew(FormattedText);
				flavor->set_command(FormattedText::FORCE_END_OF_TEXT);
				description.append(flavor);
			}
		} else {
			FormattedText *TODO = memnew(FormattedText);
			TODO->set_command(FormattedText::PUSH_SHAKE);
			description.append(TODO);

			PackedStringArray names = ClassDB::class_get_enum_constants("EffectDef", "Effect");
			for (int64_t j = 0; j < names.size(); j++) {
				if (ClassDB::class_get_integer_constant("EffectDef", names[j]) == e->get_effect()) {
					description.append_array(FormattedText::make_plain(names[j]));
					break;
				}
			}

			TODO = memnew(FormattedText);
			TODO->set_command(FormattedText::POP);
			description.append(TODO);
		}

		FormattedText *after = memnew(FormattedText);
		after->set_command(FormattedText::POP);
		description.append(after);
	}
	inst->set_description(description);

	return inst;
}
