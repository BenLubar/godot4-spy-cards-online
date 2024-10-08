#ifndef JIGSAW_PARAMETER_ICON_H
#define JIGSAW_PARAMETER_ICON_H

#include "jigsaw_parameter.h"

class JigsawParameterIcon;

#include "icon_def.h"

class JigsawParameterIcon : public JigsawParameter {
	GDCLASS(JigsawParameterIcon, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterIcon() = default;
	~JigsawParameterIcon() = default;

	DECLARE_PROPERTY(IconDef::Icon, icon, = IconDef::Icon::NONE);

	Type get_type() const override { return ICON; }

	static Ref<JigsawParameterIcon> make(IconDef::Icon icon);
};

#endif // JIGSAW_PARAMETER_ICON_H
