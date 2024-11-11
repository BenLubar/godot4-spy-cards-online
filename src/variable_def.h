#ifndef VARIABLE_DEF_H
#define VARIABLE_DEF_H

#include "dry.h"

#include "enums_variable_def.h"

class VariableDef;

#include "jigsaw_parameter.h"

class VariableDef : public Resource {
	GDCLASS(VariableDef, Resource);

public:
	using Variable = enums::VariableDef::Variable;
	enum Uniqueness {
		CARD_EFFECT_INSTANCE = 0,
		EFFECT_QUEUE_TREE = 1,
		SIDE = 2,
		GLOBAL = 3,
	};

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(String, editor_name);
	DECLARE_PROPERTY(Uniqueness, uniqueness, = Uniqueness::CARD_EFFECT_INSTANCE);
	DECLARE_PROPERTY(Ref<JigsawParameter>, default_value); // must be a concrete type, but can be non-saved

	DEFAULT_TO_STRING();
};
DECLARE_ENUM(VariableDef::Uniqueness);
DECLARE_PREDEFINED_KEY(VariableDef, VARIABLE);

#endif // VARIABLE_DEF_H
