#ifndef STICKER_DEF_H
#define STICKER_DEF_H

#include "dry.h"

class StickerDef;

#include "icon_def.h"
#include "jigsaw_command_list.h"

class StickerDef : public Resource {
	GDCLASS(StickerDef, Resource);

protected:
	static void _bind_methods();

public:
	StickerDef() = default;
	~StickerDef() = default;

	DECLARE_PROPERTY(IconDef::Icon, icon, = IconDef::Icon::NONE);
	DECLARE_PROPERTY(Rect2, position, = Rect2(0, 0, 30, 10));
	DECLARE_PROPERTY(Ref<JigsawCommandList>, should_show);
};

#endif // STICKER_DEF_H
