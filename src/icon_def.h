#ifndef ICON_DEF_H
#define ICON_DEF_H

#include "dry.h"
#include "enums_icon_def.h"

#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/image_texture.hpp>

class IconDef : public Resource {
	GDCLASS(IconDef, Resource);

public:
	using Icon = enums::IconDef::Icon;

protected:
	static void _bind_methods();

private:
	Ref<ImageTexture> texture;

public:
	IconDef();
	~IconDef() = default;

	DECLARE_PROPERTY(PackedByteArray, file_id);
	DECLARE_PROPERTY(Ref<Image>, image);
	Ref<ImageTexture> get_texture() const;

	static Ref<IconDef> convert_legacy_portrait(Icon portrait, PackedByteArray data);
};
DECLARE_PREDEFINED_KEY(Texture2D, ICON);

#endif // ICON_DEF_H
