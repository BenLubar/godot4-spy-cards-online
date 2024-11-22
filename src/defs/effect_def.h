#ifndef EFFECT_DEF_H
#define EFFECT_DEF_H

#include "dry.h"

#include "enums/enums_effect_def.h"

class EffectDef;

#include "jigsaw/parameter/jigsaw_parameter.h"
#include "jigsaw/procedure/jigsaw_procedure.h"
#include "jigsaw/procedure/jigsaw_trigger.h"

class EffectDef : public Resource {
	GDCLASS(EffectDef, Resource);

public:
	using Effect = enums::EffectDef::Effect;

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(String, editor_name);
	DECLARE_PROPERTY(int64_t, default_priority, = 65);
	DECLARE_PROPERTY(TypedArray<JigsawParameter>, default_parameters);
	DECLARE_PROPERTY(PackedStringArray, parameter_names);
	DECLARE_PROPERTY(TypedArray<JigsawTriggerEffect>, triggers);
	DECLARE_PROPERTY(Ref<JigsawProcedureEffectDescribe>, describe);
	DECLARE_PROPERTY(Ref<JigsawProcedureEffectSimpleDescribe>, simple_describe);
	DECLARE_PROPERTY(Ref<JigsawProcedureEffectExtendedDescribe>, extended_describe);
	DECLARE_PROPERTY(Ref<JigsawProcedureEffectCurve>, curve);

	DEFAULT_TO_STRING();
};
DECLARE_PREDEFINED_KEY(EffectDef, EFFECT);

#endif // EFFECT_DEF_H
