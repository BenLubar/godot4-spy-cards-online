#include "jigsaw_trigger.h"

#include "jigsaw/parameter/jigsaw_parameter_amount.h"
#include "jigsaw/parameter/jigsaw_parameter_card.h"
#include "jigsaw/parameter/jigsaw_parameter_character.h"
#include "jigsaw/parameter/jigsaw_parameter_ordered_list.h"
#include "jigsaw/parameter/jigsaw_parameter_string.h"

void JigsawTrigger::_bind_methods() {
}

#define IMPLEMENT_CUSTOM_TRIGGER(m_type) \
	void JigsawTrigger##m_type##Custom::_bind_methods() {} \
	String JigsawTrigger##m_type##Custom::get_editor_name() const { \
		return "TODO"; \
	} \
	String JigsawTrigger##m_type##Custom::get_editor_description() const { \
		return "TODO"; \
	} \
	TypedArray<JigsawParameter> JigsawTrigger##m_type##Custom::get_arguments() const { \
		return TypedArray<JigsawParameter>(); \
	} \
	PackedStringArray JigsawTrigger##m_type##Custom::get_argument_names() const { \
		return PackedStringArray(); \
	} \
	TypedArray<JigsawParameter> JigsawTrigger##m_type##Custom::get_results() const { \
		return TypedArray<JigsawParameter>(); \
	} \
	PackedStringArray JigsawTrigger##m_type##Custom::get_result_names() const { \
		return PackedStringArray(); \
	}

void JigsawTriggerEffect::_bind_methods() {
	BIND_ENUM_CONSTANT(CUSTOM);

	ClassDB::bind_method(D_METHOD("get_type"), &JigsawTriggerEffect::get_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "type"), "", "get_type");
}

IMPLEMENT_CUSTOM_TRIGGER(Effect);

void JigsawTriggerModifier::_bind_methods() {
	BIND_ENUM_CONSTANT(CUSTOM);

	ClassDB::bind_method(D_METHOD("get_type"), &JigsawTriggerModifier::get_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "type"), "", "get_type");
}

IMPLEMENT_CUSTOM_TRIGGER(Modifier);

void JigsawTriggerNPC::_bind_methods() {
	BIND_ENUM_CONSTANT(CUSTOM);

	ClassDB::bind_method(D_METHOD("get_type"), &JigsawTriggerNPC::get_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "type"), "", "get_type");
}

IMPLEMENT_CUSTOM_TRIGGER(NPC);

void JigsawTriggerVariant::_bind_methods() {
	BIND_ENUM_CONSTANT(CUSTOM);
	BIND_ENUM_CONSTANT(COSMETIC_INIT);
	BIND_ENUM_CONSTANT(CHARACTER_INIT);
	BIND_ENUM_CONSTANT(DECK_INIT);
	BIND_ENUM_CONSTANT(MATCH_START);

	ClassDB::bind_method(D_METHOD("get_type"), &JigsawTriggerVariant::get_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "type"), "", "get_type");
}

IMPLEMENT_CUSTOM_TRIGGER(Variant);

void JigsawTriggerVariantCosmeticInit::_bind_methods() {}
String JigsawTriggerVariantCosmeticInit::get_editor_name() const {
	return "Cosmetic Init";
}
String JigsawTriggerVariantCosmeticInit::get_editor_description() const {
	return "TODO"; // TODO
}
TypedArray<JigsawParameter> JigsawTriggerVariantCosmeticInit::get_arguments() const {
	return Array::make(
		JigsawParameterAmount::make(0),
		JigsawParameterAmount::make(0),
		JigsawParameterAmount::make(0),
		JigsawParameterAmount::make(0)
	);
}
PackedStringArray JigsawTriggerVariantCosmeticInit::get_argument_names() const {
	return PackedStringArray{
		"year",
		"month",
		"day",
		"seed",
	};
}
TypedArray<JigsawParameter> JigsawTriggerVariantCosmeticInit::get_results() const {
	return TypedArray<JigsawParameter>();
}
PackedStringArray JigsawTriggerVariantCosmeticInit::get_result_names() const {
	return PackedStringArray();
}

void JigsawTriggerVariantCharacterInit::_bind_methods() {}
String JigsawTriggerVariantCharacterInit::get_editor_name() const {
	return "Character Init";
}
String JigsawTriggerVariantCharacterInit::get_editor_description() const {
	return "TODO"; // TODO
}
TypedArray<JigsawParameter> JigsawTriggerVariantCharacterInit::get_arguments() const {
	return Array::make(
		JigsawParameterAmount::make(0),
		JigsawParameterString::make(""),
		JigsawParameterCharacter::make(enums::CharacterDef::NONE)
	);
}
PackedStringArray JigsawTriggerVariantCharacterInit::get_argument_names() const {
	return PackedStringArray{
		"side",
		"name",
		"character",
	};
}
TypedArray<JigsawParameter> JigsawTriggerVariantCharacterInit::get_results() const {
	return TypedArray<JigsawParameter>();
}
PackedStringArray JigsawTriggerVariantCharacterInit::get_result_names() const {
	return PackedStringArray();
}

void JigsawTriggerVariantDeckInit::_bind_methods() {}
String JigsawTriggerVariantDeckInit::get_editor_name() const {
	return "Deck Init";
}
String JigsawTriggerVariantDeckInit::get_editor_description() const {
	return "TODO"; // TODO
}
TypedArray<JigsawParameter> JigsawTriggerVariantDeckInit::get_arguments() const {
	return Array::make(
		JigsawParameterAmount::make(0),
		JigsawParameterOrderedList::make_template(Array::make(JigsawParameterCard::make(enums::CardDef::NONE)))
	);
}
PackedStringArray JigsawTriggerVariantDeckInit::get_argument_names() const {
	return PackedStringArray{
		"side",
		"deck",
	};
}
TypedArray<JigsawParameter> JigsawTriggerVariantDeckInit::get_results() const {
	return TypedArray<JigsawParameter>();
}
PackedStringArray JigsawTriggerVariantDeckInit::get_result_names() const {
	return PackedStringArray();
}

void JigsawTriggerVariantMatchStart::_bind_methods() {}
String JigsawTriggerVariantMatchStart::get_editor_name() const {
	return "Match Start";
}
String JigsawTriggerVariantMatchStart::get_editor_description() const {
	return "TODO"; // TODO
}
TypedArray<JigsawParameter> JigsawTriggerVariantMatchStart::get_arguments() const {
	return TypedArray<JigsawParameter>();
}
PackedStringArray JigsawTriggerVariantMatchStart::get_argument_names() const {
	return PackedStringArray();
}
TypedArray<JigsawParameter> JigsawTriggerVariantMatchStart::get_results() const {
	return TypedArray<JigsawParameter>();
}
PackedStringArray JigsawTriggerVariantMatchStart::get_result_names() const {
	return PackedStringArray();
}
