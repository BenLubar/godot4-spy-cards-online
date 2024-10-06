#include "sticker_def.h"

void StickerDef::_bind_methods() {
	BIND_PROPERTY(Variant::INT, icon);
	BIND_PROPERTY(Variant::RECT2, position);
	BIND_PROPERTY(Variant::OBJECT, should_show, PROPERTY_HINT_RESOURCE_TYPE, "JigsawCommandList");
}

IMPLEMENT_PROPERTY(StickerDef, IconDef::Icon, icon);
IMPLEMENT_PROPERTY(StickerDef, Rect2, position);
IMPLEMENT_PROPERTY(StickerDef, Ref<JigsawCommandList>, should_show);
