#ifndef MODIFIER_DEF_H
#define MODIFIER_DEF_H

#include "dry.h"

class ModifierDef;

#include "jigsaw_command_list.h"
#include "jigsaw_trigger.h"

class ModifierDef : public Resource {
	GDCLASS(ModifierDef, Resource);

public:
	enum Modifier {
		NONE = -1,
		EMPOWER_ATK = 0,
		EMPOWER_DEF = 1,
		NUMB = 2,
		PREVENT_NUMB = 3,

		FIRST_CUSTOM = 256,
	};

protected:
	static void _bind_methods();

public:
	ModifierDef() = default;
	~ModifierDef() = default;

	DECLARE_PROPERTY(String, editor_name);
	DECLARE_PROPERTY(Ref<JigsawCommandList>, describe);
	DECLARE_PROPERTY(Ref<JigsawCommandList>, simple_describe);
	DECLARE_PROPERTY(TypedArray<JigsawTrigger>, triggers);
};
DECLARE_ENUM(ModifierDef::Modifier);
DECLARE_PREDEFINED_KEY(ModifierDef, MODIFIER);

#endif // MODIFIER_DEF_H
