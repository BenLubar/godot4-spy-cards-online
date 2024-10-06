#ifndef JIGSAW_PARAMETER_LOCATION_H
#define JIGSAW_PARAMETER_LOCATION_H

#include "jigsaw_parameter.h"

class JigsawParameterLocation;

#include "location_def.h"

class JigsawParameterLocation : public JigsawParameter {
	GDCLASS(JigsawParameterLocation, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterLocation() = default;
	~JigsawParameterLocation() = default;

	DECLARE_PROPERTY(LocationDef::Location, location, = LocationDef::Location::NONE);

	Type get_type() const override { return LOCATION; }

	static JigsawParameterLocation *make(LocationDef::Location location);
};

#endif // JIGSAW_PARAMETER_LOCATION_H
