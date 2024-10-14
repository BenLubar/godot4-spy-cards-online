#ifndef JIGSAW_PARAMETER_ICON_H
#define JIGSAW_PARAMETER_ICON_H

#include "jigsaw_parameter.h"

#include "enums_icon_def.h"

class JigsawParameterIcon : public JigsawParameter {
	GDCLASS(JigsawParameterIcon, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterIcon() = default;
	~JigsawParameterIcon() = default;

	DECLARE_PROPERTY(enums::IconDef::Icon, icon, = enums::IconDef::Icon::NONE);

	Type get_type() const override { return ICON; }

	static Ref<JigsawParameterIcon> make(enums::IconDef::Icon icon);
};

#endif // JIGSAW_PARAMETER_ICON_H
