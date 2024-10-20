#ifndef JIGSAW_COMMAND_LOOKUP_DEFINITION_PROPERTY_H
#define JIGSAW_COMMAND_LOOKUP_DEFINITION_PROPERTY_H

#include "jigsaw_command.h"

class JigsawCommandLookupDefinitionProperty : public JigsawCommand {
	GDCLASS(JigsawCommandLookupDefinitionProperty, JigsawCommand);

public:
	enum Property {
		CARD_NAME = 0,
		CARD_RANK = 1,
		CARD_COSTS = 2,
		CARD_PORTRAIT = 3,
		CARD_TRIBES = 4,
		CARD_EFFECTS = 5,

		RANK_NAME = 6,
		RANK_COLOR = 7,
		RANK_COLOR_ALT = 8,
		RANK_COLOR_BASED_ON_PREFERENCES = 9,
		RANK_FRONT = 10,
		RANK_BACK = 11,

		TRIBE_NAME = 12,
		TRIBE_COLOR = 13,
		TRIBE_IS_HIDDEN = 14,
		TRIBE_IS_WIDE = 15,

		STAT_NAME = 16,
		STAT_ICON = 17,
	};

protected:
	static void _bind_methods();

public:
	JigsawCommandLookupDefinitionProperty() = default;
	~JigsawCommandLookupDefinitionProperty() = default;

	DECLARE_PROPERTY(Property, property, = Property::CARD_NAME);
	DECLARE_PROPERTY(Ref<JigsawParameter>, definition);
	DECLARE_PROPERTY(Ref<JigsawParameterLocalVariable>, result);

	Type get_type() const override { return LOOKUP_DEFINITION_PROPERTY; }
};
DECLARE_ENUM(JigsawCommandLookupDefinitionProperty::Property);

#endif // JIGSAW_COMMAND_LOOKUP_DEFINITION_PROPERTY_H
