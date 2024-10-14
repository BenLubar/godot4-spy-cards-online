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
	};

protected:
	static void _bind_methods();

public:
	VariableDef() = default;
	~VariableDef() = default;

	DECLARE_PROPERTY(String, editor_name);
	DECLARE_PROPERTY(Uniqueness, uniqueness, = Uniqueness::CARD_EFFECT_INSTANCE);
	DECLARE_PROPERTY(Ref<JigsawParameter>, default_value); // must be a non-variable type
};
DECLARE_ENUM(VariableDef::Uniqueness);

#endif // VARIABLE_DEF_H
