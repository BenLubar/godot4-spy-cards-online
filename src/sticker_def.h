#ifndef STICKER_DEF_H
#define STICKER_DEF_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/variant.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class StickerDef : public Resource {
	GDCLASS(StickerDef, Resource);

protected:
	static void _bind_methods();

private:

public:
	StickerDef();
	~StickerDef();
};

#endif // STICKER_DEF_H
