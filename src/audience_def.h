#ifndef AUDIENCE_DEF_H
#define AUDIENCE_DEF_H

#include "dry.h"

#include "enums_audience_def.h"
#include "enums_icon_def.h"

class AudienceDef : public Resource {
	GDCLASS(AudienceDef, Resource);

public:
	using Audience = enums::AudienceDef::Audience;

protected:
	static void _bind_methods();

public:
	AudienceDef() = default;
	~AudienceDef() = default;

	DECLARE_PROPERTY(enums::IconDef::Icon, front, = enums::IconDef::Icon::NONE);
	DECLARE_PROPERTY(enums::IconDef::Icon, front_cheer, = enums::IconDef::Icon::NONE);
	DECLARE_PROPERTY(enums::IconDef::Icon, back, = enums::IconDef::Icon::NONE);
	DECLARE_PROPERTY(enums::IconDef::Icon, back_cheer, = enums::IconDef::Icon::NONE);
	DECLARE_PROPERTY(double, weight, = 1.0);
	DECLARE_PROPERTY(double, pixel_size, = 0.01);
	DECLARE_PROPERTY(Dictionary, json_data);
};

#endif // AUDIENCE_DEF_H
