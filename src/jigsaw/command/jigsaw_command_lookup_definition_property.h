#ifndef JIGSAW_COMMAND_LOOKUP_DEFINITION_PROPERTY_H
#define JIGSAW_COMMAND_LOOKUP_DEFINITION_PROPERTY_H

#include "jigsaw/command/jigsaw_command.h"

class JigsawCommandLookupDefinitionProperty : public JigsawCommand {
	GDCLASS(JigsawCommandLookupDefinitionProperty, JigsawCommand);

public:
	enum Property {
		CARD_DEF = 0, // card_instance -> card
		CARD_NAME = 1, // card/card_instance -> formatted_text
		CARD_RANK = 2, // card/card_instance -> rank
		CARD_COSTS = 3, // card/card_instance -> ordered_list[stat_value]
		CARD_PORTRAIT = 4, // card/card_instance -> icon
		CARD_TRIBES = 5, // card/card_instance -> ordered_list[tribe]
		CARD_EFFECTS = 6, // card/card_instance -> ordered_list[effect_instance]

		RANK_NAME = 7, // rank/card/card_instance -> string
		RANK_COLOR = 8, // rank/card/card_instance -> color
		RANK_COLOR_ALT = 9, // rank/card/card_instance -> color
		RANK_COLOR_BASED_ON_PREFERENCES = 10, // rank/card/card_instance -> color
		RANK_FRONT = 11, // rank/card/card_instance -> icon
		RANK_BACK = 12, // rank/card/card_instance -> icon

		TRIBE_NAME = 13, // tribe -> string
		TRIBE_COLOR = 14, // tribe -> color
		TRIBE_IS_HIDDEN = 15, // tribe -> boolean
		TRIBE_IS_WIDE = 16, // tribe -> boolean

		STAT_DEF = 17, // stat_value -> stat
		STAT_NAME = 18, // stat/stat_value -> string
		STAT_ICON = 19, // stat/stat_value -> icon
		STAT_AMOUNT = 20, // stat_value -> amount
	};

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(Property, property, = Property::CARD_NAME);
	DECLARE_PROPERTY(Ref<JigsawParameter>, definition);
	DECLARE_PROPERTY(Ref<JigsawParameterLocalVariable>, value);

	Type get_type() const override { return LOOKUP_DEFINITION_PROPERTY; }
	bool allowed_in_mode(enums::JigsawProcedure::Mode mode, bool any_config) const override;
	JigsawExecutionState evaluate(const Ref<JigsawContext> &context, Ref<JigsawError> &err, bool first) const override;

	int64_t get_num_configs() const override;
	String get_config_name(int64_t i) const override;
	String get_config_desc(int64_t i) const override;
	int64_t get_config_value(int64_t i) const override;
	void set_config_value(int64_t i, int64_t value) override;
	PackedStringArray get_config_options(int64_t i) const override;

	int64_t get_num_arguments() const override;
	Ref<JigsawParameter> get_argument(int64_t i) const override;
	TypedArray<JigsawParameter> get_argument_template(int64_t i, const Ref<JigsawContext> &context) const override;
	void set_argument(int64_t i, const Ref<JigsawParameter> &arg) override;
	String get_argument_name(int64_t i) const override;

	int64_t get_num_results() const override;
	Ref<JigsawParameterLocalVariable> get_result(int64_t i) const override;
	TypedArray<JigsawParameter> get_result_template(int64_t i, const Ref<JigsawContext> &context) const override;
	void set_result(int64_t i, const Ref<JigsawParameterLocalVariable> &result) override;
	String get_result_name(int64_t i) const override;
};
DECLARE_ENUM(JigsawCommandLookupDefinitionProperty::Property);

#endif // JIGSAW_COMMAND_LOOKUP_DEFINITION_PROPERTY_H
