#ifndef CHOICES_DEF_H
#define CHOICES_DEF_H

#include "dry.h"

class ChoicesDef : public Resource {
	GDCLASS(ChoicesDef, Resource);

public:
	enum Choices {
		NONE = -1,

		FIRST_CUSTOM = 256,
	};

protected:
	static void _bind_methods();

public:
	ChoicesDef() = default;
	~ChoicesDef() = default;

	DECLARE_PROPERTY(String, editor_name);
	DECLARE_PROPERTY(PackedStringArray, editor_choice_names);
};
DECLARE_ENUM(ChoicesDef::Choices);

#endif // CHOICES_DEF_H
