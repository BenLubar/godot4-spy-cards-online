#ifndef CARD_INSTANCE_H
#define CARD_INSTANCE_H

#include "dry.h"

#include "enums_icon_def.h"
#include "enums_rank_def.h"

class CardInstance;

#include "card_def.h"
#include "formatted_text.h"
#include "jigsaw_global.h"
#include "modifier_instance.h"
#include "stat_value.h"

class CardInstance : public RefCounted {
	GDCLASS(CardInstance, RefCounted);

protected:
	static void _bind_methods();

public:
	CardInstance() = default;
	~CardInstance() = default;

	DECLARE_PROPERTY(JigsawGlobal *, global, = nullptr);
	DECLARE_PROPERTY(Ref<CardDef>, def);
	DECLARE_PROPERTY(TypedArray<FormattedText>, name);
	DECLARE_PROPERTY(enums::RankDef::Rank, rank, = enums::RankDef::Rank::NONE);
	DECLARE_PROPERTY(enums::IconDef::Icon, back, = enums::IconDef::Icon::NONE);
	DECLARE_PROPERTY(TypedArray<StatValue>, costs);
	DECLARE_PROPERTY(enums::IconDef::Icon, portrait, = enums::IconDef::Icon::NONE);
	DECLARE_PROPERTY(TypedArray<enums::TribeDef::Tribe>, tribes);
	DECLARE_PROPERTY(TypedArray<FormattedText>, description);
	DECLARE_PROPERTY(TypedArray<FormattedTextWithIcon>, simple_description);
	DECLARE_PROPERTY(TypedArray<ModifierInstance>, modifiers);

	void update_description();
	bool update_simple_description();

	static Ref<CardInstance> make(JigsawGlobal *global, const Ref<CardDef> &def);
};

#endif // CARD_INSTANCE_H
