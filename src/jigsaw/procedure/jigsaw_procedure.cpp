#include "jigsaw_procedure.h"

#include "jigsaw_parameter_amount.h"
#include "jigsaw_parameter_boolean.h"
#include "jigsaw_parameter_card_instance.h"
#include "jigsaw_parameter_effect_instance.h"
#include "jigsaw_parameter_float.h"
#include "jigsaw_parameter_formatted_text.h"
#include "jigsaw_parameter_icon.h"
#include "jigsaw_parameter_ordered_list.h"

void JigsawProcedure::_bind_methods() {
	BIND_PROPERTY_RESOURCE(JigsawCommandList, commands);

	ClassDB::bind_method(D_METHOD("get_editor_name"), &JigsawProcedure::get_editor_name);
	ClassDB::bind_method(D_METHOD("get_editor_description"), &JigsawProcedure::get_editor_description);
	ClassDB::bind_method(D_METHOD("get_arguments"), &JigsawProcedure::get_arguments);
	ClassDB::bind_method(D_METHOD("get_argument_names"), &JigsawProcedure::get_argument_names);
	ClassDB::bind_method(D_METHOD("get_results"), &JigsawProcedure::get_results);
	ClassDB::bind_method(D_METHOD("get_result_names"), &JigsawProcedure::get_result_names);
}

IMPLEMENT_PROPERTY(JigsawProcedure, Ref<JigsawCommandList>, commands);

void JigsawProcedureEffectCurve::_bind_methods() {}
String JigsawProcedureEffectCurve::get_editor_name() const {
	return "Effect Curve";
}
String JigsawProcedureEffectCurve::get_editor_description() const {
	return "Estimate the value of an effect. This is normally only shown in the card editor for reference, but you can also access this value in custom effects.";
}
TypedArray<JigsawParameter> JigsawProcedureEffectCurve::get_arguments() const {
	return Array::make(
		JigsawParameterEffectInstance::make(Ref<EffectInstance>())
	);
}
PackedStringArray JigsawProcedureEffectCurve::get_argument_names() const {
	return PackedStringArray{"inst"};
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
String JigsawProcedureEffectDescribe::get_editor_name() const {
	return "Describe Effect";
}
String JigsawProcedureEffectDescribe::get_editor_description() const {
	return "TODO"; // TODO
}
TypedArray<JigsawParameter> JigsawProcedureEffectDescribe::get_arguments() const {
	return Array::make(
		JigsawParameterCardInstance::make(Ref<CardInstance>()),
		JigsawParameterEffectInstance::make(Ref<EffectInstance>())
	);
}
PackedStringArray JigsawProcedureEffectDescribe::get_argument_names() const {
	return PackedStringArray{"card", "inst"};
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
String JigsawProcedureEffectSimpleDescribe::get_editor_name() const {
	return "TODO"; // TODO
}
String JigsawProcedureEffectSimpleDescribe::get_editor_description() const {
	return "TODO"; // TODO
}
TypedArray<JigsawParameter> JigsawProcedureEffectSimpleDescribe::get_arguments() const {
	return Array::make(
		JigsawParameterCardInstance::make(Ref<CardInstance>()),
		JigsawParameterEffectInstance::make(Ref<EffectInstance>())
	);
}
PackedStringArray JigsawProcedureEffectSimpleDescribe::get_argument_names() const {
	return PackedStringArray{"card", "inst"};
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
String JigsawProcedureEffectExtendedDescribe::get_editor_name() const {
	return "TODO"; // TODO
}
String JigsawProcedureEffectExtendedDescribe::get_editor_description() const {
	return "TODO"; // TODO
}
TypedArray<JigsawParameter> JigsawProcedureEffectExtendedDescribe::get_arguments() const {
	return Array::make(
		JigsawParameterCardInstance::make(Ref<CardInstance>()),
		JigsawParameterEffectInstance::make(Ref<EffectInstance>())
	);
}
PackedStringArray JigsawProcedureEffectExtendedDescribe::get_argument_names() const {
	return PackedStringArray{"card", "inst"};
}
TypedArray<JigsawParameter> JigsawProcedureEffectExtendedDescribe::get_results() const {
	return Array::make(
		JigsawParameterOrderedList::make_template(Array::make(
			JigsawParameterFormattedText::make(TypedArray<FormattedText>()),
			JigsawParameterCardInstance::make(Ref<CardInstance>())
		))
	);
}
PackedStringArray JigsawProcedureEffectExtendedDescribe::get_result_names() const {
	return PackedStringArray{"tooltips"};
}

void JigsawProcedureModifierDescribe::_bind_methods() {}
String JigsawProcedureModifierDescribe::get_editor_name() const {
	return "TODO"; // TODO
}
String JigsawProcedureModifierDescribe::get_editor_description() const {
	return "TODO"; // TODO
}
TypedArray<JigsawParameter> JigsawProcedureModifierDescribe::get_arguments() const {
	return Array::make(
		JigsawParameterCardInstance::make(Ref<CardInstance>()),
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
String JigsawProcedureModifierSimpleDescribe::get_editor_name() const {
	return "TODO"; // TODO
}
String JigsawProcedureModifierSimpleDescribe::get_editor_description() const {
	return "TODO"; // TODO
}
TypedArray<JigsawParameter> JigsawProcedureModifierSimpleDescribe::get_arguments() const {
	return Array::make(
		JigsawParameterCardInstance::make(Ref<CardInstance>()),
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
String JigsawProcedureModifierExtendedDescribe::get_editor_name() const {
	return "TODO"; // TODO
}
String JigsawProcedureModifierExtendedDescribe::get_editor_description() const {
	return "TODO"; // TODO
}
TypedArray<JigsawParameter> JigsawProcedureModifierExtendedDescribe::get_arguments() const {
	return Array::make(
		JigsawParameterCardInstance::make(Ref<CardInstance>()),
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
			JigsawParameterCardInstance::make(Ref<CardInstance>())
		))
	);
}
PackedStringArray JigsawProcedureModifierExtendedDescribe::get_result_names() const {
	return PackedStringArray{"tooltips"};
}

void JigsawProcedureStickerShouldShow::_bind_methods() {}
String JigsawProcedureStickerShouldShow::get_editor_name() const {
	return "TODO"; // TODO
}
String JigsawProcedureStickerShouldShow::get_editor_description() const {
	return "TODO"; // TODO
}
TypedArray<JigsawParameter> JigsawProcedureStickerShouldShow::get_arguments() const {
	return Array::make(
		JigsawParameterCardInstance::make(Ref<CardInstance>())
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
String JigsawProcedureStatFormatCost::get_editor_name() const {
	return "TODO"; // TODO
}
String JigsawProcedureStatFormatCost::get_editor_description() const {
	return "TODO"; // TODO
}
TypedArray<JigsawParameter> JigsawProcedureStatFormatCost::get_arguments() const {
	return Array::make(
		JigsawParameterCardInstance::make(Ref<CardInstance>()),
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
