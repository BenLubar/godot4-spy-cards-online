#ifndef JIGSAW_PARAMETER_CHOICE_H
#define JIGSAW_PARAMETER_CHOICE_H

#include "jigsaw_parameter.h"

#include "enums_choices_def.h"

class JigsawParameterChoice : public JigsawParameter {
	GDCLASS(JigsawParameterChoice, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterChoice() = default;
	~JigsawParameterChoice() = default;

	DECLARE_PROPERTY(enums::ChoicesDef::Choices, choices, = enums::ChoicesDef::Choices::NONE);
	DECLARE_PROPERTY(int64_t, index, = -1);

	Type get_type() const override { return CHOICE; }

	static Ref<JigsawParameterChoice> make(enums::ChoicesDef::Choices choices, int64_t index);
};

#endif // JIGSAW_PARAMETER_CHOICE_H
