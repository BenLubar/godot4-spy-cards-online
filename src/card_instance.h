#ifndef CARD_INSTANCE_H
#define CARD_INSTANCE_H

#include "dry.h"

class CardInstance;

#include "jigsaw_global.h"
#include "card_def.h"
#include "rank_def.h"
#include "tribe_def.h"
#include "formatted_text.h"
#include "modifier_instance.h"

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
	DECLARE_PROPERTY(RankDef::Rank, rank, = RankDef::Rank::NONE);
	DECLARE_PROPERTY(IconDef::Icon, back, = IconDef::Icon::NONE);
	DECLARE_PROPERTY(TypedArray<StatValue>, costs);
	DECLARE_PROPERTY(IconDef::Icon, portrait, = IconDef::Icon::NONE);
	DECLARE_PROPERTY(TypedArray<TribeDef::Tribe>, tribes);
	DECLARE_PROPERTY(TypedArray<FormattedText>, description);
	DECLARE_PROPERTY(TypedArray<ModifierInstance>, modifiers);

	static CardInstance *make(JigsawGlobal *global, CardDef *def);
};

#endif // CARD_INSTANCE_H
