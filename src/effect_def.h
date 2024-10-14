#ifndef EFFECT_DEF_H
#define EFFECT_DEF_H

#include "dry.h"

#include "enums_effect_def.h"

class EffectDef;

#include "jigsaw_parameter.h"
#include "jigsaw_procedure.h"
#include "jigsaw_trigger.h"

class EffectDef : public Resource {
	GDCLASS(EffectDef, Resource);

public:
	using Effect = enums::EffectDef::Effect;

protected:
	static void _bind_methods();

public:
	EffectDef() = default;
	~EffectDef() = default;

	DECLARE_PROPERTY(String, editor_name);
	DECLARE_PROPERTY(int64_t, default_priority, = 65);
	DECLARE_PROPERTY(TypedArray<JigsawParameter>, default_parameters);
	DECLARE_PROPERTY(PackedStringArray, parameter_names);
	DECLARE_PROPERTY(TypedArray<JigsawTriggerEffect>, triggers);
	DECLARE_PROPERTY(Ref<JigsawProcedureEffectDescribe>, describe);
	DECLARE_PROPERTY(Ref<JigsawProcedureEffectSimpleDescribe>, simple_describe);
	DECLARE_PROPERTY(Ref<JigsawProcedureEffectCurve>, curve);
};
DECLARE_PREDEFINED_KEY(EffectDef, EFFECT);

#endif // EFFECT_DEF_H
