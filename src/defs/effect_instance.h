#ifndef EFFECT_INSTANCE_H
#define EFFECT_INSTANCE_H

#include "dry.h"

#include "enums/enums_effect_def.h"

class EffectInstance;

#include "active/card_instance.h"
#include "defs/formatted_text.h"
#include "jigsaw/jigsaw_context.h"
#include "jigsaw/parameter/jigsaw_parameter.h"

class EffectInstance : public Resource {
	GDCLASS(EffectInstance, Resource);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(enums::EffectDef::Effect, effect, = enums::EffectDef::Effect::NONE);
	DECLARE_PROPERTY(int64_t, priority, = 65);
	DECLARE_PROPERTY(TypedArray<JigsawParameter>, params);

	TypedArray<FormattedText> format_description(JigsawGlobal *global, int64_t card_instance_id, const Ref<JigsawContext> &parent_context = Ref<JigsawContext>()) const;
	Ref<FormattedTextWithIcon> format_simple_description(JigsawGlobal *global, int64_t card_instance_id, const Ref<JigsawContext> &parent_context = Ref<JigsawContext>()) const;

	DEFAULT_TO_STRING();
};

#endif // EFFECT_INSTANCE_H
