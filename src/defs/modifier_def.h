#ifndef MODIFIER_DEF_H
#define MODIFIER_DEF_H

#include "dry.h"

#include "enums/enums_modifier_def.h"

class ModifierDef;

#include "jigsaw/procedure/jigsaw_procedure.h"
#include "jigsaw/procedure/jigsaw_trigger.h"

class ModifierDef : public Resource {
	GDCLASS(ModifierDef, Resource);

public:
	using Modifier = enums::ModifierDef::Modifier;

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(String, editor_name);
	DECLARE_PROPERTY(TypedArray<JigsawTriggerModifier>, triggers);
	DECLARE_PROPERTY(Ref<JigsawProcedureModifierDescribe>, describe);
	DECLARE_PROPERTY(Ref<JigsawProcedureModifierSimpleDescribe>, simple_describe);
	DECLARE_PROPERTY(Ref<JigsawProcedureModifierExtendedDescribe>, extended_describe);

	DEFAULT_TO_STRING();
};
DECLARE_PREDEFINED_KEY(ModifierDef, MODIFIER);

#endif // MODIFIER_DEF_H
