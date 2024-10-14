#ifndef EFFECT_INSTANCE_H
#define EFFECT_INSTANCE_H

#include "dry.h"

#include "enums_effect_def.h"

class EffectInstance;

#include "card_instance.h"
#include "formatted_text.h"
#include "jigsaw_context.h"
#include "jigsaw_parameter.h"

class EffectInstance : public Resource {
	GDCLASS(EffectInstance, Resource);

protected:
	static void _bind_methods();

public:
	EffectInstance() = default;
	~EffectInstance() = default;

	DECLARE_PROPERTY(enums::EffectDef::Effect, effect, = enums::EffectDef::Effect::NONE);
	DECLARE_PROPERTY(int64_t, priority, = 65);
	DECLARE_PROPERTY(TypedArray<JigsawParameter>, params);

	TypedArray<FormattedText> format_description(const Ref<CardInstance> &card, const Ref<JigsawContext> &parent_context = Ref<JigsawContext>()) const;
	Ref<FormattedTextWithIcon> format_simple_description(const Ref<CardInstance> &card, const Ref<JigsawContext> &parent_context = Ref<JigsawContext>()) const;
};

#endif // EFFECT_INSTANCE_H
