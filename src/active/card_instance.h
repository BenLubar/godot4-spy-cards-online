#ifndef CARD_INSTANCE_H
#define CARD_INSTANCE_H

#include "dry.h"

#include "enums/enums_icon_def.h"
#include "enums/enums_rank_def.h"

class CardInstance;
class JigsawGlobal;

#include "defs/card_def.h"
#include "defs/formatted_text.h"
#include "active/modifier_instance.h"
#include "defs/stat_value.h"

class CardInstance : public Resource {
	GDCLASS(CardInstance, Resource);

protected:
	static void _bind_methods();

public:
	DECLARE_PROPERTY(JigsawGlobal *, global, = nullptr);
	DECLARE_PROPERTY(Ref<CardDef>, def);
	DECLARE_PROPERTY(TypedArray<FormattedText>, name);
	DECLARE_PROPERTY(enums::RankDef::Rank, rank, = enums::RankDef::Rank::NONE);
	DECLARE_PROPERTY(enums::IconDef::Icon, back, = enums::IconDef::Icon::NONE);
	DECLARE_PROPERTY(TypedArray<StatValue>, costs);
	DECLARE_PROPERTY(enums::IconDef::Icon, portrait, = enums::IconDef::Icon::NONE);
	DECLARE_PROPERTY(TypedArray<EffectInstance>, effects);
	DECLARE_PROPERTY(TypedArray<enums::TribeDef::Tribe>, tribes);
	DECLARE_PROPERTY(TypedArray<FormattedText>, description);
	DECLARE_PROPERTY(TypedArray<FormattedTextWithIcon>, simple_description);
	DECLARE_PROPERTY(TypedArray<ModifierInstance>, modifiers);
	DECLARE_PROPERTY(PackedInt32Array, face_down_for_side);

	void update_description();
	bool update_simple_description();
	bool description_requires_update() const;

	DEFAULT_TO_STRING();

	static Ref<CardInstance> make(JigsawGlobal *global, const Ref<CardDef> &def);
};

#endif // CARD_INSTANCE_H
