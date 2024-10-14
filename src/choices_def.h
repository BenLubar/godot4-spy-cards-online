#ifndef CHOICES_DEF_H
#define CHOICES_DEF_H

#include "dry.h"

#include "enums_choices_def.h"

class ChoicesDef : public Resource {
	GDCLASS(ChoicesDef, Resource);

public:
	using Choices = enums::ChoicesDef::Choices;

protected:
	static void _bind_methods();

public:
	ChoicesDef() = default;
	~ChoicesDef() = default;

	DECLARE_PROPERTY(String, editor_name);
	DECLARE_PROPERTY(PackedStringArray, editor_choice_names);
};

#endif // CHOICES_DEF_H
