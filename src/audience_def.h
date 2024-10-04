#ifndef AUDIENCE_DEF_H
#define AUDIENCE_DEF_H

#include "dry.h"
#include "icon_def.h"

using namespace godot;

class AudienceDef : public Resource {
	GDCLASS(AudienceDef, Resource);

protected:
	static void _bind_methods();

public:
	AudienceDef();
	~AudienceDef();

	DECLARE_PROPERTY(IconDef::Icon, front, = IconDef::Icon::NONE);
	DECLARE_PROPERTY(IconDef::Icon, front_cheer, = IconDef::Icon::NONE);
	DECLARE_PROPERTY(IconDef::Icon, back, = IconDef::Icon::NONE);
	DECLARE_PROPERTY(IconDef::Icon, back_cheer, = IconDef::Icon::NONE);
	DECLARE_PROPERTY(Vector2, hue_range, = Vector2(0.0f, 1.0f));
	DECLARE_PROPERTY(double, weight, = 1.0);
	DECLARE_PROPERTY(double, pixel_size, = 0.01);
	DECLARE_PROPERTY_IS(bool, very_tall, = false);
};

#endif // AUDIENCE_DEF_H
