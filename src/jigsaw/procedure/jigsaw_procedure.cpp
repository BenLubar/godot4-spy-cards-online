#include "jigsaw_procedure.h"

#include "jigsaw/parameter/jigsaw_parameter_amount.h"
#include "jigsaw/parameter/jigsaw_parameter_boolean.h"
#include "jigsaw/parameter/jigsaw_parameter_card.h"
#include "jigsaw/parameter/jigsaw_parameter_card_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_character.h"
#include "jigsaw/parameter/jigsaw_parameter_effect_instance.h"
#include "jigsaw/parameter/jigsaw_parameter_float.h"
#include "jigsaw/parameter/jigsaw_parameter_formatted_text.h"
#include "jigsaw/parameter/jigsaw_parameter_icon.h"
#include "jigsaw/parameter/jigsaw_parameter_ordered_list.h"

void JigsawProcedure::_bind_methods() {
	using namespace enums::JigsawProcedure;

	BIND_ENUM_CONSTANT(FUNCTIONAL);
	BIND_ENUM_CONSTANT(INIT);
	BIND_ENUM_CONSTANT(MAIN);
	BIND_ENUM_CONSTANT(SELECT);
	BIND_ENUM_CONSTANT(CHOICE_PREVIEW);
	BIND_ENUM_CONSTANT(REALTIME_LOGIC);
	BIND_ENUM_CONSTANT(REALTIME_VISUAL);

	BIND_PROPERTY_RESOURCE(JigsawCommandList, commands);

	ClassDB::bind_method(D_METHOD("get_mode"), &JigsawProcedure::get_mode);
	ClassDB::bind_method(D_METHOD("get_editor_name"), &JigsawProcedure::get_editor_name);
	ClassDB::bind_method(D_METHOD("get_editor_description"), &JigsawProcedure::get_editor_description);
	ClassDB::bind_method(D_METHOD("get_arguments"), &JigsawProcedure::get_arguments);
	ClassDB::bind_method(D_METHOD("get_argument_names"), &JigsawProcedure::get_argument_names);
	ClassDB::bind_method(D_METHOD("get_results"), &JigsawProcedure::get_results);
	ClassDB::bind_method(D_METHOD("get_result_names"), &JigsawProcedure::get_result_names);
}

IMPLEMENT_PROPERTY(JigsawProcedure, Ref<JigsawCommandList>, commands);

void JigsawProcedureEffectCurve::_bind_methods() {}
enums::JigsawProcedure::Mode JigsawProcedureEffectCurve::get_mode() const {
	return Mode::FUNCTIONAL;
}
String JigsawProcedureEffectCurve::get_editor_name() const {
	return "Effect Curve";
}
String JigsawProcedureEffectCurve::get_editor_description() const {
	return "Estimate the value of an effect. This is normally only shown in the card editor for reference, but you can also access this value in custom effects.";
}
TypedArray<JigsawParameter> JigsawProcedureEffectCurve::get_arguments() const {
	return Array::make(
		JigsawParameterCard::make(enums::CardDef::NONE),
		JigsawParameterEffectInstance::make(Ref<EffectInstance>())
	);
}
PackedStringArray JigsawProcedureEffectCurve::get_argument_names() const {
	return PackedStringArray{"card", "inst"};
}
TypedArray<JigsawParameter> JigsawProcedureEffectCurve::get_results() const {
	return Array::make(
		JigsawParameterFloat::make(0.0)
	);
}
PackedStringArray JigsawProcedureEffectCurve::get_result_names() const {
	return PackedStringArray{"curve"};
}

void JigsawProcedureEffectDescribe::_bind_methods() {}
enums::JigsawProcedure::Mode JigsawProcedureEffectDescribe::get_mode() const {
	return Mode::FUNCTIONAL;
}
String JigsawProcedureEffectDescribe::get_editor_name() const {
	return "Describe Effect";
}
String JigsawProcedureEffectDescribe::get_editor_description() const {
	return "Generate text for a card effect to display in the description area of a card containing the effect.";
}
TypedArray<JigsawParameter> JigsawProcedureEffectDescribe::get_arguments() const {
	return Array::make(
		JigsawParameterCardInstance::make(-1),
		JigsawParameterEffectInstance::make(Ref<EffectInstance>()),
		JigsawParameterOrderedList::make_template(Array::make(
			JigsawParameterEffectInstance::make(Ref<EffectInstance>())
		))
	);
}
PackedStringArray JigsawProcedureEffectDescribe::get_argument_names() const {
	return PackedStringArray{"card", "effect", "parents"};
}
TypedArray<JigsawParameter> JigsawProcedureEffectDescribe::get_results() const {
	return Array::make(
		JigsawParameterFormattedText::make(TypedArray<FormattedText>())
	);
}
PackedStringArray JigsawProcedureEffectDescribe::get_result_names() const {
	return PackedStringArray{"text"};
}

void JigsawProcedureEffectSimpleDescribe::_bind_methods() {}
enums::JigsawProcedure::Mode JigsawProcedureEffectSimpleDescribe::get_mode() const {
	return Mode::FUNCTIONAL;
}
String JigsawProcedureEffectSimpleDescribe::get_editor_name() const {
	return "Describe Effect (Simple)";
}
String JigsawProcedureEffectSimpleDescribe::get_editor_description() const {
	return "Generate an icon and text to show in the card's description area. Only used if every top-level effect on the card generates an icon and/or text for the simple description.";
}
TypedArray<JigsawParameter> JigsawProcedureEffectSimpleDescribe::get_arguments() const {
	return Array::make(
		JigsawParameterCardInstance::make(-1),
		JigsawParameterEffectInstance::make(Ref<EffectInstance>()),
		JigsawParameterOrderedList::make_template(Array::make(
			JigsawParameterEffectInstance::make(Ref<EffectInstance>())
		))
	);
}
PackedStringArray JigsawProcedureEffectSimpleDescribe::get_argument_names() const {
	return PackedStringArray{"card", "effect", "parents"};
}
TypedArray<JigsawParameter> JigsawProcedureEffectSimpleDescribe::get_results() const {
	return Array::make(
		JigsawParameterFormattedText::make(TypedArray<FormattedText>()),
		JigsawParameterIcon::make(enums::IconDef::Icon::NONE)
	);
}
PackedStringArray JigsawProcedureEffectSimpleDescribe::get_result_names() const {
	return PackedStringArray{"text", "icon"};
}

void JigsawProcedureEffectExtendedDescribe::_bind_methods() {}
enums::JigsawProcedure::Mode JigsawProcedureEffectExtendedDescribe::get_mode() const {
	return Mode::FUNCTIONAL;
}
String JigsawProcedureEffectExtendedDescribe::get_editor_name() const {
	return "Describe Effect (Extended)";
}
String JigsawProcedureEffectExtendedDescribe::get_editor_description() const {
	return "Generate a list of facts to show next to a card when hovered. Useful for explaining keywords or showing other cards for reference.";
}
TypedArray<JigsawParameter> JigsawProcedureEffectExtendedDescribe::get_arguments() const {
	return Array::make(
		JigsawParameterCardInstance::make(-1),
		JigsawParameterEffectInstance::make(Ref<EffectInstance>()),
		JigsawParameterOrderedList::make_template(Array::make(
			JigsawParameterEffectInstance::make(Ref<EffectInstance>())
		))
	);
}
PackedStringArray JigsawProcedureEffectExtendedDescribe::get_argument_names() const {
	return PackedStringArray{"card", "effect", "parents"};
}
TypedArray<JigsawParameter> JigsawProcedureEffectExtendedDescribe::get_results() const {
	return Array::make(
		JigsawParameterOrderedList::make_template(Array::make(
			JigsawParameterFormattedText::make(TypedArray<FormattedText>()),
			JigsawParameterCardInstance::make(-1)
		))
	);
}
PackedStringArray JigsawProcedureEffectExtendedDescribe::get_result_names() const {
	return PackedStringArray{"tooltips"};
}

void JigsawProcedureModifierDescribe::_bind_methods() {}
enums::JigsawProcedure::Mode JigsawProcedureModifierDescribe::get_mode() const {
	return Mode::FUNCTIONAL;
}
String JigsawProcedureModifierDescribe::get_editor_name() const {
	return "Describe Modifier";
}
String JigsawProcedureModifierDescribe::get_editor_description() const {
	return "Generate text to name a modifier. Can be queried by game mode procedures, and is shown in the log.";
}
TypedArray<JigsawParameter> JigsawProcedureModifierDescribe::get_arguments() const {
	return Array::make(
		JigsawParameterCardInstance::make(-1),
		JigsawParameterAmount::make(0)
	);
}
PackedStringArray JigsawProcedureModifierDescribe::get_argument_names() const {
	return PackedStringArray{"card", "count"};
}
TypedArray<JigsawParameter> JigsawProcedureModifierDescribe::get_results() const {
	return Array::make(
		JigsawParameterFormattedText::make(TypedArray<FormattedText>())
	);
}
PackedStringArray JigsawProcedureModifierDescribe::get_result_names() const {
	return PackedStringArray{"text"};
}

void JigsawProcedureModifierSimpleDescribe::_bind_methods() {}
enums::JigsawProcedure::Mode JigsawProcedureModifierSimpleDescribe::get_mode() const {
	return Mode::FUNCTIONAL;
}
String JigsawProcedureModifierSimpleDescribe::get_editor_name() const {
	return "Describe Modifier (Simple)";
}
String JigsawProcedureModifierSimpleDescribe::get_editor_description() const {
	return "Generate text and an icon to display in the card's modifiers area.";
}
TypedArray<JigsawParameter> JigsawProcedureModifierSimpleDescribe::get_arguments() const {
	return Array::make(
		JigsawParameterCardInstance::make(-1),
		JigsawParameterAmount::make(0)
	);
}
PackedStringArray JigsawProcedureModifierSimpleDescribe::get_argument_names() const {
	return PackedStringArray{"card", "count"};
}
TypedArray<JigsawParameter> JigsawProcedureModifierSimpleDescribe::get_results() const {
	return Array::make(
		JigsawParameterFormattedText::make(TypedArray<FormattedText>()),
		JigsawParameterIcon::make(enums::IconDef::Icon::NONE)
	);
}
PackedStringArray JigsawProcedureModifierSimpleDescribe::get_result_names() const {
	return PackedStringArray{"text", "icon"};
}

void JigsawProcedureModifierExtendedDescribe::_bind_methods() {}
enums::JigsawProcedure::Mode JigsawProcedureModifierExtendedDescribe::get_mode() const {
	return Mode::FUNCTIONAL;
}
String JigsawProcedureModifierExtendedDescribe::get_editor_name() const {
	return "Describe Modifier (Extended)";
}
String JigsawProcedureModifierExtendedDescribe::get_editor_description() const {
	return "Generate a list of facts to show next to a card that has this modifier when hovered.";
}
TypedArray<JigsawParameter> JigsawProcedureModifierExtendedDescribe::get_arguments() const {
	return Array::make(
		JigsawParameterCardInstance::make(-1),
		JigsawParameterAmount::make(0)
	);
}
PackedStringArray JigsawProcedureModifierExtendedDescribe::get_argument_names() const {
	return PackedStringArray{"card", "count"};
}
TypedArray<JigsawParameter> JigsawProcedureModifierExtendedDescribe::get_results() const {
	return Array::make(
		JigsawParameterOrderedList::make_template(Array::make(
			JigsawParameterFormattedText::make(TypedArray<FormattedText>()),
			JigsawParameterCardInstance::make(-1)
		))
	);
}
PackedStringArray JigsawProcedureModifierExtendedDescribe::get_result_names() const {
	return PackedStringArray{"tooltips"};
}

void JigsawProcedureNPCBuildDeck::_bind_methods() {}
enums::JigsawProcedure::Mode JigsawProcedureNPCBuildDeck::get_mode() const {
	return Mode::INIT;
}
String JigsawProcedureNPCBuildDeck::get_editor_name() const {
	return "NPC: Generate Deck";
}
String JigsawProcedureNPCBuildDeck::get_editor_description() const {
	return "Generate a list of cards to put in an NPC's deck. The game mode does NOT automatically check that these cards make a valid deck.";
}
TypedArray<JigsawParameter> JigsawProcedureNPCBuildDeck::get_arguments() const {
	return Array::make(
		JigsawParameterAmount::make(0)
	);
}
PackedStringArray JigsawProcedureNPCBuildDeck::get_argument_names() const {
	return PackedStringArray{"side"};
}
TypedArray<JigsawParameter> JigsawProcedureNPCBuildDeck::get_results() const {
	return Array::make(
		JigsawParameterOrderedList::make_template(Array::make(
			JigsawParameterCard::make(enums::CardDef::NONE)
		))
	);
}
PackedStringArray JigsawProcedureNPCBuildDeck::get_result_names() const {
	return PackedStringArray{"cards"};
}

void JigsawProcedureNPCDecideChoice::_bind_methods() {
	BIND_PROPERTY_RESOURCE(JigsawParameterChoice, choice_type);
}
IMPLEMENT_PROPERTY(JigsawProcedureNPCDecideChoice, Ref<JigsawParameterChoice>, choice_type);
enums::JigsawProcedure::Mode JigsawProcedureNPCDecideChoice::get_mode() const {
	return Mode::MAIN;
}
String JigsawProcedureNPCDecideChoice::get_editor_name() const {
	return "NPC: Decide Choice";
}
String JigsawProcedureNPCDecideChoice::get_editor_description() const {
	return "Select cards for an NPC to play on their turn.";
}
TypedArray<JigsawParameter> JigsawProcedureNPCDecideChoice::get_arguments() const {
	return Array::make(
		JigsawParameterAmount::make(0)
	);
}
PackedStringArray JigsawProcedureNPCDecideChoice::get_argument_names() const {
	return PackedStringArray{"side"};
}
TypedArray<JigsawParameter> JigsawProcedureNPCDecideChoice::get_results() const {
	return Array::make(
		JigsawParameterOrderedList::make_template(Array::make(
			JigsawParameterAmount::make(0)
		))
	);
}
PackedStringArray JigsawProcedureNPCDecideChoice::get_result_names() const {
	return PackedStringArray{"choices"};
}

void JigsawProcedureNPCDecideRealtime::_bind_methods() {
	BIND_PROPERTY_RESOURCE(JigsawParameterRealtime, realtime_type);
}
IMPLEMENT_PROPERTY(JigsawProcedureNPCDecideRealtime, Ref<JigsawParameterChoice>, realtime_type);
enums::JigsawProcedure::Mode JigsawProcedureNPCDecideRealtime::get_mode() const {
	return Mode::FUNCTIONAL;
}
String JigsawProcedureNPCDecideRealtime::get_editor_name() const {
	return "NPC: Decide Realtime";
}
String JigsawProcedureNPCDecideRealtime::get_editor_description() const {
	return "Control the buttons an NPC holds during a realtime segment.";
}
TypedArray<JigsawParameter> JigsawProcedureNPCDecideRealtime::get_arguments() const {
	return Array::make(
		JigsawParameterAmount::make(0),
		JigsawParameterAmount::make(0)
	);
}
PackedStringArray JigsawProcedureNPCDecideRealtime::get_argument_names() const {
	return PackedStringArray{"side", "frame_count"};
}
TypedArray<JigsawParameter> JigsawProcedureNPCDecideRealtime::get_results() const {
	return Array::make(
		JigsawParameterBoolean::make(false),
		JigsawParameterBoolean::make(false),
		JigsawParameterBoolean::make(false),
		JigsawParameterBoolean::make(false),
		JigsawParameterBoolean::make(false),
		JigsawParameterBoolean::make(false),
		JigsawParameterBoolean::make(false),
		JigsawParameterBoolean::make(false)
	);
}
PackedStringArray JigsawProcedureNPCDecideRealtime::get_result_names() const {
	return PackedStringArray{"up", "down", "left", "right", "confirm", "cancel", "switch", "toggle"};
}

void JigsawProcedureStickerShouldShow::_bind_methods() {}
enums::JigsawProcedure::Mode JigsawProcedureStickerShouldShow::get_mode() const {
	return Mode::FUNCTIONAL;
}
String JigsawProcedureStickerShouldShow::get_editor_name() const {
	return "Should Show Sticker";
}
String JigsawProcedureStickerShouldShow::get_editor_description() const {
	return "Decide whether a sticker is visible on a given card.";
}
TypedArray<JigsawParameter> JigsawProcedureStickerShouldShow::get_arguments() const {
	return Array::make(
		JigsawParameterCardInstance::make(-1)
	);
}
PackedStringArray JigsawProcedureStickerShouldShow::get_argument_names() const {
	return PackedStringArray{"card"};
}
TypedArray<JigsawParameter> JigsawProcedureStickerShouldShow::get_results() const {
	return Array::make(
		JigsawParameterBoolean::make(false)
	);
}
PackedStringArray JigsawProcedureStickerShouldShow::get_result_names() const {
	return PackedStringArray{"show"};
}

void JigsawProcedureStatFormatCost::_bind_methods() {}
enums::JigsawProcedure::Mode JigsawProcedureStatFormatCost::get_mode() const {
	return Mode::FUNCTIONAL;
}
String JigsawProcedureStatFormatCost::get_editor_name() const {
	return "Format Cost";
}
String JigsawProcedureStatFormatCost::get_editor_description() const {
	return "Choose an icon and text to display in a card's costs area.";
}
TypedArray<JigsawParameter> JigsawProcedureStatFormatCost::get_arguments() const {
	return Array::make(
		JigsawParameterCardInstance::make(-1),
		JigsawParameterAmount::make(0)
	);
}
PackedStringArray JigsawProcedureStatFormatCost::get_argument_names() const {
	return PackedStringArray{"card", "amount"};
}
TypedArray<JigsawParameter> JigsawProcedureStatFormatCost::get_results() const {
	return Array::make(
		JigsawParameterFormattedText::make(TypedArray<FormattedText>()),
		JigsawParameterIcon::make(enums::IconDef::Icon::NONE)
	);
}
PackedStringArray JigsawProcedureStatFormatCost::get_result_names() const {
	return PackedStringArray{"text", "icon"};
}

void JigsawProcedureVariantSelectCharacter::_bind_methods() {}
enums::JigsawProcedure::Mode JigsawProcedureVariantSelectCharacter::get_mode() const {
	return Mode::SELECT;
}
String JigsawProcedureVariantSelectCharacter::get_editor_name() const {
	return "Character Select";
}
String JigsawProcedureVariantSelectCharacter::get_editor_description() const {
	return "TODO";
}
TypedArray<JigsawParameter> JigsawProcedureVariantSelectCharacter::get_arguments() const {
	return TypedArray<JigsawParameter>();
}
PackedStringArray JigsawProcedureVariantSelectCharacter::get_argument_names() const {
	return PackedStringArray{};
}
TypedArray<JigsawParameter> JigsawProcedureVariantSelectCharacter::get_results() const {
	return Array::make(
		JigsawParameterCharacter::make(enums::CharacterDef::NONE)
	);
}
PackedStringArray JigsawProcedureVariantSelectCharacter::get_result_names() const {
	return PackedStringArray{"character"};
}

void JigsawProcedureVariantBuildDeck::_bind_methods() {}
enums::JigsawProcedure::Mode JigsawProcedureVariantBuildDeck::get_mode() const {
	return Mode::SELECT;
}
String JigsawProcedureVariantBuildDeck::get_editor_name() const {
	return "Build Deck";
}
String JigsawProcedureVariantBuildDeck::get_editor_description() const {
	return "TODO";
}
TypedArray<JigsawParameter> JigsawProcedureVariantBuildDeck::get_arguments() const {
	return Array::make(
		JigsawParameterOrderedList::make_template(Array::make(
			JigsawParameterCard::make(enums::CardDef::NONE)
		)),
		JigsawParameterOrderedList::make_template(Array::make(
			JigsawParameterCard::make(enums::CardDef::NONE)
		)),
		JigsawParameterAmount::make(0)
	);
}
PackedStringArray JigsawProcedureVariantBuildDeck::get_argument_names() const {
	return PackedStringArray{"current_deck", "next_card_options", "next_card_index"};
}
TypedArray<JigsawParameter> JigsawProcedureVariantBuildDeck::get_results() const {
	return Array::make(
		JigsawParameterCard::make(enums::CardDef::NONE)
	);
}
PackedStringArray JigsawProcedureVariantBuildDeck::get_result_names() const {
	return PackedStringArray{"selected_card"};
}

void JigsawProcedureVariantValidateDeck::_bind_methods() {}
enums::JigsawProcedure::Mode JigsawProcedureVariantValidateDeck::get_mode() const {
	return Mode::FUNCTIONAL;
}
String JigsawProcedureVariantValidateDeck::get_editor_name() const {
	return "Validate Deck";
}
String JigsawProcedureVariantValidateDeck::get_editor_description() const {
	return "TODO";
}
TypedArray<JigsawParameter> JigsawProcedureVariantValidateDeck::get_arguments() const {
	return Array::make(
		JigsawParameterOrderedList::make_template(Array::make(
			JigsawParameterCard::make(enums::CardDef::NONE)
		))
	);
}
PackedStringArray JigsawProcedureVariantValidateDeck::get_argument_names() const {
	return PackedStringArray{"&current_deck"};
}
TypedArray<JigsawParameter> JigsawProcedureVariantValidateDeck::get_results() const {
	return Array::make(
		JigsawParameterOrderedList::make_template(Array::make(
			JigsawParameterCard::make(enums::CardDef::NONE)
		)),
		JigsawParameterAmount::make(-1)
	);
}
PackedStringArray JigsawProcedureVariantValidateDeck::get_result_names() const {
	return PackedStringArray{"next_card_options", "next_card_index"};
}

void JigsawProcedureVariantMain::_bind_methods() {}
enums::JigsawProcedure::Mode JigsawProcedureVariantMain::get_mode() const {
	return Mode::MAIN;
}
String JigsawProcedureVariantMain::get_editor_name() const {
	return "Main Loop";
}
String JigsawProcedureVariantMain::get_editor_description() const {
	return "The main gameplay loop, run after initialization finishes.";
}
TypedArray<JigsawParameter> JigsawProcedureVariantMain::get_arguments() const {
	return TypedArray<JigsawParameter>();
}
PackedStringArray JigsawProcedureVariantMain::get_argument_names() const {
	return PackedStringArray{};
}
TypedArray<JigsawParameter> JigsawProcedureVariantMain::get_results() const {
	return Array::make(
		JigsawParameterOrderedList::make_template(Array::make(
			JigsawParameterAmount::make(0)
		))
	);
}
PackedStringArray JigsawProcedureVariantMain::get_result_names() const {
	return PackedStringArray{"winners"};
}
