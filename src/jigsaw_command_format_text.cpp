#include "jigsaw_command_format_text.h"

void JigsawCommandFormatText::_bind_methods() {
	BIND_PROPERTY_ENUM(FormattedText::Command, command);

	BIND_PROPERTY_RESOURCE(JigsawParameter, before);
	BIND_PROPERTY_RESOURCE(JigsawParameter, text);
	BIND_PROPERTY_RESOURCE(JigsawParameter, after);

	BIND_PROPERTY(Variant::INT, table_columns);
	BIND_PROPERTY_RESOURCE(JigsawParameter, color);
	BIND_PROPERTY_RESOURCE(JigsawParameter, icon);
	BIND_PROPERTY_RESOURCE(JigsawParameter, font_size);

	BIND_PROPERTY_RESOURCE(JigsawParameterLocalVariable, result);
}

IMPLEMENT_PROPERTY(JigsawCommandFormatText, FormattedText::Command, command);

IMPLEMENT_PROPERTY(JigsawCommandFormatText, Ref<JigsawParameter>, before);
IMPLEMENT_PROPERTY(JigsawCommandFormatText, Ref<JigsawParameter>, text);
IMPLEMENT_PROPERTY(JigsawCommandFormatText, Ref<JigsawParameter>, after);

IMPLEMENT_PROPERTY(JigsawCommandFormatText, int64_t, table_columns);
IMPLEMENT_PROPERTY(JigsawCommandFormatText, Ref<JigsawParameter>, color);
IMPLEMENT_PROPERTY(JigsawCommandFormatText, Ref<JigsawParameter>, icon);
IMPLEMENT_PROPERTY(JigsawCommandFormatText, Ref<JigsawParameter>, font_size);

IMPLEMENT_PROPERTY(JigsawCommandFormatText, Ref<JigsawParameterLocalVariable>, result);
