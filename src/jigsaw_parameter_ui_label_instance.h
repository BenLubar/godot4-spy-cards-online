#ifndef JIGSAW_PARAMETER_UI_LABEL_INSTANCE_H
#define JIGSAW_PARAMETER_UI_LABEL_INSTANCE_H

#include "jigsaw_parameter.h"

class JigsawParameterUILabelInstance : public JigsawParameter {
	GDCLASS(JigsawParameterUILabelInstance, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterUILabelInstance() = default;
	~JigsawParameterUILabelInstance() = default;

	DECLARE_PROPERTY(int64_t, label, = -1);

	Type get_type() const override { return UI_LABEL_INSTANCE; }

	static Ref<JigsawParameterUILabelInstance> make(int64_t label);
};

#endif // JIGSAW_PARAMETER_UI_LABEL_INSTANCE_H
