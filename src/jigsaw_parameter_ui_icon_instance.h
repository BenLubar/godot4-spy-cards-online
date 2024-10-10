#ifndef JIGSAW_PARAMETER_UI_ICON_INSTANCE_H
#define JIGSAW_PARAMETER_UI_ICON_INSTANCE_H

#include "jigsaw_parameter.h"

class JigsawParameterUIIconInstance : public JigsawParameter {
	GDCLASS(JigsawParameterUIIconInstance, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterUIIconInstance() = default;
	~JigsawParameterUIIconInstance() = default;

	DECLARE_PROPERTY(int64_t, icon, = -1);

	Type get_type() const override { return UI_ICON_INSTANCE; }

	static Ref<JigsawParameterUIIconInstance> make(int64_t icon);
};

#endif // JIGSAW_PARAMETER_UI_ICON_INSTANCE_H
