#include "jigsaw_command_lookup_definition_property.h"

void JigsawCommandLookupDefinitionProperty::_bind_methods() {
	BIND_ENUM_CONSTANT(CARD_NAME);
	BIND_ENUM_CONSTANT(CARD_RANK);
	BIND_ENUM_CONSTANT(CARD_COSTS);
	BIND_ENUM_CONSTANT(CARD_PORTRAIT);
	BIND_ENUM_CONSTANT(CARD_TRIBES);
	BIND_ENUM_CONSTANT(CARD_EFFECTS);

	BIND_ENUM_CONSTANT(RANK_NAME);
	BIND_ENUM_CONSTANT(RANK_COLOR);
	BIND_ENUM_CONSTANT(RANK_COLOR_ALT);
	BIND_ENUM_CONSTANT(RANK_COLOR_BASED_ON_PREFERENCES);
	BIND_ENUM_CONSTANT(RANK_FRONT);
	BIND_ENUM_CONSTANT(RANK_BACK);

	BIND_ENUM_CONSTANT(TRIBE_NAME);
	BIND_ENUM_CONSTANT(TRIBE_COLOR);
	BIND_ENUM_CONSTANT(TRIBE_IS_HIDDEN);
	BIND_ENUM_CONSTANT(TRIBE_IS_WIDE);

	BIND_ENUM_CONSTANT(STAT_NAME);
	BIND_ENUM_CONSTANT(STAT_ICON);

	BIND_PROPERTY_ENUM(JigsawCommandLookupDefinitionProperty::Property, property);
	BIND_PROPERTY_RESOURCE(JigsawParameter, definition);
	BIND_PROPERTY_RESOURCE(JigsawParameterLocalVariable, result);
}

IMPLEMENT_PROPERTY(JigsawCommandLookupDefinitionProperty, JigsawCommandLookupDefinitionProperty::Property, property);
IMPLEMENT_PROPERTY(JigsawCommandLookupDefinitionProperty, Ref<JigsawParameter>, definition);
IMPLEMENT_PROPERTY(JigsawCommandLookupDefinitionProperty, Ref<JigsawParameterLocalVariable>, result);
