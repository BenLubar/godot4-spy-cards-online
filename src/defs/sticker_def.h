#ifndef STICKER_DEF_H
#define STICKER_DEF_H

#include "dry.h"

#include "enums/enums_icon_def.h"

class StickerDef;

#include "jigsaw/procedure/jigsaw_procedure.h"

class StickerDef : public Resource {
	GDCLASS(StickerDef, Resource);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(enums::IconDef::Icon, icon, = enums::IconDef::Icon::NONE);
	DECLARE_PROPERTY(Rect2, position, = Rect2(0, 0, 30, 10));
	DECLARE_PROPERTY(Ref<JigsawProcedureStickerShouldShow>, should_show);

	DEFAULT_TO_STRING();
};

#endif // STICKER_DEF_H
