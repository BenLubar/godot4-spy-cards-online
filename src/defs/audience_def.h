#ifndef AUDIENCE_DEF_H
#define AUDIENCE_DEF_H

#include "dry.h"

#include "enums/enums_audience_def.h"
#include "enums/enums_icon_def.h"

class AudienceDef : public Resource {
	GDCLASS(AudienceDef, Resource);

public:
	using Audience = enums::AudienceDef::Audience;

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(String, editor_name);
	DECLARE_PROPERTY(enums::IconDef::Icon, front, = enums::IconDef::Icon::NONE);
	DECLARE_PROPERTY(enums::IconDef::Icon, front_cheer, = enums::IconDef::Icon::NONE);
	DECLARE_PROPERTY(enums::IconDef::Icon, back, = enums::IconDef::Icon::NONE);
	DECLARE_PROPERTY(enums::IconDef::Icon, back_cheer, = enums::IconDef::Icon::NONE);
	DECLARE_PROPERTY(float, weight, = 1.0f);
	DECLARE_PROPERTY(float, pixel_size, = 0.01f);

	DEFAULT_TO_STRING();
};

#endif // AUDIENCE_DEF_H
