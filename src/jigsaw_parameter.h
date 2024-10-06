#ifndef JIGSAW_PARAMETER_H
#define JIGSAW_PARAMETER_H

#include "dry.h"

class JigsawParameter : public Resource {
	GDCLASS(JigsawParameter, Resource);

public:
	enum Type {
		SPRITE_INSTANCE = -8,
		SCENE_INSTANCE = -7,
		QUEUED_EFFECT = -6,
		MODIFIER_INSTANCE = -5,
		CARD_INSTANCE = -4,
		FORMATTED_TEXT = -3,
		ORDERED_LIST = -2,
		LOCAL_VARIABLE = -1,
		VARIABLE = 0,
		BOOLEAN = 1,
		AMOUNT = 2,
		FLOAT = 3,
		STRING = 4,
		EFFECT_INSTANCE = 5,
		CARD_FILTER = 6,
		CARD = 7,
		STAT = 8,
		RANK = 9,
		TRIBE = 10,
		MODIFIER = 11,
		LOCATION = 12,
		ICON = 13,
		CID_OPUS = 14,
		FILE_ID_OPUS = 15,
		FILE_ID_GLTF = 16,
		COLOR = 17,
		CHOICE = 18,
	};

protected:
	static void _bind_methods();

public:
	JigsawParameter() = default;
	~JigsawParameter() = default;

	virtual Type get_type() const = 0;
};
DECLARE_ENUM(JigsawParameter::Type);

#endif // JIGSAW_PARAMETER_H
