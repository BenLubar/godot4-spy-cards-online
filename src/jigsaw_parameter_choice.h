#ifndef JIGSAW_PARAMETER_CHOICE_H
#define JIGSAW_PARAMETER_CHOICE_H

#include "jigsaw_parameter.h"

class JigsawParameterChoice;

#include "choices_def.h"

class JigsawParameterChoice : public JigsawParameter {
	GDCLASS(JigsawParameterChoice, JigsawParameter);

protected:
	static void _bind_methods();

public:
	JigsawParameterChoice() = default;
	~JigsawParameterChoice() = default;

	DECLARE_PROPERTY(ChoicesDef::Choices, choices, = ChoicesDef::Choices::NONE);
	DECLARE_PROPERTY(int64_t, index, = -1);

	Type get_type() const override { return CHOICE; }
};

#endif // JIGSAW_PARAMETER_CHOICE_H
