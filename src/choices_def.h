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
	DECLARE_PROPERTY(String, editor_name);
	DECLARE_PROPERTY(PackedStringArray, editor_choice_names);

	DEFAULT_TO_STRING();
};

#endif // CHOICES_DEF_H
