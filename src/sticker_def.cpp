#include "sticker_def.h"

void StickerDef::_bind_methods() {
	BIND_PROPERTY(Variant::INT, icon);
	BIND_PROPERTY(Variant::RECT2, position);
	BIND_PROPERTY_RESOURCE(JigsawCommandList, should_show);
}

IMPLEMENT_PROPERTY(StickerDef, IconDef::Icon, icon);
IMPLEMENT_PROPERTY(StickerDef, Rect2, position);
IMPLEMENT_PROPERTY(StickerDef, Ref<JigsawCommandList>, should_show);
